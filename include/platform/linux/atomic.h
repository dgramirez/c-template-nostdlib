#ifndef INCLUDE_PLATFORM_LINUX_ATOMIC_H
#define INCLUDE_PLATFORM_LINUX_ATOMIC_H

extern void
atomic_store(void *ptr, isz val);

extern isz
atomic_xadd(void *ptr, isz val);

extern isz
atomic_cmpxchg(void *ptr, isz old_val, isz new_val);

extern isz
atomic_xchg64(void *ptr, isz val);

#endif // INCLUDE_PLATFORM_LINUX_ATOMIC_H

