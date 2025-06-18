/**************************************************************************
 * Name:   Memory Functions (memfn.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_MEMFN__
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

// NOTE [Optimization]: Despite trying to utilize IPC, There are potential
//                      optimizations that can be employed. This note does
//                      not include SSE and multi-threading as they will be
//                      employed either as a separate dynamic library (CPU
//                      Features like SSE, AVX, etc.) or when it seems
//                      appropriate on a project basis (Jobs via Threading)

#ifndef __STANDALONE_MEMORY_FUNCTIONS_H
#define __STANDALONE_MEMORY_FUNCTIONS_H

////////////////////////
// Standalone Defines //
////////////////////////
#ifndef __nostd_u8_t
	#define __nostd_u8_t unsigned char
#endif

#ifndef __nostd_u16_t
	#define __nostd_u16_t unsigned short
#endif

#ifndef __nostd_u32_t
	#define __nostd_u32_t unsigned int
#endif

#ifndef __nostd_usz_t
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define __nostd_usz_t unsigned int
	#else
		#define __nostd_usz_t unsigned long long
	#endif
#endif

#ifndef __nostd_memunion
	typedef union {
		void *pv;
		__nostd_u8_t   *pu8;
		__nostd_u16_t  *pu16;
		__nostd_u32_t  *pu32;
		__nostd_usz_t  *pusz;
	} MemUnion;

	#define __nostd_memunion MemUnion
#endif

#ifndef __nostd_api
	#define __nostd_api
#endif

#ifndef __nostd_extern 
	#define __nostd_extern extern
#endif

#ifndef __nostd_align_pad
	#ifndef __nostd_align_over
		#define __nostd_align_over(p, a) ((__nostd_usz_t)(p) & ((a) - 1))
	#endif
	#define __nostd_align_pad(p, a) \
		(((a) - __nostd_align_over(p, a)) & ((a) - 1))
#endif

#ifndef __nostd_rotr
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		__nostd_api inline usz
		__nostd_fn_rotr(__nostd_usz_t x,
		     int k)
		{
			return (x >> k) | (x << (32 - k));
		}

		#define __nostd_rotr(x, k) __nostd_fn_rotr(x, k)
	#else
		#define __nostd_rotr(x, k) ((x >> k) | (x << (64 - k)));
	#endif
#endif

/////////////////
//  Prototypes //
/////////////////
typedef void (*pfn_memcpy)(void *, void *, __nostd_usz_t);
__nostd_extern pfn_memcpy memcpya;
__nostd_extern pfn_memcpy memcpyu;

typedef void (*pfn_memmove)(void *, void *, __nostd_usz_t);
__nostd_extern pfn_memmove memmoveu;

typedef void (*pfn_memset)(void *, __nostd_usz_t, __nostd_usz_t);
__nostd_extern pfn_memset memseta;
__nostd_extern pfn_memset memsetu;

typedef int (*pfn_memcmp)(void *, void *, __nostd_usz_t);
__nostd_extern pfn_memcmp memcmpa;
__nostd_extern pfn_memcmp memcmpu;

typedef int (*pfn_memeq)(void *, void *, __nostd_usz_t);
__nostd_extern pfn_memeq memeqa;
__nostd_extern pfn_memeq memequ;

typedef void (*pfn_memzero)(void *, __nostd_usz_t);
__nostd_extern pfn_memzero memzeroa;
__nostd_extern pfn_memzero memzerou;

// u8
__nostd_api void
memcpya_u8(void          *dst,
           void          *src,
           __nostd_usz_t  size);

__nostd_api void
memcpyu_u8(void          *dst,
           void          *src,
           __nostd_usz_t  size);

__nostd_api void
memmoveu_u8(void          *dst,
            void          *src,
            __nostd_usz_t  size);

__nostd_api void
memseta_u8(void          *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size);

__nostd_api void
memsetu_u8(void          *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size);

__nostd_api int
memcmpa_u8(void          *left,
           void          *right,
           __nostd_usz_t  size);

__nostd_api int
memcmpu_u8(void          *left,
           void          *right,
           __nostd_usz_t  size);

__nostd_api void
memzeroa_u8(void          *dst,
            __nostd_usz_t  size);

__nostd_api void
memzerou_u8(void          *dst,
            __nostd_usz_t  size);

__nostd_api int
memeqa_u8(void          *left,
          void          *right,
          __nostd_usz_t  size);

__nostd_api int
memequ_u8(void          *left,
          void          *right,
          __nostd_usz_t  size);

// usz
__nostd_api void
memcpya_usz(void          *dst,
            void          *src,
            __nostd_usz_t  size);

__nostd_api void
memcpyu_usz(void          *dst,
            void          *src,
            __nostd_usz_t  size);

__nostd_api void
memmoveu_usz(void          *dst,
             void          *src,
             __nostd_usz_t  size);

__nostd_api void
memseta_usz(void         *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size);

__nostd_api void
memsetu_usz(void         *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size);

__nostd_api int
memcmpa_usz(void         *left,
            void         *right,
           __nostd_usz_t  size);

__nostd_api int
memcmpu_usz(void         *left,
            void         *right,
           __nostd_usz_t  size);

__nostd_api void
memzeroa_usz(void         *dst,
            __nostd_usz_t  size);

__nostd_api void
memzerou_usz(void         *dst,
            __nostd_usz_t  size);

__nostd_api int
memeqa_usz(void         *left,
           void         *right,
          __nostd_usz_t  size);

__nostd_api int
memequ_usz(void         *left,
           void         *right,
          __nostd_usz_t  size);

//////////////////////////?///////////////////////
// Function Definitions - Memory Functions (u8) //
//////////////////////////////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MEMFN__)
__nostd_api void
memcpya_u8(void          *dst,
           void          *src,
           __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_u8_t  size_count;

	d.pv = dst;
	s.pv = src;
	size_count = sizeof(__nostd_u8_t) << 2;
	while (size > size_count) {
		d.pu32[0] = s.pu32[0];

		d.pu32++;
		s.pu32++;
		size -= size_count;
	}

	while (size--)
		d.pu8[size] = s.pu8[size];
}

__nostd_api void
memcpyu_u8(void          *dst,
           void          *src,
           __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memcpya_u8(dst, src, size);
		return;
	}

	d.pv  = dst;
	s.pv  = src;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	src = (char*)src + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = s.pu8[pad];

	memcpya_u8(dst, src, size);
}

__nostd_api void
memmoveu_u8(void          *dst,
            void          *src,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_usz_t dst_offset;
	__nostd_u8_t  size_count;

	if (dst == src)
		return;

	d.pv = dst;
	s.pv = src;
	if ((s.pv < d.pv) && ((s.pu8 + size) > d.pu8)) {
		// BACKWARDS
		d.pu8 = (__nostd_u8_t *)dst + size;
		s.pu8 = (__nostd_u8_t *)src + size;
		dst_offset = (__nostd_usz_t)d.pu8 & (sizeof(__nostd_usz_t) - 1);
		size -= dst_offset;
		while(dst_offset--) {
			d.pu8--;
			s.pu8--;

			d.pu8[0] = s.pu8[0];
		}

		size_count = sizeof(__nostd_u8_t) << 2;
		while (size > size_count) {
			d.pu32--;
			s.pu32--;
			size -= size_count;

			d.pu32[0] = s.pu32[0];
		}

		d.pv = dst;
		s.pv = src;
		while (size--)
			d.pu8[size] = s.pu8[size];
	}
	else {
		// FORWARDS
		d.pv = dst;
		s.pv = src;
		dst_offset = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
		size -= dst_offset;
		while (dst_offset--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}

		size_count = sizeof(__nostd_u8_t) << 2;
		while (size > size_count) {
			d.pu32[0] = s.pu32[0];

			d.pu32++;
			s.pu32++;
			size -= size_count;
		}

		while (size--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}
	}
}

__nostd_api void
memseta_u8(void          *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  size_count;

	d.pv = dst;
	size_count = sizeof(__nostd_u8_t) << 2;
	while (size > size_count) {
		d.pu32[0] = (__nostd_u32_t)val;
		val = __nostd_rotr(val, 32);

		d.pu32++;
		size -= size_count;
	}

	while (size--) {
		d.pu8[size] = (__nostd_u8_t)val;
		val = __nostd_rotr(val, 8);
	}
}

__nostd_api void
memsetu_u8(void          *dst,
           __nostd_usz_t  val,
           __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memseta_u8(dst, val, size);
		return;
	}

	d.pv  = dst;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--) {
		d.pu8[pad] = (__nostd_u8_t)val;
		val = __nostd_rotr(val, 8);
	}

	memseta_u8(dst, val, size);
}

__nostd_api int
memcmpa_u8(void          *left,
           void          *right,
           __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(__nostd_u8_t) << 2;
	while (size > size_count) {
		if (l.pu32[0] ^ r.pu32[0])
			return (l.pu32[0] < r.pu32[0]) ? -1 : 1;

		l.pu32++;
		r.pu32++;
		size -= size_count;
	}

	while (size--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;

		l.pu8++;
		r.pu8++;
	}

	return 0;
}

__nostd_api int
memcmpu_u8(void          *left,
           void          *right,
           __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t))
		return memcmpa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = __nostd_align_pad(left, sizeof(__nostd_usz_t));
	left  = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;
	}

	return memcmpa_u8(left, right, size);
}

__nostd_api void
memzeroa_u8(void          *dst,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  size_count;

	d.pv = dst;
	size_count = sizeof(__nostd_u8_t) << 2;
	while (size > size_count) {
		d.pu32[0] = 0;
		d.pu32++;
		size -= size_count;
	}

	while (size--)
		d.pu8[size] = 0;
}

__nostd_api void
memzerou_u8(void          *dst,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memzeroa_u8(dst, size);
		return;
	}

	d.pv  = dst;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = 0;

	memzeroa_u8(dst, size);
}

__nostd_api int
memeqa_u8(void          *left,
          void          *right,
          __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(__nostd_u8_t) << 2;
	while (size > size_count) {
		if (l.pu32[0] ^ r.pu32[0])
			return 0;

		l.pu32++;
		r.pu32++;
		size -= size_count;
	}

	while (size--) {
		if (l.pu8[0] ^ r.pu8[0])
			return 0;

		l.pu8++;
		r.pu8++;
	}

	return 1;
}

__nostd_api int
memequ_u8(void          *left,
          void          *right,
          __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t))
		return memeqa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = __nostd_align_pad(left, sizeof(__nostd_usz_t));
	left  = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return 0;
	}

	return memeqa_u8(left, right, size);
}

//////////////////////////?///////////////////////////
// Function Definitions - Memory Functions (size_t) //
//////////////////////////////////////////////////////
__nostd_api void
memcpya_usz(void          *dst,
            void          *src,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_usz_t  size_count;

	d.pv = dst;
	s.pv = src;
	size_count = sizeof(__nostd_usz_t) << 2;
	while (size > size_count) {
		d.pusz[0] = s.pusz[0];
		d.pusz[1] = s.pusz[1];
		d.pusz[2] = s.pusz[2];
		d.pusz[3] = s.pusz[3];

		d.pusz  += 4;
		s.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(__nostd_usz_t);
	size -= size_count * sizeof(__nostd_usz_t);
	while (size_count--) {
		d.pusz[0] = s.pusz[0];
		d.pusz += 1;
		s.pusz += 1;
	}

	while (size--)
		d.pu8[size] = s.pu8[size];
}

__nostd_api void
memcpyu_usz(void          *dst,
            void          *src,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memcpya_u8(dst, src, size);
		return;
	}

	d.pv  = dst;
	s.pv  = src;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	src = (char*)src + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = s.pu8[pad];

	memcpya_usz(dst, src, size);
}

__nostd_api void
memmoveu_usz(void          *dst,
             void          *src,
             __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_memunion s;
	__nostd_usz_t dst_offset;
	__nostd_u8_t  size_count;

	if (dst == src)
		return;

	d.pv = dst;
	s.pv = src;
	if ((s.pv < d.pv) && ((s.pu8 + size) > d.pu8)) {
		// BACKWARDS
		d.pu8 = (__nostd_u8_t *)dst + size;
		s.pu8 = (__nostd_u8_t *)src + size;
		dst_offset = (__nostd_usz_t)d.pu8 & (sizeof(__nostd_usz_t) - 1);
		size -= dst_offset;
		while(dst_offset--) {
			d.pu8--;
			s.pu8--;

			d.pu8[0] = s.pu8[0];
		}

		size_count = sizeof(__nostd_usz_t) << 2;
		while (size > size_count) {
			d.pusz -= 4;
			s.pusz -= 4;
			size -= size_count;

			d.pusz[3] = s.pusz[3];
			d.pusz[2] = s.pusz[2];
			d.pusz[1] = s.pusz[1];
			d.pusz[0] = s.pusz[0];
		}

		while (size--) {
			d.pu8--;
			s.pu8--;

			d.pu8[0] = s.pu8[0];
		}
	}
	else {
		// FORWARDS
		d.pv = dst;
		s.pv = src;
		dst_offset = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
		size -= dst_offset;
		while (dst_offset--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}

		size_count = sizeof(__nostd_usz_t) << 2;
		while (size > size_count) {
			d.pusz[0] = s.pusz[0];
			d.pusz[1] = s.pusz[1];
			d.pusz[2] = s.pusz[2];
			d.pusz[3] = s.pusz[3];

			d.pusz += 4;
			s.pusz += 4;
			size -= size_count;
		}

		while (size--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}
	}
}

__nostd_api void
memseta_usz(void          *dst,
            __nostd_usz_t  val,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_usz_t  size_count;

	d.pv = dst;
	size_count = sizeof(__nostd_usz_t) << 2;
	while (size > size_count) {
		d.pusz[0] = val;
		d.pusz[1] = val;
		d.pusz[2] = val;
		d.pusz[3] = val;

		d.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(__nostd_usz_t);
	size -= size_count * sizeof(__nostd_usz_t);
	while (size_count--) {
		d.pusz[0] = val;
		d.pusz += 1;
	}

	while (size--) {
		d.pu8[size] = (__nostd_u8_t)val;
		val = __nostd_rotr(val, 8);
	}
}

__nostd_api void
memsetu_usz(void          *dst,
            __nostd_usz_t  val,
            __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memseta_u8(dst, val, size);
		return;
	}

	d.pv  = dst;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--) {
		d.pu8[pad] = (__nostd_u8_t)val;
		val = __nostd_rotr(val, 8);
	}

	memseta_usz(dst, val, size);
}

__nostd_api int
memcmpa_usz(void          *left,
            void          *right,
            __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_usz_t  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(__nostd_usz_t) << 2;
	while (size > size_count) {
		if (l.pusz[0] ^ r.pusz[0])
			return (l.pusz[0] < r.pusz[0]) ? -1 : 1;
		if (l.pusz[1] ^ r.pusz[1])
			return (l.pusz[1] < r.pusz[1]) ? -1 : 1;
		if (l.pusz[2] ^ r.pusz[2])
			return (l.pusz[2] < r.pusz[2]) ? -1 : 1;
		if (l.pusz[3] ^ r.pusz[3])
			return (l.pusz[3] < r.pusz[3]) ? -1 : 1;

		l.pusz  += 4;
		r.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(__nostd_usz_t);
	size -= size_count * sizeof(__nostd_usz_t);
	while (size_count--) {
		if (l.pusz[0] ^ r.pusz[0])
			return (l.pusz[0] < r.pusz[0]) ? -1 : 1;

		l.pusz += 1;
		r.pusz += 1;
	}

	while (size--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;

		l.pu8++;
		r.pu8++;
	}

	return 0;
}

__nostd_api int
memcmpu_usz(void          *left,
            void          *right,
            __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t))
		return memcmpa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = __nostd_align_pad(left, sizeof(__nostd_usz_t));
	left = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;
	}

	return memcmpa_usz(left, right, size);
}

__nostd_api void
memzeroa_usz(void          *dst,
             __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_usz_t  size_count;

	d.pv = dst;
	size_count = sizeof(__nostd_usz_t) << 2;
	while (size > size_count) {
		d.pusz[0] = 0;
		d.pusz[1] = 0;
		d.pusz[2] = 0;
		d.pusz[3] = 0;

		d.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(__nostd_usz_t);
	size -= size_count * sizeof(__nostd_usz_t);
	while (size_count--) {
		d.pusz[0] = 0;
		d.pusz += 1;
	}

	while (size--)
		d.pu8[size] = 0;
}

__nostd_api void
memzerou_usz(void          *dst,
             __nostd_usz_t  size)
{
	__nostd_memunion d;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t)) {
		memzeroa_u8(dst, size);
		return;
	}

	d.pv  = dst;

	pad   = __nostd_align_pad(dst, sizeof(__nostd_usz_t));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = 0;

	memzeroa_usz(dst, size);
}

__nostd_api int
memeqa_usz(void          *left,
           void          *right,
           __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_usz_t  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(__nostd_usz_t) << 2;
	while (size > size_count) {
		if (l.pusz[0] ^ r.pusz[0])
			return 0;
		if (l.pusz[1] ^ r.pusz[1])
			return 0;
		if (l.pusz[2] ^ r.pusz[2])
			return 0;
		if (l.pusz[3] ^ r.pusz[3])
			return 0;

		l.pusz  += 4;
		r.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(__nostd_usz_t);
	size -= size_count * sizeof(__nostd_usz_t);
	while (size_count--) {
		if (l.pusz[0] ^ r.pusz[0])
			return 0;

		l.pusz += 1;
		r.pusz += 1;
	}

	while (size--) {
		if (l.pu8[0] ^ r.pu8[0])
			return 0;

		l.pu8++;
		r.pu8++;
	}

	return 1;
}

__nostd_api int
memequ_usz(void          *left,
           void          *right,
           __nostd_usz_t  size)
{
	__nostd_memunion l;
	__nostd_memunion r;
	__nostd_u8_t  pad;

	if (size < sizeof(__nostd_usz_t))
		return memeqa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = __nostd_align_pad(left, sizeof(__nostd_usz_t));
	left = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return 0;
	}

	return memeqa_usz(left, right, size);
}

///////////////////////
// FUNCTION POINTERS //
///////////////////////
__nostd_api pfn_memcpy memcpya = memcpya_usz;
__nostd_api pfn_memcpy memcpyu = memcpyu_usz;

__nostd_api pfn_memmove memmoveu = memmoveu_usz;

__nostd_api pfn_memset memseta = memseta_usz;
__nostd_api pfn_memset memsetu = memsetu_usz;

__nostd_api pfn_memcmp memcmpa = memcmpa_usz;
__nostd_api pfn_memcmp memcmpu = memcmpu_usz;

__nostd_api pfn_memeq memeqa = memeqa_usz;
__nostd_api pfn_memeq memequ = memequ_usz;

__nostd_api pfn_memzero memzeroa = memzeroa_usz;
__nostd_api pfn_memzero memzerou = memzerou_usz;

#ifdef __linux__
	void *memset(void *s,
	             int c,
	             size_t n)
	{
		// NOTE [32-Bit]: Bad for 32-bit support
		__nostd_usz_t csz;

		csz= c | ((__nostd_usz_t)c << 32);
		memsetu(s, csz, n);
		return s;
	}
#endif

#if defined(_MSC_VER)
	void *memset(void *dst,
	             int c,
	             size_t count);
	#pragma function(memset)

	void *memcpy(void *dst,
	             const void *src,
	             size_t count);
	#pragma function(memcpy)

	void *memset(void *dst,
	             int c,
	             size_t count)
	{
		#if EXE_ARCH == 32
			__nostd_usz_t _c = (__nostd_usz_t)c;
		#else
			__nostd_usz_t _c = (__nostd_usz_t)c | ((__nostd_usz_t)c << 32);
		#endif
		memsetu(dst, _c, count);
		return dst;
	}
	
	void *memcpy(void *dst,
	             const void *src,
	             size_t count)
	{
		memcpyu(dst, (void*)src, count);
		return dst;
	}
#endif 

#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MEMFN__)

#endif // __STANDALONE_MEMORY_FUNCTIONS_H

