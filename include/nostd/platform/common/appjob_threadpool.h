#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H

typedef void *TpAppHandle;
typedef struct _tp_stack {
	// Note: Reminder that __stdcall is a requirement for xp 32-bit
	void (__stdcall *entry)(struct _tp_stack *);

	// Thread-Based Variables Below:
	// Note: Pointers here are considered *shared objects* for all threads.
	ThreadAppJobData  tdata;
	AppJobQueue      *queue;
	usz              *work_count;
	usz              *quit;
	u32              *addr_jobpost;
} TpAppStack;

////////////////
// Prototypes //
////////////////
local AppJobQueue *
appjob_init_threadpool(MArena *sysmem,
                       usz     threads,
                       usz     stack_size,
                       usz     temp_size);

local HAppJob
appjob_post(TpAppHandle  tp,
            AppJobFn     fn,
            void        *arg,
            AppJobFlags  f);

local __thread_return __stdcall
appjob_thread_entry(TpAppStack *s);

/////////////////
// Definitions //
/////////////////
local AppJobQueue *
appjob_init_threadpool(MArena *sysmem,
                       usz     threads,
                       usz     stack_size,
                       usz     temp_size)
{
	TpAppStack    *thread_stack;
	AppJobQueue   *shared_queue;
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
	shared_queue      = marena_alloc(sysmem, sizeof(AppJobQueue), page_size);
	allocated = (usz)sysmem->previous;
	shared_queue_pool = marena_alloc(sysmem, sizeof(MPool), 8);
	shared_lock       = marena_alloc(sysmem, sizeof(MCSLock), 8);
	shared_work_count = marena_alloc(sysmem, sizeof(usz), 8);
	shared_quit       = marena_alloc(sysmem, sizeof(usz), 8);

	shared_addr_jobpost  = marena_alloc(sysmem, sizeof(usz), 8);
	shared_addr_jobavail = marena_alloc(sysmem, sizeof(usz), 8);

	allocated = (usz)sysmem->current - allocated;
	pool_buffer_size   = KB(8) - allocated;
	shared_pool_buffer = marena_alloc(sysmem, pool_buffer_size, 8);
	mpool_init(shared_queue_pool,
	           shared_pool_buffer,
	           pool_buffer_size,
	           sizeof(AppJobNode),
	           8);

	queue_appjob_init(shared_queue,
	                    shared_queue_pool,
	                    shared_addr_jobavail);

	for (i = 0; i < threads; ++i) {
		thread_stack = marena_alloc(sysmem, stack_size, page_size);

		// Note: On Linux, this sets up the actual stack frame. So the
		//       I must treat it like such by going to the end of the
		//       address and subtracting the size. On Windows, its just
		//       an argument.
		#ifdef __linux__
		thread_stack = (TpAppStack *)((u8*)thread_stack +
		                           stack_size -
		                           sizeof(TpAppStack));
		#endif

		marena_init(&thread_stack->tdata.tmp,
		            marena_alloc(sysmem, temp_size, page_size),
		            temp_size,
		            8);

		thread_stack->tdata.mtx.lock        = shared_lock;
		thread_stack->queue                 = shared_queue;
		thread_stack->work_count            = shared_work_count;
		thread_stack->quit                  = shared_quit;
		thread_stack->addr_jobpost          = shared_addr_jobpost;

		thread_stack->entry = appjob_thread_entry;
		__thread_create(appjob_thread_entry, thread_stack, stack_size);
	}

	return shared_queue;
}

local HAppJob
appjob_post(TpAppHandle   tphandle,
            AppJobFn      fn,
            void         *args,
            AppJobFlags f)
{
	TpAppStack *tpstack;
	tpstack = (TpAppStack *)tphandle;
	HAppJob hjob;

	hjob = queue_appjob_enqueue(tpstack->queue, fn, args, f);
	if (!hjob)
		return 0;

	if (atomic_load(tpstack->addr_jobpost) == 0) {
		atomic_store(tpstack->addr_jobpost, 1);
		__thread_wake_one(tpstack->addr_jobpost);
	}

	return hjob;
}

local void
appjob_wait(TpAppHandle tphandle)
{
	TpAppStack *s;
	s = (TpAppStack *)tphandle;

	if (atomic_load(s->work_count) > 0) {
		atomic_store(s->queue->addr_jobavail, 0);
		__thread_wait(s->queue->addr_jobavail, 0);
	}
}

local void
appjob_quit(TpAppHandle tphandle)
{
	TpAppStack *s;
	s = (TpAppStack *)tphandle;

	atomic_store(s->quit, 1);
	atomic_store(s->addr_jobpost, 1);
	__thread_wake_all(s->addr_jobpost);
}

local __thread_return __stdcall
appjob_thread_entry(TpAppStack *s)
{
	// Variable Section
	MArenaTemp tmp;
	fb8 fb = {0};
	AppJobNode *job;
	int spin;

	// Loop
	marena_save(&tmp, &s->tdata.tmp);
	fb.data = marena_alloc(&s->tdata.tmp, page_size, 8);
	fb.cap  = page_size;

	fb8_append(&fb, s8("New Thread Enter Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpAppStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&tmp);
	do {
		// Wait for Job
		while (atomic_load(&s->queue->head.ptr) == 0) {
			if (atomic_load(s->quit))
				break;

			spin = 300;
			if (spin-- <= 0) {
				atomic_store(s->addr_jobpost, 0);
				__thread_wait(s->addr_jobpost, 0);
			}
			else
				cpu_relax();
		}

		// Quit?
		if (atomic_load(s->quit))
			break;

		// Get Job
		queue_appjob_dequeue(s->queue, &job);
		atomic_inc(s->work_count);

		// Do Job
		if (job) job->fn(job->args, &s->tdata);
		atomic_dec(s->work_count);

		// Free Job
		mcs_lock(&s->tdata.mtx);
		mpool_free(s->queue->pool, job);
		if (atomic_load(s->queue->addr_jobavail) == 0) {
			atomic_store(s->queue->addr_jobavail, 1);
			__thread_wake_one(s->queue->addr_jobavail);
		}
		mcs_unlock(&s->tdata.mtx);
	} while (1);

	// End
	if (atomic_load(s->queue->addr_jobavail) == 0) {
		atomic_store(s->queue->addr_jobavail, 1);
		__thread_wake_one(s->queue->addr_jobavail);
	}

	marena_save(&tmp, &s->tdata.tmp);
	fb.data = marena_alloc(&s->tdata.tmp, KB(16), 8);
	fb.cap  = KB(16);

	fb8_append(&fb, s8("Thread Leaving Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpAppStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&tmp);

	__thread_exit();
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_THREADPOOL_APPJOB_H

