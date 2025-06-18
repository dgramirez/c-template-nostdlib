/**************************************************************************
 * Name:   Memory Pool (MPool, mpool.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_MPOOL__
 *     - Use this define above for this file only.
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

#ifndef __STANDALONE_MEMORY_POOL_H
#define __STANDALONE_MEMORY_POOL_H

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

#ifndef __nostd_align_pad
	#ifndef __nostd_align_over
		#define __nostd_align_over(p, a) ((__nostd_usz_t)(p) & ((a) - 1))
	#endif
	#define __nostd_align_pad(p, a) \
		(((a) - __nostd_align_over(p, a)) & ((a) - 1))
#endif

#ifndef __nostd_memzero
	#define __nostd_memzero(dst, size) memset(dst, 0, size)
#endif

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef struct __mpool_node {
	struct __mpool_node *next;
} MPoolNode;

typedef struct {
	__nostd_u8_t  *start;
	__nostd_u8_t  *end;
	MPoolNode     *head;
	__nostd_usz_t  chunk_len;
} MPool;

__nostd_api void
mpool_init(MPool        *out,
           void         *buf,
           __nostd_usz_t len,
           __nostd_usz_t chunk_len,
           __nostd_usz_t align);

__nostd_api void *
mpool_alloc(MPool *pool);

__nostd_api void
mpool_free(MPool *pool,
           void  *addr);

__nostd_api void
mpool_reset(MPool *pool);


//////////////////////////
// Function Definitions //
//////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MPOOL__)
__nostd_api void
mpool_init(MPool         *out,
           void          *buf,
           __nostd_usz_t  len,
           __nostd_usz_t  chunk_len,
           __nostd_usz_t  align)
{
	__nostd_usz_t pad;

	if (!__nostd_is_pow2(align))
		__nostd_assert(0, "Align variable is NOT power of 2!");

	pad = __nostd_align_pad(buf, align);
	out->start = (__nostd_u8_t *)buf + pad;
	out->end = out->start + len - pad;
	__nostd_assert((out->end - out->start) >= sizeof(MPoolNode),
	       "Buffer size is too small to fit one node in this pool!");

	pad = __nostd_align_pad(chunk_len, align);
	out->chunk_len = chunk_len + pad;
	__nostd_assert((__nostd_usz_t)(out->end - out->start) >= out->chunk_len,
	       "Buffer size is too small to fit one chunk in this pool!");

	mpool_reset(out);
}

__nostd_api void *
mpool_alloc(MPool *pool)
{
	MPoolNode *n;

	n = pool->head;
	if (!n)
		return 0;

	pool->head = n->next;
	__nostd_memzero(n, pool->chunk_len);
	return n;
}

__nostd_api void
mpool_free(MPool *pool,
           void  *addr)
{
	MPoolNode *n;

	n = (MPoolNode *)addr;
	if (!n)
		return;

	if ((__nostd_u8_t *)n < pool->start || (__nostd_u8_t *)n >= pool->end) {
		__nostd_assert(0, "Address is outside of this pool!");
		return;
	}

	n->next = pool->head;
	pool->head = n;
}

__nostd_api void
mpool_reset(MPool *pool)
{
	MPoolNode *c;
	__nostd_usz_t chunks;

	pool->head = 0;
	chunks = ((pool->end - pool->start) / pool->chunk_len) - 1;
	do {
		c = (MPoolNode *)(pool->start + (chunks * pool->chunk_len));
		c->next = pool->head;
		pool->head = c;
	} while (chunks--);
}

#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MPOOL__)

#endif // __STANDALONE_MEMORY_POOL_H

