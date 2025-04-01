#ifndef INCLUDE_PLATFORM_LINUX_ATOMIC_H
#define INCLUDE_PLATFORM_LINUX_ATOMIC_H

extern void
atomic_store(void *ptr, isz val);

extern isz
atomic_load(void *ptr);

extern isz
atomic_xadd(void *ptr, isz val);

extern isz
atomic_cmpxchg(void *ptr, isz old_val, isz new_val);

extern isz
atomic_xchg64(void *ptr, isz val);

extern void
atomic_inc(void *ptr);

extern void
atomic_dec(void *ptr);

extern void
mem_barrier();

extern void
cpu_relax();

#endif // INCLUDE_PLATFORM_LINUX_ATOMIC_H

