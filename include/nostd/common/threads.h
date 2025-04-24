#ifndef INCLUDE_NOSTD_COMMON_THREADS_H
#define INCLUDE_NOSTD_COMMON_THREADS_H

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

declfn(u32,
       thread_create,
       unref(addr); unref(arg); unref(stack_size); return 0;,
       void *addr,
	   void *arg,
       u32 stack_size);

///////////
// Locks //
///////////
typedef void *AppLock;
typedef void *AppMLock;

declfn(AppMLock,
       mlock_init,
       unref(a); unref(lock); unref(mlock); return 0;,
       MArena   *a,
	   AppLock   lock,
       AppMLock  mlock);

declfn(void,
       mlock_acquire,
       unref(lock);,
       AppMLock *lock);

declfn(void,
       mlock_release,
       unref(lock);,
       AppMLock *lock);

/////////////////
// Thread Pool //
/////////////////
typedef void *TPData;
typedef void *TPJob;
typedef void (*TPJobFn)(void *args, TPData *data);

declfn(TPJob,
       tp_post,
       unref(tp_data);unref(fn);unref(args);unref(flags);return 0;,
	   TPData tp_data,
	   TPJobFn fn,
	   void *args,
	   usz flags);

declfn(void,
       tp_wait_all,
       unref(tp_data);,
       TPData tp_data);

declfn(void,
       tp_quit,
       unref(tp_data);,
       TPData tp_data);

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


#endif // INCLUDE_NOSTD_COMMON_THREADS_H

