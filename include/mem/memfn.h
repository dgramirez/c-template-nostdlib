#ifndef INCLUDE_MEM_MEMFN_H
#define INCLUDE_MEM_MEMFN_H

#include "define.h"
#include "platform.h"

// NOTE [Optimization]: Despite trying to utilize IPC, There are potential
//                      optimizations that can be employed. This note does
//                      not include SSE and multi-threading as they will be
//                      employed either as a separate dynamic library (CPU
//                      Features like SSE, AVX, etc.) or when it seems
//                      appropriate on a project basis (Jobs via Threading)

#if EXE_ARCH == 32
local inline usz
rotr(usz x,
     int k)
{
	return (x >> k) | (x << (32 - k));
}
#else
local inline usz
rotr(usz x,
     int k)
{
	return (x >> k) | (x << (64 - k));
}
#endif

////////////
// MEMCPY //
////////////

local void
memcpya_u8(void *dst,
           void *src,
           usz size)
{
	MemUnion d;
	MemUnion s;
	u8  size_count;

	d.pv = dst;
	s.pv = src;
	size_count = sizeof(u8) << 2;
	while (size > size_count) {
		d.pu32[0] = s.pu32[0];

		d.pu32++;
		s.pu32++;
		size -= size_count;
	}

	while (size--)
		d.pu8[size] = s.pu8[size];
}

local void
memcpyu_u8(void *dst,
           void *src,
           usz size)
{
	MemUnion d;
	MemUnion s;
	u8  pad;

	if (size < sizeof(usz)) {
		memcpya_u8(dst, src, size);
		return;
	}

	d.pv  = dst;
	s.pv  = src;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	src = (char*)src + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = s.pu8[pad];

	memcpya_u8(dst, src, size);
}

