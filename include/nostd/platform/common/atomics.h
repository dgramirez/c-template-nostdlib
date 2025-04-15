#ifndef INCLUDE_PLATFORM_COMMON_ATOMIC_H
#define INCLUDE_PLATFORM_COMMON_ATOMIC_H

extern void __atomic_asm_prefix
atomic_store(void *ptr, isz val);

extern void __atomic_asm_prefix
atomic_store32(void *ptr, i32 val);

extern void __atomic_asm_prefix
atomic_store128(void *ptr, void *val);

extern isz __atomic_asm_prefix
atomic_load(void *ptr);

extern i32 __atomic_asm_prefix
atomic_load32(void *ptr);

extern isz __atomic_asm_prefix
atomic_cas(void *ptr, isz old_val, isz new_val);

extern isz __atomic_asm_prefix
atomic_cas128(void *ptr, void *old_val, void *new_val);

extern isz __atomic_asm_prefix
atomic_swap(void *ptr, isz val);

extern void __atomic_asm_prefix
atomic_inc(void *ptr);

extern void __atomic_asm_prefix
atomic_dec(void *ptr);

extern void __atomic_asm_prefix
cpu_barrier();

extern void __atomic_asm_prefix
cpu_relax();

#endif // INCLUDE_PLATFORM_COMMON_ATOMIC_H

