#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H

typedef void *TpAppHandle;
typedef struct _tp_stack {
	// Note: Reminder that __stdcall is a requirement for xp 32-bit
	void (__stdcall *entry)(struct _tp_stack *);

	// Thread-Based Variables Below:
	// Note: Pointers here are considered *shared objects* for all threads.
	ThreadAppJobData  tdata;
	MSQueue      *queue;
	usz              *work_count;
	usz              *quit;
} TpAppStack;

////////////////
// Prototypes //
////////////////
local TpAppStack *
appjob_init_threadpool(MArena *sysmem,
                       usz     threads,
                       usz     stack_size,
                       usz     temp_size);

local HAppJob
appjob_post(TpAppHandle  tp,
            AppJobFn     fn,
            void        *arg,
            MSQueueFlags  f);

local thread_return __stdcall
appjob_thread_entry(TpAppStack *s);

local void
appjob_wait(TpAppHandle tp);

local void
appjob_quit(TpAppHandle tp);

/////////////////
// Definitions //
/////////////////
local TpAppStack *
appjob_init_threadpool(MArena *sysmem,
                       usz     threads,
                       usz     stack_size,
                       usz     temp_size)
{
	TpAppStack    *thread_stack;
	MSQueue   *shared_queue;
	MCSLock       *shared_lock;
	MPool         *shared_queue_pool;
	usz           *shared_work_count;
	usz           *shared_quit;
	u32           *shared_addr_jobpost;
	u32           *shared_addr_jobavail;
	void          *shared_pool_buffer;
	isz            pool_buffer_size;
	usz            allocated;
	usz            i;
	int            max_queue;

	if (!temp_size)
		return 0;

	// TODO: Get Logical Core Count, multiply by 2, replace "8" with value.
	threads = iclamp(1, threads, 8);
	stack_size--;
	stack_size = align_next(stack_size, page_size);
	if (temp_size) {
		temp_size--;
		temp_size = align_next(temp_size, page_size);
	}

	// Shared Objects
	shared_queue      = marena_alloc(sysmem, sizeof(MSQueue), page_size);
	allocated         = (usz)sysmem->previous;
	shared_queue_pool = marena_alloc(sysmem, sizeof(MPool), 8);
	shared_lock       = marena_alloc(sysmem, sizeof(MCSLock), 8);
	shared_work_count = marena_alloc(sysmem, sizeof(usz), 8);
	shared_quit       = marena_alloc(sysmem, sizeof(usz), 8);

	shared_addr_jobpost  = marena_alloc(sysmem, sizeof(usz), 8);
	shared_addr_jobavail = marena_alloc(sysmem, sizeof(usz), 8);

	allocated          = (usz)sysmem->current - allocated;
	pool_buffer_size   = KB(8) - allocated;
	shared_pool_buffer = marena_alloc(sysmem, pool_buffer_size, 8);
	mpool_init(shared_queue_pool,
	           shared_pool_buffer,
	           pool_buffer_size,
	           sizeof(MSQueueNode),
	           8);

	msqueue_init(shared_queue,
	                  shared_queue_pool,
	                  shared_addr_jobavail,
	                  shared_addr_jobpost);

	thread_stack = 0;
	for (i = 0; i < threads; ++i) {
		thread_stack = marena_alloc(sysmem, stack_size, page_size);

		// Note: On Linux, this sets up the actual stack frame. So the
		//       I must treat it like such by going to the end of the
		//       address and subtracting the size. On Windows, its just
		//       an argument.
		#ifdef __linux__
		thread_stack = (TpAppStack *)(
				(u8*)thread_stack + stack_size - sizeof(TpAppStack));
		#endif

		marena_init(&thread_stack->tdata.tmp,
		            marena_alloc(sysmem, temp_size, page_size),
		            temp_size,
		            8);

		max_queue = (int)((pool_buffer_size / threads) - 1);
		thread_stack->tdata.qr.max     = max_queue;
		thread_stack->tdata.qr.nodes   =
			marena_alloc(sysmem,
			             max_queue * sizeof(MSQueueNode *),
		                 page_size);
		thread_stack->tdata.qr.current = 0;

		thread_stack->tdata.mtx.lock        = shared_lock;
		thread_stack->queue                 = shared_queue;
		thread_stack->work_count            = shared_work_count;
		thread_stack->quit                  = shared_quit;

		thread_stack->entry = appjob_thread_entry;
		thread_create(appjob_thread_entry, thread_stack, stack_size);
	}

	return thread_stack;
}

