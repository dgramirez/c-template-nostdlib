/**************************************************************************
 * Name:   Memory Arena (MArena, marena.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION
 *     - Use this define above all nostd standalone files.
 *
 * License:
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org/>
 */

#ifndef __STANDALONE_MEMORY_ARENA_H__
#define __STANDALONE_MEMORY_ARENA_H__

////////////////////////
// Standalone Defines //
////////////////////////
#ifndef __nostd_u8_t
	#define __nostd_u8_t unsigned char
#endif

#ifndef __nostd_usz_t
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define __nostd_usz_t unsigned int
	#else
		#define __nostd_usz_t unsigned long long
	#endif
#endif

#ifndef __nostd_api
	#define __nostd_api
#endif

#ifndef __nostd_assert
	#define __nostd_assert(x, msg)
#endif

#ifndef __nostd_is_pow2
	#define __nostd_is_pow2(x) ((x) && !((x) & ((x) - 1)))
#endif

#ifndef __nostd_imin
	#ifndef __nostd_minmax_bits
		#define __nostd_minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
	#endif
	#define __nostd_imin(x, y) ((y) ^ __nostd_minmax_bits((x), (y)))
#endif

#ifndef __nostd_align_pad
	#ifndef __nostd_align_over
		#define __nostd_align_over(p, a) ((__nostd_usz_t)(p) & ((a) - 1))
	#endif
	#define __nostd_align_pad(p, a) \
		(((a) - __nostd_align_over(p, a)) & ((a) - 1))
#endif

#ifndef __nostd_memmove
	#define __nostd_memmove(dst, src, size) memmove(dst, src, size)
#endif

#ifndef __nostd_memzero
	#define __nostd_memzero(dst, size) memset(dst, 0, size)
#endif

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef struct {
	__nostd_u8_t *start;
	__nostd_u8_t *end;
	__nostd_u8_t *current;
	__nostd_u8_t *previous;
} MArena;

typedef struct {
	MArena *arena;
	__nostd_u8_t *current;
	__nostd_u8_t *previous;
} MArenaTemp;

__nostd_api void
marena_init(MArena        *out,
            void          *buf,
            __nostd_usz_t  len,
            int            align);

__nostd_api void *
marena_alloc(MArena        *arena,
             __nostd_usz_t  len,
             __nostd_usz_t  align);

__nostd_api void *
marena_realloc(MArena        *arena,
               void          *oldbuf,
               __nostd_usz_t  len,
               __nostd_usz_t  align);

__nostd_api void
marena_reset(MArena *arena);

__nostd_api void
marena_save(MArenaTemp *out,
            MArena     *arena);

__nostd_api void
marena_load(MArenaTemp *temp);

//////////////////////////
// Function Definitions //
//////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MARENA__)
__nostd_api void
marena_init(MArena        *out,
            void          *buf,
            __nostd_usz_t  len,
            int            align)
{
	__nostd_usz_t pad;

	if (!__nostd_is_pow2(align))
		__nostd_assert(0, "Align Variable is NOT power of 2!");
	pad = __nostd_align_pad(buf, align);

	out->start = (__nostd_u8_t *)buf + pad;
	out->end = out->start + len - pad;
	out->current = out->start;
	out->previous = 0;
}

__nostd_api void *
marena_alloc(MArena        *arena,
             __nostd_usz_t  len,
             __nostd_usz_t  align)
{
	__nostd_usz_t pad;

	if (!__nostd_is_pow2(align))
		__nostd_assert(0, "Align Variable is NOT power of 2!");

	pad = __nostd_align_pad(arena->current, align);
	if ((arena->current + pad + len) > arena->end)
		return 0;

	arena->previous = arena->current + pad;
	arena->current  = arena->current + pad + len;

	__nostd_memzero(arena->previous, len);
	return arena->previous;
}

__nostd_api void *
marena_realloc(MArena         *arena,
               void           *oldbuf,
               __nostd_usz_t  len,
               __nostd_usz_t  align)
{
	__nostd_usz_t oldlen;
	__nostd_usz_t pad;

	if (oldbuf && (oldbuf == arena->previous)) {
		pad = __nostd_align_pad(oldbuf, align);
		if ((arena->previous + pad + len) > arena->end)
			return 0;

		if (!__nostd_is_pow2(align))
			__nostd_assert(0, "Align Variable is NOT power of 2!");

		oldlen = arena->current - arena->previous;
		if (pad) {
			__nostd_memmove(arena->previous + pad,
			              arena->previous,
			              __nostd_imin(oldlen, len));

			arena->previous += pad;
			if ((oldlen + pad) > len)
				__nostd_memzero(arena->current + pad, len - (oldlen + pad));

			arena->current = arena->previous + len;
			return oldbuf;
		}

		if (len > oldlen)
			__nostd_memzero(arena->current, len - oldlen);

		arena->current = arena->previous + len;
		return oldbuf;
	}

	return marena_alloc(arena, len, align);
}

__nostd_api void
marena_reset(MArena *arena)
{
	arena->current = arena->start;
	arena->previous = 0;
}

__nostd_api void
marena_save(MArenaTemp *out,
            MArena     *arena)
{
	out->arena    = arena;
	out->current  = arena->current;
	out->previous = arena->previous;
}

__nostd_api void
marena_load(MArenaTemp *temp)
{
	temp->arena->current  = temp->current;
	temp->arena->previous = temp->previous;
}
#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MARENA__)

#endif // __STANDALONE_MEMORY_ARENA_H__

