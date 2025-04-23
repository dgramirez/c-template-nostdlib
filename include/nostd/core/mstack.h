/**************************************************************************
 * Name:   Memory Arena (MArena, marena.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
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

#ifndef __STANDALONE_MEMORY_STACK_H
#define __STANDALONE_MEMORY_STACK_H

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
typedef struct __mstack_footer {
	__nostd_u8_t  *base_ptr;
	__nostd_usz_t  pad;
} MStackFooter;

typedef struct {
	__nostd_u8_t *start;
	__nostd_u8_t *end;
	__nostd_u8_t *current;
} MStack;

__nostd_api void
mstack_init(MStack        *out,
            void          *buf,
            __nostd_usz_t  len);

__nostd_api void *
mstack_alloc(MStack        *stack,
             __nostd_usz_t  len,
             __nostd_usz_t  align);

__nostd_api void *
mstack_realloc(MStack        *stack,
               void          *oldbuf,
               __nostd_usz_t  len,
               __nostd_usz_t  align);

__nostd_api void
mstack_free(MStack *stack,
            void   *ptr);

__nostd_api void
mstack_pop(MStack *stack);

__nostd_api void
mstack_reset(MStack *stack);

//////////////////////////
// Function Definitions //
//////////////////////////
__nostd_api void
mstack_init(MStack        *out,
            void          *buf,
            __nostd_usz_t  len)
{
	out->start    = buf;
	out->end      = buf + len;
	out->current  = out->start;
}

__nostd_api void *
mstack_alloc(MStack        *stack,
             __nostd_usz_t  len,
             __nostd_usz_t  align)
{
	MStackFooter *footer;
	__nostd_u8_t *ptr;
	__nostd_u8_t *ptr_end;
	__nostd_usz_t pad;

	len    += __nostd_align_pad(len, align);
	pad     = __nostd_align_pad(stack->current, align);
	ptr_end = stack->current + pad + len + sizeof(MStackFooter);
	if (ptr_end >= stack->end)
		return 0;

	ptr = stack->current + pad;
	__nostd_memzero(ptr, len + sizeof(MStackFooter));

	footer           = (MStackFooter *)(ptr_end - sizeof(MStackFooter));
	footer->base_ptr = stack->current;
	footer->pad      = pad;

	stack->current  = ptr_end;
	return ptr;
}

__nostd_api void *
mstack_realloc(MStack        *stack,
               void          *oldbuf,
               __nostd_usz_t  len,
               __nostd_usz_t  align)
{
	MStackFooter *footer;
	__nostd_u8_t *ptr;
	__nostd_u8_t *ptr_end;
	__nostd_usz_t pad;
	__nostd_usz_t oldlen;

	if (stack->current == stack->start)
		return mstack_alloc(stack, len, align);

	footer = (MStackFooter *)(stack->current - sizeof(MStackFooter));
	if (footer->base_ptr + footer->pad != oldbuf)
		return mstack_alloc(stack, len, align);

	len    += __nostd_align_pad(len, align);
	pad     = __nostd_align_pad(footer->base_ptr, align);
	ptr_end = footer->base_ptr + pad + len + sizeof(MStackFooter);
	if (ptr_end >= stack->end)
		return 0;

	ptr            = footer->base_ptr + pad;
	oldlen         = (__nostd_u8_t *)footer - (__nostd_u8_t *)oldbuf;
	stack->current = footer->base_ptr;

	// NOTE: This could override the footer data, that is ok at this point!
	//       The footer will be filled re-filled in after these changes!.
	if (pad != footer->pad)
		__nostd_memmove(ptr, oldbuf, __nostd_imin(oldlen, len));
	if (len > oldlen)
		__nostd_memzero(ptr + oldlen, len - oldlen);

	footer           = (MStackFooter *)(ptr_end - sizeof(MStackFooter));
	footer->base_ptr = stack->current;
	footer->pad      = pad;

	stack->current = ptr_end;
	return ptr;
}

__nostd_api void
mstack_free(MStack *stack, void *ptr) {
	MStackFooter *footer;

	if (stack->current == stack->start)
		return;

	footer = (MStackFooter *)(stack->current - sizeof(MStackFooter));
	if (footer->base_ptr + footer->pad != ptr)
		return;

	stack->current = footer->base_ptr;
}

__nostd_api void
mstack_pop(MStack *stack) {
	MStackFooter *footer;

	if (stack->current == stack->start)
		return;

	footer = (MStackFooter *)(stack->current - sizeof(MStackFooter));
	stack->current = footer->base_ptr;
}

__nostd_api void
mstack_reset(MStack *stack) {
	stack->current = stack->start;
}

#endif // __STANDALONE_MEMORY_STACK_H

