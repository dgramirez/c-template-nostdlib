#ifndef INCLUDE_CPU_INTRINSICS_H
#define INCLUDE_CPU_INTRINSICS_H

#include <emmintrin.h>
#include <immintrin.h>

typedef __m128i (*pfn_si128_load)(__m128i const *);
typedef    void (*pfn_si128_store)(__m128i *, __m128i);
typedef __m256i (*pfn_si256_load)(__m256i const *);
typedef    void (*pfn_si256_store)(__m256i *, __m256i);

#endif // INCLUDE_CPU_INTRINSICS_H

