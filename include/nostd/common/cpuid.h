#ifndef INCLUDE_NOSTD_COMMON_CPUID_H
#define INCLUDE_NOSTD_COMMON_CPUID_H

// TODO: Fix this: It should contain all the appropriate CPUID Structures
//       and Functions instead of a function pointer.
//
//       Ultimatately, Send over
declfn(b8,
       cpuid_vendor,
       unref(b);unref(blen);return ((b8){0, 0});,
       u8 *b,
       usz blen);

#endif // INCLUDE_NOSTD_COMMON_CPUID_H

