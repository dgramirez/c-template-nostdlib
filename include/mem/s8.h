#ifndef INCLUDE_S8_H
#define INCLUDE_S8_H
// NOTE: #include "type.h" for s8 types

//////////
// cstr //
//////////
local usz
c_strlen(const char *start)
{
	char *end = (char *const )start;
	while (*end++);

	return end - start - 1;
}

local int
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
local u8 *
s8_chr(s8 s,
       char c,
       usz max)
{
	max = imin(s.len, max);
	while(max--) {
		if (*s.data == c)
			return s.data;
		s.data++;
	}

	return 0;
}

local u8 *
s8_rchr(s8 s,
        char c,
        usz max)
{
	max = imin(s.len, max);
	while(max--) {
		if (*s.data == c)
			return s.data;
		s.data--;
	}

	return 0;
}

local int
s8_eq(s8 l,
      s8 r,
      usz size)
{
	if (size == 0) {
		if (l.len != r.len)
			return 0;
		return memequ(l.data, r.data, l.len);
	}

	if (l.len < size || r.len < size)
		return 0;

	return memequ(l.data, r.data, size);
}

///////////////
// sb8 / mb8 //
///////////////
local void
mb8_init(mb8 *mb,
         void *b,
         usz len)
{
	mb->data = (u8 *)b;
	mb->cap  = len;
	mb->len  = 0;
}

local void
mb8_cpy(mb8 *dst,
        b8   src)
{
	usz max_len;

	max_len = imin(src.len, dst->cap);
	memcpyu(dst->data, src.data, max_len);
	dst->len = max_len;
}

local void
mb8_cat(mb8 *dst,
        b8   src)
{
	usz max_len;

	max_len = imin(src.len, dst->cap - dst->len);
	memcpyu(dst->data + dst->len, src.data, max_len);
	dst->len += max_len;
}

local void
mb8_move(mb8 *dst,
         b8   src)
{
	usz max_len;

	max_len = imin(src.len, dst->cap);
	memmoveu(dst->data, src.data, max_len);
	dst->len = max_len;
}

local void
mb8_zero_init(mb8 *mb)
{
	memzerou(mb->data, mb->cap);
	mb->len = 0;
}

local int
mb8_cmp(mb8 *l,
        mb8 *r)
{
	usz max_len;
	int rval;

	max_len = imin(l->len, r->len);
	rval = memcmpu(l->data, r->data, max_len);

	if (rval == 0) {
		if (l->len < r->len)
			return -1;
		if (l->len > r->len)
			return 1;
	}

	return rval;
}

local int
mb8_eq(mb8 *l,
       mb8 *r)
{
	if (l->len != r->len)
		return 0;
	return memequ(l->data, r->data, l->len);
}

local void
mb8_sprint_byte(mb8 *dst,
                u8   c)
{
	if (dst->len == dst->cap)
		return;

	dst->data[dst->len++] = c;
}

local void
mb8_sprint_lf(mb8 *mb)
{
	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
		mb8_sprint_byte(mb, '\r'); // CR
	#endif
	mb8_sprint_byte(mb, '\n'); // LF
}

local void
mb8_sprint_crlf(mb8 *mb)
{
	mb8_sprint_byte(mb, '\r'); // CR
	mb8_sprint_byte(mb, '\n'); // LF
}

