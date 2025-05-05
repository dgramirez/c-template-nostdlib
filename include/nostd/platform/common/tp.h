#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_TP_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_TP_H

////////////////
// Prototypes //
////////////////
local void
tp_init_generic(TPData *tp,
                usz     thread_count,
                usz     stack_size,
                usz     temp_size,
                usz     job_count);

local TPJob
tp_post_generic(TPData   *tpdata,
                TPJobFn   fn,
                void     *args,
                usz       f);

local void
tp_wait_generic(TPData *tpdata,
                TPJob   job);

local void
tp_quit_generic(TPData *tpdata);

local thread_return __stdcall
tp_entry_generic(TPStack *s);

local void
tp_entry_debug_init(TPStack *s);

local MSQueueNode *
tp_entry_get_job(TPStack *s);

local void
tp_entry_end_job(TPStack *s);

local void
tp_entry_close(TPStack *s);

/////////////////
// Definitions //
/////////////////
local void
tp_init_generic(TPData *tp,
                usz     thread_count,
                usz     stack_size,
                usz     temp_size,
                usz     job_count)
{
	TPStack              tps;
	TPStack             *tps_tosend;
	MSQueue             *q;
	MSQueueRetiredNodes *qr;
	MSQueueNode        *qr_nodes;
	MCSLock             *lock;
	MCSMutex            *mutexes;
	usz                  pool_len;
	usz                  i;
	usz                  memlen;
	usz                  job_size;

	if (!tp)
		return;

	assert(_cpuid.id != CPUID_VENDOR_ERROR && _cpuid.cpu,
	       "CPUID Needs to be initialized correctly. . .");

	if (_cpuid.id == CPUID_VENDOR_AuthenticAMD) {
		thread_count = iclamp(1,
		                      thread_count,
		                      _cpuid.amd->identifier_and_features.lcpu_count);
	}
	else if (_cpuid.id == CPUID_VENDOR_GenuineIntel) {
		thread_count = iclamp(1,
		                      thread_count,
		                      4);
	}
	else {
		thread_count = iclamp(1,
		                      thread_count,
		                      4);
	}

	stack_size += align_pad(imax(stack_size, page_size), page_size);
	temp_size  += align_pad(imax(temp_size, page_size), page_size);

	// Note: Due to the retired node technique, we cannot have all jobs
	//       being held up. 1 page == 128 jobs && 16 threads == 128 jobs.
	//       So we're multiplying the page count for every 16 threads.
	//
	//       For x86, I'll be honest the target x86 platform is Windows XP
	//       32-bit SP3. If there is an 8-core cpu w/ 16 threads out there
	//       with XP drivers, then I will apologize for the wasted space.
	//
	//       Lastly, this code is currently not designed with x86 servers
	//       in mind, neither linux or bsd. just Windows XP. Sorry!
	job_count  = imax(job_count,
	                  (page_size * (1 + (thread_count >> 4))) / sizeof(MSQueueNode));
	job_size   = job_count * sizeof(MSQueueNode);
	job_size  += align_pad(job_size, page_size);

	if (tp->mem.start == 0) {
		// Diagram:
		// +------+-----+------+-----+-----------------------+----------
		// | BASE | PAD | JOBS | PAD | THRD1 STK & THRD1 TMP | THRD2 ...
		// +------+-----+------+-----+-----------------------+----------
		#define TP_LEN_BASE                 (sizeof(MSQueueRetiredNodes) + \
			                                 sizeof(MSQueue)             + \
											 sizeof(usz)                 + \
											 sizeof(usz)                 + \
											 sizeof(MPool)               + \
											 sizeof(usz)                 + \
											 sizeof(usz)                 + \
											 sizeof(MCSLock)               \
		                                    )
		#define TP_LEN_PAD                  (KB(4) << 1)
		#define TP_LEN_MPOOL_JOBS           (job_size)
		#define TP_LEN_QRET_PER_THREAD      ((sizeof(MSQueueRetiredNodes) * \
		                                     thread_count) +                \
		                                     ((sizeof(MSQueueNode *) << 3) * \
		                                     thread_count)\
		                                    )
		#define TP_LEN_MUTEX_PER_THREAD     (sizeof(MCSMutex) * thread_count)
		#define TP_LEN_STACK_PER_THREAD     (stack_size * thread_count)
		#define TP_LEN_TEMPSZ_PER_THREAD    (temp_size  * thread_count)

		memlen = TP_LEN_BASE + TP_LEN_MPOOL_JOBS + TP_LEN_PAD +
		         TP_LEN_STACK_PER_THREAD + TP_LEN_TEMPSZ_PER_THREAD +
		         TP_LEN_MUTEX_PER_THREAD + TP_LEN_QRET_PER_THREAD;

		marena_init(&tp->mem,
		            mfreelist_alloc(&_sysfl, memlen, page_size),
		            memlen,
		            word_size);
	}

	tps.queue      = marena_alloc(&tp->mem, sizeof(MSQueue), word_size);
	tps.work_count = marena_alloc(&tp->mem, sizeof(usz*),    word_size);
	tps.quit       = marena_alloc(&tp->mem, sizeof(usz*),    word_size);

	q                 = (MSQueue *)tps.queue;
	q->pool           = marena_alloc(&tp->mem, sizeof(MPool), word_size);
	q->addr_jobavail  = marena_alloc(&tp->mem, sizeof(u32*),  word_size);
	q->addr_jobposted = marena_alloc(&tp->mem, sizeof(u32*),  word_size);

	qr          = marena_alloc(&tp->mem,
	                           sizeof(MSQueueRetiredNodes) * thread_count,
	                           word_size);
	qr_nodes    = marena_alloc(&tp->mem,
	                           (sizeof(MSQueueNode *) * thread_count ) << 3,
	                           word_size);

	lock    = (MCSLock  *)marena_alloc(&tp->mem, sizeof(MCSLock), word_size);
	mutexes = (MCSMutex *)marena_alloc(&tp->mem,
	                                    sizeof(MCSMutex) * thread_count,
	                                    word_size);

	pool_len = align_pad(tp->mem.current, page_size) + job_size;
	mpool_init(q->pool,
	           marena_alloc(&tp->mem, pool_len, word_size),
			   pool_len,
			   sizeof(MSQueueNode),
			   word_size);
	msqueue_init(q, q->pool, q->addr_jobavail, q->addr_jobposted);

	for (i = 0; i < thread_count; ++i) {
		#ifdef __linux__
			tps_tosend = marena_alloc(&tp->mem, stack_size, page_size);
			tps_tosend = tps_tosend + (stack_size / sizeof(TPStack)) - 1;
		#else
			tps_tosend = marena_alloc(&tp->mem, sizeof(TPStack), page_size);
		#endif

		if (i == 0)
			tp->threads = tps_tosend;

		tps_tosend->qr = &qr[i];
		qr[i].nodes    = &qr_nodes[i << 3];
		qr[i].current  = 0;
		qr[i].max      = 8;
		marena_init(&tps_tosend->tdata.tmp,
		            marena_alloc(&tp->mem, temp_size, 8),
		            temp_size,
		            8);

		mutexes[i].lock            = lock;
		tps_tosend->tdata.mtx      = &mutexes[i];
		tps_tosend->queue          = tps.queue;
		tps_tosend->work_count     = tps.work_count;
		tps_tosend->quit           = tps.quit;

		tps_tosend->entry = tp_entry_generic;
		thread_create(tp_entry_generic, tps_tosend, stack_size);
	}

	tp->thread_count = thread_count;
}

