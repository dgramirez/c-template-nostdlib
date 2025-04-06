#ifndef INCLUDE_PLATFORM_COMMON_ATOMIC_H
#define INCLUDE_PLATFORM_COMMON_ATOMIC_H

extern void __atomic_asm_prefix
atomic_store(void *ptr, isz val);

extern isz __atomic_asm_prefix
atomic_load(void *ptr);

extern isz __atomic_asm_prefix
atomic_xadd(void *ptr, isz val);

extern isz __atomic_asm_prefix
atomic_cmpxchg(void *ptr, isz old_val, isz new_val);

extern isz __atomic_asm_prefix
atomic_xchg64(void *ptr, isz val);

extern void __atomic_asm_prefix
atomic_inc(void *ptr);

extern void __atomic_asm_prefix
atomic_dec(void *ptr);

extern void __atomic_asm_prefix
mem_barrier();

extern void __atomic_asm_prefix
cpu_relax();

#endif // INCLUDE_PLATFORM_COMMON_ATOMIC_H

