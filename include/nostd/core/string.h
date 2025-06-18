/**************************************************************************
 * Name:   String Structures & Functions (string.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_STRING__
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

#ifndef __STANDALONE_STRING_H__
#define __STANDALONE_STRING_H__

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

#ifndef __nostd_imin
	#ifndef __nostd_minmax_bits
		#define __nostd_minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
	#endif
	#define __nostd_imin(x, y) ((y) ^ __nostd_minmax_bits((x), (y)))
#endif

#ifndef __nostd_isinf
	#define __nostd_isinf(x) (((x) & 0x7FF0000000000000) == 0x7FF0000000000000)
#endif

#ifndef __nostd_isnan
	#define __nostd_isnan(x) (((x) & 0x7FFFFFFFFFFFFFFF)  > 0x7FF0000000000000)
#endif

#ifndef __nostd_memcpy
	#define __nostd_memcpy(dst, src, size) memcpy(dst, src, size)
#endif

#ifndef __nostd_memmove
	#define __nostd_memmove(dst, src, size) memmove(dst, src, size)
#endif

#ifndef __nostd_memcmp
	#define __nostd_memcmp(m1, m2, size) memcmp(m1, m2, size)
#endif

#ifndef __nostd_memeq
	#define __nostd_memeq(m1, m2, size) (memcmp(m1, m2, size) == 0)
#endif

#ifndef __nostd_memzero
	#define __nostd_memzero(dst, size) memset(dst, 0, size)
#endif

#ifndef __nostd_extern 
	#define __nostd_extern extern
#endif

#ifndef __s8_countof
	#define __s8_countof(x) ((__nostd_usz_t)(sizeof(x) / sizeof(*(x))))
#endif

#ifndef s8_lengthof
	#define s8_lengthof(x) (__s8_countof(x) - 1)
#endif

#ifndef s8
	#define s8(x)   ((s8){(__nostd_u8_t*)x, s8_lengthof(x)})
#endif

#ifndef sb8
	#define sb8(x) ((sb8){(__nostd_u8_t *)x, s8_lengthof(x), s8_lengthof(x)})
#endif

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef struct {
	__nostd_u8_t *data;
	__nostd_usz_t len;
} s8, b8;

typedef struct {
	union {
		b8 b;
		struct { __nostd_u8_t *data; __nostd_usz_t len; };
	};

	__nostd_usz_t cap;
} sb8, mb8;

typedef struct {
	union {
		mb8 mb;
		struct {
			union {
				b8 b;
				struct { __nostd_u8_t *data; __nostd_usz_t len; };
			};
			__nostd_usz_t cap;
		};
	};

	void *fd;
	__nostd_usz_t err;
} fb8;

typedef int (*PFN_os_write)(fb8 *);
int stub_os_write(fb8 *b);
__nostd_extern PFN_os_write os_write;

// cstr
__nostd_api __nostd_usz_t
c_strlen(const char *start);

__nostd_api int
c_streq(const char *s1,
        const char *s2);

// Buffer (b8) & String Buffer (s8)
#define s8_chr  b8_chr
#define s8_rchr b8_rchr
#define s8_eq   b8_eq

__nostd_api __nostd_u8_t *
b8_chr(b8            s,
       char          c,
       __nostd_usz_t max);

__nostd_api __nostd_u8_t *
b8_rchr(b8            s,
        char          c,
        __nostd_usz_t max);

__nostd_api int
b8_eq(b8            l,
      b8            r,
      __nostd_usz_t size);

// Memory Buffer (mb8) & String Memory Buffer (sb8)
#define sb8_init           mb8_init
#define sb8_cpy            mb8_cpy
#define sb8_cat            mb8_cat
#define sb8_move           mb8_move
#define sb8_cmp            mb8_cmp
#define sb8_eq             mb8_eq

#define sb8_append_byte    mb8_append_byte
#define sb8_append_lf      mb8_append_lf
#define sb8_append_crlf    mb8_append_crlf
#define sb8_append_isz     mb8_append_isz
#define sb8_append_usz     mb8_append_usz
#define sb8_append_hex     mb8_append_hex 
#define sb8_append_hex_cap mb8_append_hex_cap
#define sb8_append_f64     mb8_append_f64

__nostd_api void
mb8_init(mb8           *mb,
         void          *b,
         __nostd_usz_t  len);

__nostd_api void
mb8_cpy(mb8 *dst,
        b8   src);

__nostd_api void
mb8_cat(mb8 *dst,
        b8   src);

__nostd_api void
mb8_move(mb8 *dst,
         b8   src);

__nostd_api int
mb8_cmp(mb8 *l,
        mb8 *r);

__nostd_api int
mb8_eq(mb8 *l,
       mb8 *r);

__nostd_api void
mb8_append_byte(mb8           *dst,
                __nostd_u8_t   c);

__nostd_api void
mb8_append_lf(mb8 *mb);

__nostd_api void
mb8_append_crlf(mb8 *mb);

__nostd_api void
mb8_append_isz(mb8           *mb,
               __nostd_isz_t  x);

__nostd_api void
mb8_append_usz(mb8           *mb,
               __nostd_usz_t  x);

__nostd_api void
mb8_append_hex(mb8           *mb,
               __nostd_usz_t  x);

__nostd_api void
mb8_append_hex_cap(mb8          *mb,
                   __nostd_usz_t x);

__nostd_api void
mb8_append_f64(mb8    *mb,
               double  d); // [Insert Joke Here.]

// File Buffer (fb8)
__nostd_api void
fb8_flush(fb8 *fb);

__nostd_api void
fb8_append(fb8 *fb,
           b8   b);

__nostd_api void
fb8_append_cstr(fb8           *fb,
                const char    *cstr,
                __nostd_usz_t  len);

__nostd_api void
fb8_append_byte(fb8          *dst,
                __nostd_u8_t  c);

__nostd_api void
fb8_append_lf(fb8 *fb);

__nostd_api void
fb8_append_lf(fb8 *fb);

__nostd_api void
fb8_append_isz(fb8           *fb,
               __nostd_isz_t  x);

__nostd_api void
fb8_append_usz(fb8           *fb,
               __nostd_usz_t  x);

__nostd_api void
fb8_append_hex(fb8           *fb,
               __nostd_usz_t  x);

__nostd_api void
fb8_append_hex_cap(fb8           *fb,
                   __nostd_usz_t  x);

__nostd_api void
fb8_append_f64(fb8    *fb,
               double  x);

//////////////////////////?///////////
// Function Definitions - C Strings //
//////////////////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_STRING__)

__nostd_api __nostd_usz_t
c_strlen(const char *start)
{
	char *end = (char *const )start;
	while (*end++);

	return end - start - 1;
}

__nostd_api int
c_streq(const char *s1,
        const char *s2)
{
	while(*s1 && *s2) {
		if (*s1 != *s2)
			return 0;

		s1++;
		s2++;
	}
	return *s1 == *s2;
}

/////////////
// s8 / b8 //
/////////////
__nostd_api __nostd_u8_t *
s8_chr(s8            s,
       char          c,
       __nostd_usz_t max)
{
	max = __nostd_imin(s.len, max);
	while(max--) {
		if (*s.data == c)
			return s.data;
		s.data++;
	}

	return 0;
}

__nostd_api __nostd_u8_t *
s8_rchr(s8            s,
        char          c,
        __nostd_usz_t max)
{
	max = __nostd_imin(s.len, max);
	while(max--) {
		if (*s.data == c)
			return s.data;
		s.data--;
	}

	return 0;
}

__nostd_api int
s8_eq(s8 l,
      s8 r,
      __nostd_usz_t size)
{
	if (size == 0) {
		if (l.len != r.len)
			return 0;
		return __nostd_memeq(l.data, r.data, l.len);
	}

	if (l.len < size || r.len < size)
		return 0;

	return __nostd_memeq(l.data, r.data, size);
}

///////////////
// sb8 / mb8 //
///////////////
__nostd_api void
mb8_init(mb8           *mb,
         void          *b,
         __nostd_usz_t  len)
{
	mb->data = (__nostd_u8_t *)b;
	mb->cap  = len;
	mb->len  = 0;

	__nostd_memzero(mb->data, mb->cap);
}

__nostd_api void
mb8_cpy(mb8 *dst,
        b8   src)
{
	__nostd_usz_t max_len;

	max_len = __nostd_imin(src.len, dst->cap);
	__nostd_memcpy(dst->data, src.data, max_len);
	dst->len = max_len;
}

__nostd_api void
mb8_cat(mb8 *dst,
        b8   src)
{
	__nostd_usz_t max_len;

	max_len = __nostd_imin(src.len, dst->cap - dst->len);
	__nostd_memcpy(dst->data + dst->len, src.data, max_len);
	dst->len += max_len;
}

__nostd_api void
mb8_move(mb8 *dst,
         b8   src)
{
	__nostd_usz_t max_len;

	max_len = __nostd_imin(src.len, dst->cap);
	__nostd_memmove(dst->data, src.data, max_len);
	dst->len = max_len;
}

__nostd_api int
mb8_cmp(mb8 *l,
        mb8 *r)
{
	__nostd_usz_t max_len;
	int rval;

	max_len = __nostd_imin(l->len, r->len);
	rval = __nostd_memcmp(l->data, r->data, max_len);

	if (rval == 0) {
		if (l->len < r->len)
			return -1;
		if (l->len > r->len)
			return 1;
	}

	return rval;
}

__nostd_api int
mb8_eq(mb8 *l,
       mb8 *r)
{
	if (l->len != r->len)
		return 0;
	return __nostd_memeq(l->data, r->data, l->len);
}

__nostd_api void
mb8_append_byte(mb8          *dst,
                __nostd_u8_t  c)
{
	if (dst->len == dst->cap)
		return;

	dst->data[dst->len++] = c;
}

__nostd_api void
mb8_append_lf(mb8 *mb)
{
	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
		mb8_append_byte(mb, '\r'); // CR
	#endif
	mb8_append_byte(mb, '\n'); // LF
}

__nostd_api void
mb8_append_crlf(mb8 *mb)
{
	mb8_append_byte(mb, '\r'); // CR
	mb8_append_byte(mb, '\n'); // LF
}

__nostd_api void
mb8_append_isz(mb8 *mb,
               __nostd_isz_t  x)
{
	__nostd_u8_t tmp[32] = {0};
	__nostd_u8_t *end = tmp + sizeof(tmp);
	__nostd_u8_t *beg = end;
	b8 b;

	if (x < 0) {
		tmp[0] = x < 0;
		x = -x;
	}

	do {
		*--beg = '0' + (x % 10);
	} while (x /= 10);

	if (tmp[0]) {
		*--beg = '-';
	}

	b.data = beg;
	b.len = end - beg;
	mb8_cat(mb, b);
}

__nostd_api void
mb8_append_usz(mb8           *mb,
               __nostd_usz_t  x)
{
	__nostd_u8_t tmp[32] = {0};
	__nostd_u8_t *end;
	__nostd_u8_t *beg;
	b8 b;

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = '0' + (x % 10);
	} while (x /= 10);

	b.data = beg;
	b.len = end - beg;
	mb8_cat(mb, b);
}

__nostd_api void
mb8_append_hex(mb8           *mb,
               __nostd_usz_t  x)
{
	static const __nostd_u8_t hex_table[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	__nostd_u8_t tmp[32] = {0};
	__nostd_u8_t *end;
	__nostd_u8_t *beg;
	b8 b;

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = hex_table[x & 0xF];
	} while(x >>= 4);

	*--beg = 'x';
	*--beg = '0';

	b.data = beg;
	b.len = end - beg;
	mb8_cat(mb, b);
}

__nostd_api void
mb8_append_hex_cap(mb8           *mb,
                   __nostd_usz_t  x)
{
	static const __nostd_u8_t hex_table[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};

	__nostd_u8_t tmp[32] = {0};
	__nostd_u8_t *end;
	__nostd_u8_t *beg;
	b8 b;

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = hex_table[x & 0xF];
	} while(x >>= 4);

	*--beg = 'x';
	*--beg = '0';

	b.data = beg;
	b.len = end - beg;
	mb8_cat(mb, b);
}

__nostd_api void
mb8_append_f64(mb8    *mb,
               double  d) // [Insert Joke Here.]
{
	__nostd_usz_t integral;
	__nostd_usz_t fractional; 
	__nostd_usz_t dbits;
	int precision;
	__nostd_usz_t i;

	if (d < 0) {
		mb8_append_byte(mb, '-');
		d = -d;
	}

	d += 0.0000005;
	dbits = *(__nostd_usz_t*)(&d);

	if (__nostd_isinf(dbits)) {
		mb8_cpy(mb, s8("inf"));
		return;
	}

	if (__nostd_isnan(dbits)) {
		mb8_cpy(mb, s8("nan"));
		return;
	}

	precision = 1000000;
	integral = (__nostd_usz_t)d;
	fractional = (__nostd_usz_t)((d - integral) * precision);

	mb8_append_isz(mb, integral);
	mb8_append_byte(mb, '.');
	for (i = precision/10; i > 1; i /= 10) {
		if (i > fractional)
			mb8_append_byte(mb, '0');
	}

	mb8_append_usz(mb, fractional);
}

/////////
// fb8 //
/////////
__nostd_api void
fb8_flush(fb8 *fb)
{
	fb->err |= ((__nostd_isz_t)fb->fd < 0);
	if (!fb->err && fb->len) {
		os_write(fb);
		fb->len = 0;
	}
}

__nostd_api void
fb8_append(fb8 *fb,
           b8   b)
{
	__nostd_usz_t len_write;
	while (b.len) {
		len_write = __nostd_imin(b.len, fb->cap - fb->len);
		__nostd_memcpy(fb->data + fb->len, b.data, len_write);

		b.len   -= len_write;
		b.data  += len_write;
		fb->len += len_write;
		if (b.len)
			fb8_flush(fb);
	}
}

__nostd_api void
fb8_append_cstr(fb8           *fb,
                const char    *cstr,
                __nostd_usz_t  len)
{
	b8 b;

	if (!len)
		len = 0xFFFFFFFF;

	b.data = (__nostd_u8_t *)cstr;
	b.len  = __nostd_imin(len, c_strlen(cstr));
	fb8_append(fb, b);
}

__nostd_api void
fb8_append_byte(fb8          *dst,
                __nostd_u8_t  c)
{
	if (dst->len == dst->cap)
		fb8_flush(dst);

	dst->data[dst->len++] = c;
}

__nostd_api void
fb8_append_lf(fb8 *fb)
{
	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
		fb8_append_byte(fb, '\r'); // CR
	#endif
	fb8_append_byte(fb, '\n'); // LF
}

__nostd_api void
fb8_append_crlf(fb8 *fb)
{
	fb8_append_byte(fb, '\r'); // CR
	fb8_append_byte(fb, '\n'); // LF
}

__nostd_api void
fb8_append_isz(fb8           *fb,
               __nostd_isz_t  x)
{
	__nostd_u8_t *temp[32];
	mb8 val;

	val.data = (__nostd_u8_t *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_append_isz(&val, x);

	fb8_append(fb, val.b);
}

__nostd_api void
fb8_append_usz(fb8           *fb,
               __nostd_usz_t  x)
{
	__nostd_u8_t *temp[32];
	mb8 val;

	val.data = (__nostd_u8_t *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_append_usz(&val, x);

	fb8_append(fb, val.b);
}

__nostd_api void
fb8_append_hex(fb8           *fb,
               __nostd_usz_t  x)
{
	__nostd_u8_t *temp[32];
	mb8 val;

	val.data = (__nostd_u8_t *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_append_hex(&val, x);

	fb8_append(fb, val.b);
}

__nostd_api void
fb8_append_hex_cap(fb8           *fb,
                   __nostd_usz_t  x)
{
	__nostd_u8_t *temp[32];
	mb8 val;

	val.data = (__nostd_u8_t *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_append_hex_cap(&val, x);

	fb8_append(fb, val.b);
}

__nostd_api void
fb8_append_f64(fb8    *fb,
               double  x)
{
	__nostd_u8_t *temp[32];
	mb8 val;

	val.data = (__nostd_u8_t *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_append_f64(&val, x);

	fb8_append(fb, val.b);
}

int stub_os_write(fb8 *b) { (void)sizeof(b); return -1; }
__nostd_api PFN_os_write os_write = stub_os_write;

#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_STRING__)

#endif // __STANDALONE_STRING_H__

