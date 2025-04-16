#ifndef INCLUDE_PLATFORM_COMMON_ATOMIC_H
#define INCLUDE_PLATFORM_COMMON_ATOMIC_H

extern void __atomic_asm_prefix
_afn_atstoreI(void *ptr, i32 val);

extern void __atomic_asm_prefix
_afn_atstoreW(void *ptr, isz val);

extern void __atomic_asm_prefix
_afn_atstoreD(void *ptr, void *val);

extern isz __atomic_asm_prefix
_afn_atloadW(void *ptr);

extern i32 __atomic_asm_prefix
_afn_atloadI(void *ptr);

extern isz __atomic_asm_prefix
_afn_atcasW(void *ptr, isz old_val, isz new_val);

extern isz __atomic_asm_prefix
_afn_atcasD(void *ptr, void *old_val, void *new_val);

extern isz __atomic_asm_prefix
_afn_atswapW(void *ptr, isz val);

extern void __atomic_asm_prefix
_afn_atincW(void *ptr);

extern void __atomic_asm_prefix
_afn_atdecW(void *ptr);

extern void __atomic_asm_prefix
_afn_mfence();

extern void __atomic_asm_prefix
_afn_cpurelax();

#endif // INCLUDE_PLATFORM_COMMON_ATOMIC_H