local TPJob
tp_post_generic(TPData   *tpdata,
                TPJobFn   fn,
                void     *args,
                usz       f)
{
	MSQueue *q;
	TPJob hjob;

	q = tpdata->threads[0].queue;
	hjob = msqueue_enqueue(q, fn, args, f);
	if (!hjob)
		return 0;

	if (atloadI(q->addr_jobavail) == 0) {
		atstoreI(q->addr_jobavail, 1);
		thread_wake_one(q->addr_jobavail);
	}

	return hjob;
}

local void
tp_wait_generic(TPData *tpdata,
                TPJob   job)
{
	MSQueue *q;

	q = (MSQueue *)tpdata->threads[0].queue;
	if (!job) {
		while (atloadW(&q->head.ptr->next.ptr) != 0) {
			atstoreI(q->addr_jobavail, 0);
			thread_wait(q->addr_jobavail, 0);
		}
	}
	else {
		// TODO: In Post, Use freelist to create a futex for the new node
		//       and set that to the node.
	}
}

local void
tp_quit_generic(TPData *tpdata)
{
	MSQueue *q;
	q = (MSQueue *)tpdata->threads[0].queue;

	atstoreI(tpdata->threads[0].quit, 1);
	atstoreI(q->addr_jobposted, 1);
	thread_wake_all(q->addr_jobposted);

	tp_wait_generic(tpdata, 0);
	mfreelist_free(&_sysfl, tpdata->mem.start);
}

