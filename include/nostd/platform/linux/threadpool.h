#ifndef INCLUDE_PLATFORM_LINUX_THREADPOOL_H
#define INCLUDE_PLATFORM_LINUX_THREADPOOL_H

typedef void *TpHandle;
typedef struct _tp_stack {
	void (*entry)(struct _tp_stack *);

	// Thread-Based Variables Below:
	// Note: Pointers here are considered *shared objects* for all threads.
	ThreadItems thread_items;
	TpJobQueue *queue;
	usz        *work_count;
	usz        *quit;
	u32        *futex_jobpost;
} TpStack;

////////////////
// Prototypes //
////////////////
local TpHandle
linux_init_threadpool(MArena *sysmem,
                      usz threads,
                      usz stack_size,
                      usz temp_size);

local TpJobHandle
linux_job_post(TpHandle tp,
               TpJobFn fn,
               void *arg,
               TpFlags f);

local void
linux_tp_entry(TpStack *s);

/////////////////
// Definitions //
/////////////////
local TpHandle
linux_init_threadpool(MArena *sysmem,
                      usz threads,
                      usz stack_size,
                      usz temp_size)
{
	TpStack    *thread_stack;
	TpJobQueue *shared_queue;
	MCSLock    *shared_lock;
	MPool      *shared_queue_pool;
	usz        *shared_work_count;
	usz        *shared_quit;
	u32        *shared_futex_jobpost;
	u32        *shared_futex_jobavail;
	void       *shared_pool_buffer;
	isz         pool_buffer_size;
	usz         allocated;
	usz         i;

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
	shared_queue      = marena_alloc(sysmem, sizeof(TpJobQueue), page_size);
	allocated = (usz)sysmem->previous;
	shared_queue_pool = marena_alloc(sysmem, sizeof(MPool), 8);
	shared_lock       = marena_alloc(sysmem, sizeof(MCSLock), 8);
	shared_work_count = marena_alloc(sysmem, sizeof(usz), 8);
	shared_quit       = marena_alloc(sysmem, sizeof(usz), 8);

	shared_futex_jobpost  = marena_alloc(sysmem, sizeof(usz), 8);
	shared_futex_jobavail = marena_alloc(sysmem, sizeof(usz), 8);

	allocated = (usz)sysmem->current - allocated;
	pool_buffer_size   = KB(8) - allocated;
	shared_pool_buffer = marena_alloc(sysmem, pool_buffer_size, 8);
	mpool_init(shared_queue_pool,
	           shared_pool_buffer,
	           pool_buffer_size,
	           sizeof(TpJobNode),
	           8);

	queue_tpjob_init(shared_queue,
	                 shared_queue_pool,
	                 shared_futex_jobavail);

	for (i = 0; i < threads; ++i) {
		thread_stack = marena_alloc(sysmem, stack_size, page_size);
		thread_stack = (TpStack *)((u8*)thread_stack +
		                           stack_size -
		                           sizeof(TpStack));

		marena_init(&thread_stack->thread_items.tmp,
		            marena_alloc(sysmem, temp_size, page_size),
		            temp_size,
		            8);

		thread_stack->thread_items.mtx.lock = shared_lock;
		thread_stack->queue                 = shared_queue;
		thread_stack->work_count            = shared_work_count;
		thread_stack->quit                  = shared_quit;
		thread_stack->futex_jobpost         = shared_futex_jobpost;

		thread_stack->entry = linux_tp_entry;
		sys_clone(0x50f00, thread_stack, 0, 0, 0);
	}

	return thread_stack;
}

local TpJobHandle
linux_job_post(TpHandle tphandle,
               TpJobFn fn,
               void *args,
               TpFlags f)
{
	TpStack *tpstack;
	tpstack = (TpStack *)tphandle;
	TpJobHandle hjob;

	hjob = queue_tpjob_enqueue(tpstack->queue, fn, args, f);
	if (!hjob)
		return 0;

	if (_afn_atloadI(tpstack->futex_jobpost) == 0) {
		_afn_atstoreI(tpstack->futex_jobpost, 1);
		futex_wake(tpstack->futex_jobpost, 1);
	}

	return hjob;
}

local void
linux_job_wait(TpHandle tphandle)
{
	TpStack *s;
	s = (TpStack *)tphandle;

	if (_afn_atloadW(s->work_count) > 0) {
		_afn_atstoreW(s->queue->futex_jobavail, 0);
		futex_wait(s->queue->futex_jobavail, 0);
	}
}

local void
linux_job_quit(TpHandle tphandle)
{
	TpStack *s;
	s = (TpStack *)tphandle;

	_afn_atstoreW(s->quit, 1);
	_afn_atstoreI(s->futex_jobpost, 1);
	futex_wake_all(s->futex_jobpost);
}

local void
linux_tp_entry(TpStack *s)
{
	// Variable Section
	MArenaTemp tmp;
	fb8 fb = {0};
	TpJobNode job;
	int spin;

	// Loop
	marena_save(&tmp, &s->thread_items.tmp);
	fb.data = marena_alloc(&s->thread_items.tmp, KB(16), 8);
	fb.cap  = KB(16);

	fb8_append(&fb, s8("New Thread Enter Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&tmp);
	do {
		// Wait for Job
		while (_afn_atloadW(&s->queue->head.ptr) == 0) {
			if (_afn_atloadW(s->quit))
				break;

			spin = 300;
			if (spin-- <= 0) {
				_afn_atstoreI(s->futex_jobpost, 0);
				futex_wait(s->futex_jobpost, 0);
			}
			else
				_afn_cpurelax();
		}

		// Quit?
		if (_afn_atloadW(s->quit))
			break;

		// Get Job
		queue_tpjob_dequeue(s->queue, job);
		_afn_atincW(s->work_count);

		// Do Job
		if (job.fn) job.fn(job.args, &s->thread_items);
		_afn_atdecW(s->work_count);

		// Free Job
		mcs_lock(&s->thread_items.mtx);
		if (job.fn) {
			if (_afn_atloadW(s->queue->futex_jobavail) == 0) {
				_afn_atstoreW(s->queue->futex_jobavail, 1);
				futex_wake(s->queue->futex_jobavail, 1);
			}
		}
		mcs_unlock(&s->thread_items.mtx);
	} while (1);

	// End
	if (_afn_atloadW(s->queue->futex_jobavail) == 0) {
		_afn_atstoreW(s->queue->futex_jobavail, 1);
		futex_wake(s->queue->futex_jobavail, 1);
	}

	marena_save(&tmp, &s->thread_items.tmp);
	fb.data = marena_alloc(&s->thread_items.tmp, KB(16), 8);
	fb.cap  = KB(16);

	fb8_append(&fb, s8("Thread Leaving Thread Pool: "));
	fb8_append_hex(&fb, (usz)s + sizeof(TpStack));
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&tmp);

	sys_exit(0);
}

#endif // INCLUDE_PLATFORM_LINUX_THREADPOOL_H

