#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#ifndef NO_STANDARD_TYPES
	#include <stdint.h>
	#ifdef __linux__
		#include <stddef.h>
	#endif
#endif

	typedef int8_t    i8;
	typedef int16_t   i16;
	typedef int32_t   i32;
	typedef ptrdiff_t isz;

	typedef uint8_t  u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef size_t   usz;

	typedef float  f32;
	typedef double f64;

	typedef int b32;

	typedef union {
		void *pv;
		u8   *pu8;
		u16  *pu16;
		u32  *pu32;
		usz  *pusz;
	} MemUnion;

#endif // INCLUDE_PLATFORM_H