local HAppJob
appjob_post(TpAppHandle   tphandle,
            AppJobFn      fn,
            void         *args,
            MSQueueFlags f)
{
	TpAppStack *tpstack;
	tpstack = (TpAppStack *)tphandle;
	HAppJob hjob;

	hjob = msqueue_enqueue(tpstack->queue, fn, args, f);
	if (!hjob)
		return 0;

	if (_afn_atloadI(tpstack->queue->addr_jobavail) == 0) {
		_afn_atstoreI(tpstack->queue->addr_jobavail, 1);
		thread_wake_one(tpstack->queue->addr_jobavail);
	}

	return hjob;
}

local void
appjob_wait(TpAppHandle tphandle)
{
	TpAppStack *s;
	s = (TpAppStack *)tphandle;

	while (_afn_atloadW(&s->queue->head.ptr->next.ptr) != 0) {
		_afn_atstoreW(s->queue->addr_jobavail, 0);
		thread_wait(s->queue->addr_jobavail, 0);
	}
}

local void
appjob_quit(TpAppHandle tphandle)
{
	TpAppStack *s;
	s = (TpAppStack *)tphandle;

	_afn_atstoreI(s->quit, 1);
	_afn_atstoreI(s->queue->addr_jobposted, 1);
	thread_wake_all(s->queue->addr_jobposted);
}

local thread_return __stdcall
appjob_thread_entry(TpAppStack *s)
{
	// Variable Section
	MArenaTemp tmp;
	fb8 fb = {0};
	MSQueueNode *job;
	AppJobFn job_fn;
	void *job_args;
	int spin;

	fb.data = marena_alloc(&s->tdata.tmp, page_size, 8);
	fb.cap  = page_size;

	marena_save(&tmp, &s->tdata.tmp);
	fb8_append(&fb, s8("New Thread Enter Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpAppStack));
	fb8_append_lf(&fb);
	log_debug(fb.b);
	marena_load(&tmp);

	do {
		// Wait for Job
		spin = 300;
		while (_afn_atloadW(&s->queue->head.ptr->next.ptr) == 0) {
			if (_afn_atloadW(s->quit))
				goto quit_tp;

			if (spin-- <= 0) {
				logc_debug("Attempting to sleep...");
				_afn_atstoreI(s->queue->addr_jobposted, 0);
				thread_wait(s->queue->addr_jobposted, 0);
			}
			else
				_afn_cpurelax();
		}

		// Quit?
		if (_afn_atloadW(s->quit))
			break;

		// Get Job
		mcs_lock(&s->tdata.mtx);
		msqueue_dequeue(s->queue, &job);
		_afn_atincW(s->work_count);
		mcs_unlock(&s->tdata.mtx);

		// Do Job
		if (job->fn) {
			job_fn = job->fn;
			job_args = job->args;
			marena_reset(&s->tdata.tmp);
			msqueue_retire(s->queue, job, &s->tdata.qr);

			job_fn(job_args, &s->tdata);
		}
		_afn_atdecW(s->work_count);

		// Free Job
		mcs_lock(&s->tdata.mtx);
		if (job->fn && _afn_atloadW(s->work_count) == 0) {
			if (_afn_atloadW(s->queue->addr_jobavail) == 0) {
				_afn_atstoreW(s->queue->addr_jobavail, 1);
				thread_wake_one(s->queue->addr_jobavail);
			}
		}
		mcs_unlock(&s->tdata.mtx);
	} while (1);
quit_tp:
	// End
	if (_afn_atloadW(s->queue->addr_jobavail) == 0) {
		_afn_atstoreW(s->queue->addr_jobavail, 1);
		thread_wake_one(s->queue->addr_jobavail);
	}

	marena_save(&tmp, &s->tdata.tmp);

	fb8_append(&fb, s8("Thread Leaving Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpAppStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&tmp);

	thread_exit(0);
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H

