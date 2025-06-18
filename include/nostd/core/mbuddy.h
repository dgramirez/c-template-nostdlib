/**************************************************************************
 * Name:   Memory Buddy (MBuddy, mbuddy.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_MBUDDY__
 *     - Use this define above for this file only.
 *
 * Notes:
 *     This is the only allocator that requires an additional allocation.
 *     This is because I wanted to keep this allocator true to its function:
 *     being a power-of-two allocator. This contains **NO** headers and
 *     **NO** internal metadata.
 *
 *     To use this buffer, you must allocate a bitmap. This can be allocated
 *     statically or dynamically, as long as all the bits required are
 *     readable & writeable. To get the required bits, use this function:
 *         size_t mbuddy_get_bitmap_len(size_t len);
 *
 *     This allocator is optimized for space over speed. The idea is to use
 *     this allocator as the starting allocator, and use the appropriate
 *     allocator for the problem at hand.
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

#ifndef __nostd_isz_t
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define __nostd_isz_t signed int
	#else
		#define __nostd_isz_t signed long long
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

#ifndef __nostd_imax
	#ifndef __nostd_minmax_bits
		#define __nostd_minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
	#endif
	#define __nostd_imax(x, y) ((x) ^ __nostd_minmax_bits((x), (y)))
#endif

#ifndef __nostd_within_addr
	#define __nostd_within_addr(x, addr, size) \
		(((char*)x >= (char*)addr) && ((char*)x < ((char*)addr + size)))
#endif

#ifndef __nostd_ceilto_pow2
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define __nostd_ceilto_pow2(x) \
		x--; \
		x |= x >> 1;  \
		x |= x >> 2;  \
		x |= x >> 4;  \
		x |= x >> 8;  \
		x |= x >> 16; \
		x++;
	#else
		#define __nostd_ceilto_pow2(x) \
		x--; \
		x |= x >> 1;  \
		x |= x >> 2;  \
		x |= x >> 4;  \
		x |= x >> 8;  \
		x |= x >> 16; \
		x |= x >> 32; \
		x++;
	#endif
#endif

#ifndef __nostd_log2
	#ifndef __nostd_log2_init
		#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
			#define __nostd_log2_init()                  \
				const __nostd_usz_t __nostd_bits[] = {   \
					0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000   \
				};                                       \
				                                         \
				const __nostd_usz_t __nostd_bitpos[] = { \
					1, 2, 4, 8, 16                       \
				}
		#else
			#define __nostd_log2_init()                                    \
				const __nostd_usz_t __nostd_bits[] = {                     \
					0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000, 0xFFFFFFFF00000000 \
				};                                       \
				                                                           \
				const __nostd_usz_t __nostd_bitpos[] = {                   \
					1, 2, 4, 8, 16, 32                                     \
				}

		#endif // ARCH_DEFINE
	#endif //__nostd_log2_init 
		#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
			#define __nostd_log2(retvar, tmpvar)   \
				if (tmpvar   & __nostd_bits[4]) {    \
					tmpvar >>= __nostd_bitpos[4];   \
					retvar  |= __nostd_bitpos[4]; \
				} \
				if (tmpvar   & __nostd_bits[3]) {    \
					tmpvar >>= __nostd_bitpos[3];   \
					retvar  |= __nostd_bitpos[3]; \
				} \
				if (tmpvar   & __nostd_bits[2]) {    \
					tmpvar >>= __nostd_bitpos[2];   \
					retvar  |= __nostd_bitpos[2]; \
				} \
				if (tmpvar   & __nostd_bits[1]) {    \
					tmpvar >>= __nostd_bitpos[1];   \
					retvar  |= __nostd_bitpos[1]; \
				} \
				if (tmpvar   & __nostd_bits[0]) { \
					tmpvar >>= __nostd_bitpos[0];  \
					retvar  |= __nostd_bitpos[0];  \
				}
		#else
			#define __nostd_log2(retvar, tmpvar)   \
				if (tmpvar & __nostd_bits[5]) {    \
					tmpvar >>= __nostd_bitpos[5];   \
					retvar |= __nostd_bitpos[5]; \
				} \
				if (tmpvar & __nostd_bits[4]) {    \
					tmpvar >>= __nostd_bitpos[4];   \
					retvar |= __nostd_bitpos[4]; \
				} \
				if (tmpvar & __nostd_bits[3]) {    \
					tmpvar >>= __nostd_bitpos[3];   \
					retvar |= __nostd_bitpos[3]; \
				} \
				if (tmpvar & __nostd_bits[2]) {    \
					tmpvar >>= __nostd_bitpos[2];   \
					retvar |= __nostd_bitpos[2]; \
				} \
				if (tmpvar & __nostd_bits[1]) {    \
					tmpvar >>= __nostd_bitpos[1];   \
					retvar |= __nostd_bitpos[1]; \
				} \
				if (tmpvar & __nostd_bits[0]) {    \
					tmpvar >>= __nostd_bitpos[0];   \
					retvar |= __nostd_bitpos[0]; \
				}
		#endif // ARCH_DEFINE
#endif //__nostd_log2

#ifndef __nostd_memzero
	#define __nostd_memzero(dst, size) memset(dst, 0, size)
#endif

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
#define MBUDDY_MIN_LEN (__nostd_usz_t)4096
#define MBUDDY_MIN_ORDER 12
#define MBUDDY_GET_INDEX(offset, order, block_order) \
	(1 << (b->max_order - order)) - 1 + (offset >> block_order)

typedef struct {
	__nostd_u8_t  *buf;
	__nostd_usz_t  len;
	__nostd_usz_t  max_order;
	__nostd_u8_t  *bitmap;
} MBuddy;

__nostd_api void
mbuddy_init(MBuddy        *b,
            void          *buf,
            __nostd_usz_t  len,
            void          *bitmap);

__nostd_api void *
mbuddy_alloc(MBuddy        *b,
             __nostd_usz_t  len);

__nostd_api void
mbuddy_free(MBuddy *b,
            void   *ptr);

__nostd_api void
mbuddy_reset(MBuddy *b);

__nostd_api __nostd_usz_t
mbuddy_get_bitmap_len(__nostd_usz_t len);

__nostd_api void
mbuddy_set_bit(__nostd_u8_t *bitmap, __nostd_usz_t i);

__nostd_api void
mbuddy_clear_bit(__nostd_u8_t *bitmap, __nostd_usz_t i);

__nostd_api int
mbuddy_test_bit(__nostd_u8_t *bitmap, __nostd_usz_t i);

//////////////////////////
// Function Definitions //
//////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MBUDDY__)

__nostd_api void
mbuddy_init(MBuddy        *b,
            void          *buf,
            __nostd_usz_t  len,
            void          *bitmap)
{
	__nostd_usz_t blocks;
	__nostd_usz_t levels;
	__nostd_log2_init();

	__nostd_assert(__nostd_is_pow2(len), "arg2 (len) is not a power of 2!")
	if (!len) return;

	b->buf    = buf;
	b->len    = len;
	b->bitmap = bitmap;

	levels = 0;
	blocks = len >> MBUDDY_MIN_ORDER;
	__nostd_log2(levels, blocks);
	b->max_order = levels;

	mbuddy_reset(b);
}

__nostd_api void *
mbuddy_alloc2(MBuddy        *b,
             __nostd_usz_t  len)
{
    __nostd_usz_t  order;
    __nostd_usz_t  order_size;
    __nostd_usz_t  max_index;
    __nostd_usz_t  index;
    __nostd_usz_t  depth;
    __nostd_usz_t  i;
    __nostd_usz_t  current_order;
    __nostd_usz_t  split_index;
    __nostd_usz_t  leaf_offset;
    __nostd_usz_t  offset;
    void          *ptr;
	__nostd_log2_init();

	if (!len || len > b->len)
		return 0;

	__nostd_ceilto_pow2(len);
	order_size = MBUDDY_MIN_LEN;
	order = 0;
	while (order_size < len) {
		order_size <<= 1;
		order++;
	}

	if (order > b->max_order)
		return 0;

	max_index = ((__nostd_usz_t)1 << (b->max_order + 1)) - 1;
	for (index = 0; index < max_index; ++index) {
		depth = 0;
		i = index + 1;
		__nostd_log2(depth, i);
		current_order = b->max_order - depth;

		if (current_order < order)
			continue;

		if (!mbuddy_test_bit(b->bitmap, index)) {
			split_index = index;
			while (current_order > order) {
				mbuddy_set_bit(b->bitmap, split_index);
				split_index = 2 * split_index + 1;
				current_order--;
			}
			mbuddy_set_bit(b->bitmap, split_index);

			leaf_offset = (split_index - ((1 << (b->max_order - order)) - 1));
			offset      = leaf_offset * (MBUDDY_MIN_LEN << order);
			ptr         = b->buf + offset;

			__nostd_memzero(ptr, (MBUDDY_MIN_LEN << order));
			return ptr;
		}
	}

	return 0;
}

__nostd_api void *
mbuddy_alloc(MBuddy        *b,
             __nostd_usz_t  len)
{
	__nostd_usz_t  offset;
	__nostd_usz_t  offset_max;
	__nostd_usz_t  offset_buddy;
	__nostd_usz_t  offset_p;
	__nostd_usz_t  i;
	__nostd_usz_t  buddy_i;
	__nostd_usz_t  parent_i;
	__nostd_usz_t  order;
	__nostd_usz_t  order_v;
	__nostd_usz_t  block_len;
	__nostd_usz_t  block_order;
	__nostd_usz_t  block_len_v;
	__nostd_usz_t  block_order_v;

	void          *ptr;
	__nostd_log2_init();

	if (!len || len > b->len)
		return 0;
	__nostd_ceilto_pow2(len);

	offset      = 0;
	offset_max  = (__nostd_usz_t)((b->buf + b->len) - b->buf);
	block_len   = len;

	block_order = 0;
	__nostd_log2(block_order, len);
	order       = block_order - MBUDDY_MIN_ORDER;

	ptr = 0;
//	while (offset < offset_max) {
//		i = (1 << (b->max_order - order)) - 1 + (offset >> block_order);
//		if (!mbuddy_test_bit(b->bitmap, i)) {
//			mbuddy_set_bit(b->bitmap, i);
//			ptr = b->buf + offset;
//			break;
//		}
//		offset += block_len;
//	}
	while (offset < offset_max) {
		i = MBUDDY_GET_INDEX(offset, order, block_order);
		if (i == 0) { // Root Index
			if (mbuddy_test_bit(b->bitmap, i)) {
				offset += b->len;
				break;
			}

			mbuddy_set_bit(b->bitmap, i);
			ptr = b->buf + offset;
			break;
		}

		// Buddy
		offset_buddy = offset ^ block_len;
		buddy_i = MBUDDY_GET_INDEX(offset_buddy, order, block_order);
		if (mbuddy_test_bit(b->bitmap, i)) {
			if (mbuddy_test_bit(b->bitmap, buddy_i)) {
				offset += (block_len << 1);
				continue;
			}

			offset = offset_buddy;
			mbuddy_set_bit(b->bitmap, buddy_i);
			ptr = b->buf + offset;
			break;
		}
		else {
			if (mbuddy_test_bit(b->bitmap, buddy_i)) {
				mbuddy_set_bit(b->bitmap, i);
				ptr = b->buf + offset;
				break;
			}
		}

		// Parent
		order_v       = order;
		offset_p      = offset;
		block_len_v   = block_len;
		block_order_v = block_order;
		while (1) {
			block_len_v <<= 1;
			order_v++;
			block_order_v++;

			if ((offset_p & (block_len_v - 1)) != 0)
				offset_p -= (block_len_v >> 1);

			parent_i = MBUDDY_GET_INDEX(offset_p, order_v, block_order_v);
			if (parent_i == 0)
				break;

			if (mbuddy_test_bit(b->bitmap, parent_i)) {
				offset = offset_p + block_len_v;
				break;
			}
		}

		if (parent_i == 0) {
			mbuddy_set_bit(b->bitmap, i);
			ptr = b->buf + offset;
			break;
		}
	}
	if (offset >= offset_max)
		return 0;

	++order;
	++block_order;
	block_len <<= 1;
	while (order <= b->max_order) {
		if ((offset & (block_len - 1)) != 0)
			offset -= (block_len >> 1);

		i = (1 << (b->max_order - order)) - 1 + (offset >> block_order);
		if (mbuddy_test_bit(b->bitmap, i))
			break;
		mbuddy_set_bit(b->bitmap, i);

		++order;
		++block_order;
		block_len <<= 1;
	}

	return ptr;
}

__nostd_api void
mbuddy_free(MBuddy *b,
            void    *ptr)
{
	__nostd_usz_t offset;
	__nostd_usz_t order;
	__nostd_usz_t i;
	__nostd_usz_t block_len;
	__nostd_usz_t block_order;

	__nostd_usz_t offset_buddy;
	__nostd_usz_t i_buddy;

	if (!ptr || !__nostd_within_addr(ptr, b->buf, b->len))
		return;

	offset      = (__nostd_u8_t *)ptr - b->buf;
	order       = 0;
	block_len   = MBUDDY_MIN_LEN;
	block_order = MBUDDY_MIN_ORDER;

	i = (1 << (b->max_order - order)) - 1 + (offset >> block_order);
	while (!mbuddy_test_bit(b->bitmap, i)) {
		order++;
		block_order++;
		block_len <<= 1;
		i = (1 << (b->max_order - order)) - 1 + (offset >> block_order);
	}
	mbuddy_clear_bit(b->bitmap, i);

	offset_buddy = offset ^ block_len;
	i_buddy      = (1 << (b->max_order - order)) - 1 + (offset_buddy >> block_order);
	if (mbuddy_test_bit(b->bitmap, i_buddy))
			return;

	block_len <<= 1;
	order++;
	block_order++;
	while (order < b->max_order) {
		i = (1 << (b->max_order - order)) - 1 + (offset >> block_order);
		mbuddy_clear_bit(b->bitmap, i);

		offset_buddy = offset ^ block_len;
		i_buddy      = (1 << (b->max_order - order)) - 1 + (offset_buddy >> block_order);
		if (mbuddy_test_bit(b->bitmap, i_buddy))
				return;

		block_len <<= 1;
		if ((offset & (block_len-1)) != 0)
			offset -= (block_len >> 1);

		order++;
		block_order++;
	}

	if (order == b->max_order) {
		if (!mbuddy_test_bit(b->bitmap, 1) && !mbuddy_test_bit(b->bitmap, 2))
			mbuddy_clear_bit(b->bitmap, 0);
	}
}

__nostd_api void
mbuddy_reset(MBuddy *b) {
	__nostd_usz_t bitmap_len;

	bitmap_len = mbuddy_get_bitmap_len(b->len);
	__nostd_memzero(b->bitmap, bitmap_len);
}


__nostd_api __nostd_usz_t
mbuddy_get_bitmap_len(__nostd_usz_t len)
{
	__nostd_assert(__nostd_is_pow2(len), "arg is not a power of 2!")
	return ((len >> MBUDDY_MIN_ORDER) << 1) >> 3;
}

inline void
mbuddy_set_bit(u8 *bitmap, __nostd_usz_t i)
{
	bitmap[i >> 3] |=  (1 << (i & 7));
}

inline void
mbuddy_clear_bit(u8 *bitmap, __nostd_usz_t i)
{
	bitmap[i >> 3] &= ~(1 << (i & 7));
}

inline int
mbuddy_test_bit(u8 *bitmap, __nostd_usz_t i)
{
	return (bitmap[i >> 3] >> (i & 7)) & 1;
}


#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MBUDDY__)

