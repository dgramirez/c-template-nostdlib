#ifndef INCLUDE_NOSTD_COMMON_TYPEDEFS_H
#define INCLUDE_NOSTD_COMMON_TYPEDEFS_H

typedef void *AppLock;
typedef void *AppMLock;
typedef void *FdStdOut;

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

#endif // INCLUDE_NOSTD_COMMON_TYPEDEFS_H

