#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#ifndef NO_STANDARD_TYPES
	#include <stdint.h>
	#ifdef __linux__
		#include <stddef.h>
		#include <bits/types/struct_timespec.h>

		#ifndef	__ssize_t_defined
		typedef __ssize_t ssize_t;
		# define __ssize_t_defined
		#endif
		
		# ifndef __off_t_defined
		#  ifndef __USE_FILE_OFFSET64
		typedef __off_t off_t;
		#  else
		typedef __off64_t off_t;
		#  endif
		#  define __off_t_defined
		# endif
		# if defined __USE_LARGEFILE64 && !defined __off64_t_defined
		typedef __off64_t off64_t;
		#  define __off64_t_defined
		# endif

		#ifndef __mode_t_defined
		typedef __mode_t mode_t;
		# define __mode_t_defined
		#endif
	#endif

	#ifdef _WIN32
		typedef ptrdiff_t ssize_t;
	#endif
#endif

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef ssize_t isz;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef size_t   usz;

typedef float  f32;
typedef double f64;

#endif // INCLUDE_PLATFORM_H

