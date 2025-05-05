#ifndef INCLUDE_NOSTD_COMMON_THREADS_H
#define INCLUDE_NOSTD_COMMON_THREADS_H
////////////////////////
// Platform-Specifics //
////////////////////////
#ifdef _WIN32
	typedef DWORD thread_return;
	#ifndef _ZIG
		#define __atomic_asm_prefix __fastcall
	#else
		#define __atomic_asm_prefix
	#endif

#elif __linux__
	typedef void thread_return;
	#define __atomic_asm_prefix
	#define __stdcall
#endif

/////////////
// Threads //
/////////////
declfn(long,
       thread_wait,
       unref(addr); unref(expected); return -1;,
       u32 *addr,
       u32  expected);

declfn(long,
       thread_wake_one,
       unref(addr); return -1;,
       u32 *addr);

declfn(long,
       thread_wake_all,
       unref(addr); return -1;,
       u32 *addr);

declfn(void,
       thread_exit,
       unref(exit_code);,
       int exit_code);

declfn(void *,
       thread_create,
       unref(addr); unref(arg); unref(stack_size); return 0;,
       void *addr,
	   void *arg,
       u32 stack_size);

///////////
// Locks //
///////////
typedef void *TLock;
typedef void *TMutex;

declfn(TMutex,
       mlock_init,
       unref(lock); unref(mlock); return 0;,
	   TLock   lock,
       TMutex  mlock);

declfn(void,
       mlock_acquire,
       unref(lock);,
       TMutex lock);

declfn(void,
       mlock_release,
       unref(lock);,
       TMutex lock);

declfn(void,
       mlock_free,
       unref(lock); unref(flags);,
       TMutex lock,
       u32    flags);

/////////////////
// Thread Pool //
/////////////////
typedef struct {
	MArena tmp;
	TMutex mtx;
} TData;

typedef struct __tp_stack {
	void (__stdcall *entry)(struct __tp_stack *);

	TData  tdata;
	void  *qr;
	void  *queue;
	usz   *work_count;
	usz   *quit;
} TPStack;

typedef struct {
	MArena   mem;
	TPStack *threads;
	usz      thread_count;
	usz      stack_size;
} TPData;

typedef void *TPJob;
typedef void *TPJobArgs;
typedef void (*TPJobFn)(TPJobArgs args, TData *data);

declfn(TPJob,
       tp_post,
       unref(tp_data);unref(fn);unref(args);unref(f);return 0;,
	   TPData  *tp_data,
	   TPJobFn  fn,
	   void    *args,
	   usz      f);

declfn(void,
       tp_wait,
       unref(tp_data); unref(job);,
       TPData *tp_data,
       TPJob  job);

declfn(void,
       tp_quit,
       unref(tp_data);,
       TPData *tp_data);

#endif // INCLUDE_NOSTD_COMMON_THREADS_H

