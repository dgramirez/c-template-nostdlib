#ifndef INCLUDE_NOSTD_COMMON_TYPEDEFS_H
#define INCLUDE_NOSTD_COMMON_TYPEDEFS_H

typedef void *AppLock;
typedef void *AppMLock;
typedef void *FdStdOut;
typedef void *TPData;
typedef void *TPJob;
typedef void (*TPJobFn)(void *args, TPData *data);

declfn(b8,
       cpuid_vendor,
       unref(b);unref(blen);return ((b8){0, 0});,
       u8 *b,
       usz blen);

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

#endif // INCLUDE_NOSTD_COMMON_TYPEDEFS_H