local void
memcpya_usz(void *dst,
            void *src,
            usz size)
{
	MemUnion d;
	MemUnion s;
	usz  size_count;

	d.pv = dst;
	s.pv = src;
	size_count = sizeof(usz) << 2;
	while (size > size_count) {
		d.pusz[0] = s.pusz[0];
		d.pusz[1] = s.pusz[1];
		d.pusz[2] = s.pusz[2];
		d.pusz[3] = s.pusz[3];

		d.pusz  += 4;
		s.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(usz);
	size -= size_count * sizeof(usz);
	while (size_count--) {
		d.pusz[0] = s.pusz[0];
		d.pusz += 1;
		s.pusz += 1;
	}

	while (size--)
		d.pu8[size] = s.pu8[size];
}

local void
memcpyu_usz(void *dst,
            void *src,
            usz size)
{
	MemUnion d;
	MemUnion s;
	u8  pad;

	if (size < sizeof(usz)) {
		memcpya_u8(dst, src, size);
		return;
	}

	d.pv  = dst;
	s.pv  = src;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	src = (char*)src + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = s.pu8[pad];

	memcpya_usz(dst, src, size);
}

/////////////
// MEMMOVE //
/////////////
local void
memmoveu_u8(void *dst,
            void *src,
            usz size)
{
	MemUnion d;
	MemUnion s;
	usz dst_offset;
	u8  size_count;

	if (dst == src)
		return;

	d.pv = dst;
	s.pv = src;
	if ((s.pv < d.pv) && ((s.pu8 + size) > d.pu8)) {
		// BACKWARDS
		d.pu8 = (u8 *)dst + size;
		s.pu8 = (u8 *)src + size;
		dst_offset = (usz)d.pu8 & (sizeof(usz) - 1);
		size -= dst_offset;
		while(dst_offset--) {
			d.pu8--;
			s.pu8--;

			d.pu8[0] = s.pu8[0];
		}

		size_count = sizeof(u8) << 2;
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
		dst_offset = align_pad(dst, sizeof(usz));
		size -= dst_offset;
		while (dst_offset--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}

		size_count = sizeof(u8) << 2;
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

local void
memmoveu_usz(void *dst,
             void *src,
             usz size)
{
	MemUnion d;
	MemUnion s;
	usz dst_offset;
	u8  size_count;

	if (dst == src)
		return;

	d.pv = dst;
	s.pv = src;
	if ((s.pv < d.pv) && ((s.pu8 + size) > d.pu8)) {
		// BACKWARDS
		d.pu8 = (u8 *)dst + size;
		s.pu8 = (u8 *)src + size;
		dst_offset = (usz)d.pu8 & (sizeof(usz) - 1);
		size -= dst_offset;
		while(dst_offset--) {
			d.pu8--;
			s.pu8--;

			d.pu8[0] = s.pu8[0];
		}

		size_count = sizeof(usz) << 2;
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
		dst_offset = align_pad(dst, sizeof(usz));
		size -= dst_offset;
		while (dst_offset--) {
			d.pu8[0] = s.pu8[0];

			d.pu8++;
			s.pu8++;
		}

		size_count = sizeof(usz) << 2;
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

////////////
// MEMSET //
////////////
local void
memseta_u8(void *dst,
           usz val,
           usz size)
{
	MemUnion d;
	u8  size_count;

	d.pv = dst;
	size_count = sizeof(u8) << 2;
	while (size > size_count) {
		d.pu32[0] = (u32)val;
		val = rotr(val, 32);

		d.pu32++;
		size -= size_count;
	}

	while (size--) {
		d.pu8[size] = (u8)val;
		val = rotr(val, 8);
	}
}

local void
memsetu_u8(void *dst,
           usz val,
           usz size)
{
	MemUnion d;
	u8  pad;

	if (size < sizeof(usz)) {
		memseta_u8(dst, val, size);
		return;
	}

	d.pv  = dst;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--) {
		d.pu8[pad] = (u8)val;
		val = rotr(val, 8);
	}

	memseta_u8(dst, val, size);
}

local int
memcmpa_u8(void *left,
           void *right,
           usz size)
{
	MemUnion l;
	MemUnion r;
	u8  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(u8) << 2;
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

local int
memcmpu_u8(void *left,
           void *right,
           usz size)
{
	MemUnion l;
	MemUnion r;
	u8  pad;

	if (size < sizeof(usz))
		return memcmpa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = align_pad(left, sizeof(usz));
	left  = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;
	}

	return memcmpa_u8(left, right, size);
}

local void
memzeroa_u8(void *dst,
            usz size)
{
	MemUnion d;
	u8  size_count;

	d.pv = dst;
	size_count = sizeof(u8) << 2;
	while (size > size_count) {
		d.pu32[0] = 0;
		d.pu32++;
		size -= size_count;
	}

	while (size--)
		d.pu8[size] = 0;
}

local void
memzerou_u8(void *dst,
            usz size)
{
	MemUnion d;
	u8  pad;

	if (size < sizeof(usz)) {
		memzeroa_u8(dst, size);
		return;
	}

	d.pv  = dst;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = 0;

	memzeroa_u8(dst, size);
}

local int
memeqa_u8(void *left,
          void *right,
          usz size)
{
	MemUnion l;
	MemUnion r;
	u8  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(u8) << 2;
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

local int
memequ_u8(void *left,
          void *right,
          usz size)
{
	MemUnion l;
	MemUnion r;
	u8  pad;

	if (size < sizeof(usz))
		return memeqa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = align_pad(left, sizeof(usz));
	left  = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return 0;
	}

	return memeqa_u8(left, right, size);
}

////////////////////////
// MEMCPY size_t-Bits //
////////////////////////
local void
memseta_usz(void *dst,
            usz val,
            usz size)
{
	MemUnion d;
	usz  size_count;

	d.pv = dst;
	size_count = sizeof(usz) << 2;
	while (size > size_count) {
		d.pusz[0] = val;
		d.pusz[1] = val;
		d.pusz[2] = val;
		d.pusz[3] = val;

		d.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(usz);
	size -= size_count * sizeof(usz);
	while (size_count--) {
		d.pusz[0] = val;
		d.pusz += 1;
	}

	while (size--) {
		d.pu8[size] = (u8)val;
		val = rotr(val, 8);
	}
}

local void
memsetu_usz(void *dst,
            usz val,
            usz size)
{
	MemUnion d;
	u8  pad;

	if (size < sizeof(usz)) {
		memseta_u8(dst, val, size);
		return;
	}

	d.pv  = dst;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--) {
		d.pu8[pad] = (u8)val;
		val = rotr(val, 8);
	}

	memseta_usz(dst, val, size);
}

local int
memcmpa_usz(void *left,
            void *right,
            usz size)
{
	MemUnion l;
	MemUnion r;
	usz  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(usz) << 2;
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

	size_count = size / sizeof(usz);
	size -= size_count * sizeof(usz);
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

local int
memcmpu_usz(void *left,
            void *right,
            usz size)
{
	MemUnion l;
	MemUnion r;
	u8  pad;

	if (size < sizeof(usz))
		return memcmpa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = align_pad(left, sizeof(usz));
	left = (char*)left + pad;
	right = (char*)right + pad;
	size -= pad;

	while (pad--) {
		if (l.pu8[0] ^ r.pu8[0])
			return (l.pu8[0] < r.pu8[0]) ? -1 : 1;
	}

	return memcmpa_usz(left, right, size);
}

local void
memzeroa_usz(void *dst,
             usz size)
{
	MemUnion d;
	usz  size_count;

	d.pv = dst;
	size_count = sizeof(usz) << 2;
	while (size > size_count) {
		d.pusz[0] = 0;
		d.pusz[1] = 0;
		d.pusz[2] = 0;
		d.pusz[3] = 0;

		d.pusz  += 4;
		size -= size_count;
	}

	size_count = size / sizeof(usz);
	size -= size_count * sizeof(usz);
	while (size_count--) {
		d.pusz[0] = 0;
		d.pusz += 1;
	}

	while (size--)
		d.pu8[size] = 0;
}

local void
memzerou_usz(void *dst,
             usz size)
{
	MemUnion d;
	u8  pad;

	if (size < sizeof(usz)) {
		memzeroa_u8(dst, size);
		return;
	}

	d.pv  = dst;

	pad   = align_pad(dst, sizeof(usz));
	dst = (char*)dst + pad;
	size -= pad;

	while (pad--)
		d.pu8[pad] = 0;

	memzeroa_usz(dst, size);
}

local int
memeqa_usz(void *left,
           void *right,
           usz size)
{
	MemUnion l;
	MemUnion r;
	usz  size_count;

	l.pv = left;
	r.pv = right;
	size_count = sizeof(usz) << 2;
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

	size_count = size / sizeof(usz);
	size -= size_count * sizeof(usz);
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

local int
memequ_usz(void *left,
           void *right,
           usz size)
{
	MemUnion l;
	MemUnion r;
	u8  pad;

	if (size < sizeof(usz))
		return memeqa_u8(left, right, size);

	l.pv  = left;
	r.pv  = right;

	pad   = align_pad(left, sizeof(usz));
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
typedef void (*pfn_memcpy)(void *, void *, usz);
local pfn_memcpy memcpya = memcpya_usz;
local pfn_memcpy memcpyu = memcpyu_usz;

typedef void (*pfn_memmove)(void *, void *, usz);
local pfn_memmove memmoveu = memmoveu_usz;

typedef void (*pfn_memset)(void *, usz, usz);
local pfn_memset memseta = memseta_usz;
local pfn_memset memsetu = memsetu_usz;

typedef int (*pfn_memcmp)(void *, void *, usz);
local pfn_memcmp memcmpa = memcmpa_usz;
local pfn_memcmp memcmpu = memcmpu_usz;

typedef int (*pfn_memeq)(void *, void *, usz);
local pfn_memeq memeqa = memeqa_usz;
local pfn_memeq memequ = memequ_usz;

typedef void (*pfn_memzero)(void *, usz);
local pfn_memzero memzeroa = memzeroa_usz;
local pfn_memzero memzerou = memzerou_usz;

#ifdef __linux__
	void *memset(void *s, int c, size_t n)
	{
		// NOTE [32-Bit]: Bad for 32-bit support
		usz csz;

		csz= c | ((usz)c << 32);
		memsetu(s, csz, n);
		return s;
	}
#endif

#endif // INCLUDE_MEM_MEMFN_H