local void
mb8_sprint_isz(mb8 *mb,
               isz  x)
{
	u8 tmp[32] = {0};
	u8 *end = tmp + sizeof(tmp);
	u8 *beg = end;
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

local void
mb8_sprint_usz(mb8 *mb,
               usz  x)
{
	u8 tmp[32] = {0};
	u8 *end;
	u8 *beg;
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

local void
mb8_sprint_hex(mb8 *mb,
               usz  x)
{
	sticky const u8 hex_table[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	u8 tmp[32] = {0};
	u8 *end;
	u8 *beg;
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

local void
mb8_sprint_hex_cap(mb8 *mb,
                   usz x)
{
	sticky const u8 hex_table[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};

	u8 tmp[32] = {0};
	u8 *end;
	u8 *beg;
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

local void
mb8_sprint_f64(mb8    *mb,
               double  d) // [Insert Joke Here.]
{
	usz integral;
	usz fractional; 
	usz dbits;
	i32 precision;
	usz i;

	if (d < 0) {
		mb8_sprint_byte(mb, '-');
		d = -d;
	}

	d += 0.0000005;
	dbits = *(usz*)(&d);

	if (isinf(dbits)) {
		mb8_cpy(mb, s8("inf"));
		return;
	}

	if (isnan(dbits)) {
		mb8_cpy(mb, s8("nan"));
		return;
	}

	precision = 1000000;
	integral = (usz)d;
	fractional = (usz)((d - integral) * precision);

	mb8_sprint_isz(mb, integral);
	mb8_sprint_byte(mb, '.');
	for (i = precision/10; i > 1; i /= 10) {
		if (i > fractional)
			mb8_sprint_byte(mb, '0');
	}

	mb8_sprint_usz(mb, fractional);
}

/////////
// fb8 //
/////////
local void
fb8_flush(fb8 *fb)
{
	fb->err |= ((isz)fb->fd < 0);
	if (!fb->err && fb->len) {
		os_write(fb);
		fb->len = 0;
	}
}

local void
fb8_append(fb8 *fb,
           b8   b)
{
	usz len_write;
	while (b.len) {
		len_write = imin(b.len, fb->cap - fb->len);
		memcpyu(fb->data + fb->len, b.data, len_write);

		b.len   -= len_write;
		b.data  += len_write;
		fb->len += len_write;
		if (b.len)
			fb8_flush(fb);
	}
}

local void
fb8_append_cstr(fb8  *fb,
                const char *cstr,
                usz   len)
{
	b8 b;

	if (!len)
		len = 0xFFFFFFFF;

	b.data = (u8 *)cstr;
	b.len  = imin(len, c_strlen(cstr));
	fb8_append(fb, b);
}

local void
fb8_append_byte(fb8 *dst,
                u8   c)
{
	if (dst->len == dst->cap)
		fb8_flush(dst);

	dst->data[dst->len++] = c;
}

local void
fb8_append_lf(fb8 *fb)
{
	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
		fb8_append_byte(fb, '\r'); // CR
	#endif
	fb8_append_byte(fb, '\n'); // LF
}

local void
fb8_append_crlf(fb8 *fb)
{
	fb8_append_byte(fb, '\r'); // CR
	fb8_append_byte(fb, '\n'); // LF
}

local void
fb8_append_isz(fb8 *fb,
               isz  x)
{
	u8 *temp[32];
	mb8 val;

	val.data = (u8 *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_sprint_isz(&val, x);

	fb8_append(fb, val.b);
}

local void
fb8_append_usz(fb8 *fb,
               usz  x)
{
	u8 *temp[32];
	mb8 val;

	val.data = (u8 *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_sprint_usz(&val, x);

	fb8_append(fb, val.b);
}

local void
fb8_append_hex(fb8 *fb,
               usz  x)
{
	u8 *temp[32];
	mb8 val;

	val.data = (u8 *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_sprint_hex(&val, x);

	fb8_append(fb, val.b);
}

local void
fb8_append_hex_cap(fb8 *fb,
                   usz  x)
{
	u8 *temp[32];
	mb8 val;

	val.data = (u8 *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_sprint_hex_cap(&val, x);

	fb8_append(fb, val.b);
}

local void
fb8_append_f64(fb8    *fb,
               double  x)
{
	u8 *temp[32];
	mb8 val;

	val.data = (u8 *)temp;
	val.cap  = 32;
	val.len  = 0;
	mb8_sprint_f64(&val, x);

	fb8_append(fb, val.b);
}

#endif // INCLUDE_S8_H