local thread_return __stdcall
tp_entry_generic(TPStack *s)
{
	MSQueueNode *job;
	TPJobFn      fn;
	TPJobArgs    args;

	tp_entry_debug_init(s);
	do {
		job = tp_entry_get_job(s);
		if (atloadW(s->quit)) break;
		if (!job) continue;

		fn   = job->fn;
		args = job->args;
		msqueue_retire(s->queue, job, s->qr);

		marena_reset(&s->tdata.tmp);
		fn(args, &s->tdata);

		tp_entry_end_job(s);
	} while (1);

	tp_entry_close(s);
	thread_exit(0);
}

local void
tp_entry_debug_init(TPStack *s)
{
	fb8        fb = {0};

	marena_reset(&s->tdata.tmp);
	fb.cap  = page_size;
	fb.data = marena_alloc(&s->tdata.tmp, page_size, 8);

	fb8_append(&fb, s8("New Thread Enter Pool: "));
	fb8_append_hex_cap(&fb, (usz)s + sizeof(TPStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_reset(&s->tdata.tmp);
}

local MSQueueNode *
tp_entry_get_job(TPStack *s)
{
	MSQueue     *q;
	MSQueueNode *job;
	int          spin;
	long         val;

	q    = (MSQueue *)s->queue;
	spin = 300;
	job = 0;
	do {
		while (atloadW(&q->head.ptr->next.ptr) == 0) {
			if (atloadW(s->quit))
				return 0;

			if (spin-- <= 0) {
				logc_debug("Attempting to sleep. . .");
				atstoreI(q->addr_jobposted, 0);
				val = (long)thread_wait(q->addr_jobposted, 0);
				unref(val);
			}
			else
				cpu_relax();
		}

		msqueue_dequeue(q, &job);
	} while (!job);

	atincW(s->work_count);
	return job;
}

local void
tp_entry_end_job(TPStack *s)
{
	MSQueue *q;

	q = (MSQueue *)s->queue;
	mlock_acquire(s->tdata.mtx);

	*s->work_count -= 1;
	if (*q->addr_jobavail == 0) {
		*q->addr_jobavail = 1;
		thread_wake_all(q->addr_jobavail);
	}
	mlock_release(s->tdata.mtx);
}

local void
tp_entry_close(TPStack *s)
{
	fb8      fb = {0};
	MSQueue *q;

	q = (MSQueue *)s->queue;
	if (atloadW(q->addr_jobavail) == 0) {
		atstoreW(q->addr_jobavail, 1);
		thread_wake_all(q->addr_jobavail);
	}

	marena_reset(&s->tdata.tmp);
	fb.cap  = page_size;
	fb.data = marena_alloc(&s->tdata.tmp, page_size, 8);

	fb8_append(&fb, s8("Thread Leaving Thread Pool: "));
	fb8_append_hex_cap(&fb, (usz)s + sizeof(TPStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_TP_H

