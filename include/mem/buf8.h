#ifndef INCLUDE_BUF8_H
#define INCLUDE_BUF8_H

#include "define.h"
#include "platform.h"
#include "memfn.h"

typedef struct {
	u8   *data;
	usz   len;
	void *fd;
	usz   cap;
	b32   err;
} buf8;

typedef isz (*pfn_os_write)(buf8 *b);
local isz stub_os_write(buf8 *b) {unref(b); return -1;}
pfn_os_write os_write = stub_os_write;

local void
buf8_flush(buf8 *b)
{
	b->err |= ((isz)b->fd < 0);
	if (!b->err && b->len) {
		os_write(b);
		b->len = 0;
	}
}

local void
buf8_append(buf8 *b,
            u8 *data,
            usz size)
{
	usz size_write;
	while (size) {
		size_write = imin(size, b->cap - b->len);
		memcpyu(b->data + b->len, data, size_write);

		size -= size_write;
		b->len += size_write;
		if (size)
			buf8_flush(b);
	}
}

local void
buf8_append_byte(buf8 *b,
                 u8 c)
{
	if (b->len == b->cap)
		buf8_flush(b);

	b->data[b->len++] = c;
}

local void
buf8_append_lf(buf8 *b)
{
	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
		buf8_append_byte(b, '\r'); // CR
	#endif
	buf8_append_byte(b, '\n'); // LF
}

local void
buf8_append_crlf(buf8 *b)
{
	buf8_append_byte(b, '\r'); // CR
	buf8_append_byte(b, '\n'); // LF
}

local void
buf8_append_isz(buf8 *b,
                isz x)
{
	u8 tmp[32] = {0};
	u8 *end = tmp + sizeof(tmp);
	u8 *beg = end;

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

	buf8_append(b, beg, end - beg);
}

local void
buf8_append_usz(buf8 *b,
                usz x)
{
	u8 tmp[32] = {0};
	u8 *end;
	u8 *beg;

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = '0' + (x % 10);
	} while (x /= 10);

	buf8_append(b, beg, end - beg);
}

local void
buf8_append_hex(buf8 *b,
                usz x)
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

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = hex_table[x & 0xF];
	} while(x >>= 4);

	*--beg = 'x';
	*--beg = '0';

	buf8_append(b, beg, end - beg);
}

local void
buf8_append_hex_cap(buf8 *b,
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

	end = tmp + sizeof(tmp);
	beg = end;
	do {
		*--beg = hex_table[x & 0xF];
	} while(x >>= 4);

	*--beg = 'x';
	*--beg = '0';

	buf8_append(b, beg, end - beg);
}

local void
buf8_append_f64(buf8 *b,
                double d) // [Insert Joke Here.]
{
	usz integral;
	usz fractional; 
	usz dbits;
	i32 precision;
	usz i;

	if (d < 0) {
		buf8_append_byte(b, '-');
		d = -d;
	}

	d += 0.0000005;
	dbits = *(usz*)(&d);

	if (isinf(dbits)) {
		buf8_append(b, (u8*)"inf", 3);
		return;
	}

	if (isnan(dbits)) {
		buf8_append(b, (u8*)"nan", 3);
		return;
	}

	precision = 1000000;
	integral = (usz)d;
	fractional = (usz)((d - integral) * precision);

	buf8_append_usz(b, integral);
	buf8_append_byte(b, '.');
	for (i = precision/10; i > 1; i /= 10) {
		if (i > fractional)
			buf8_append_byte(b, '0');
	}

	buf8_append_usz(b, fractional);
}

#endif // INCLUDE_BUF8_H

