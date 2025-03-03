#include "define.h"
#include "platform.h"
#include "mem/s8.h"

local     int g_ivar = 0;
external long g_lvar = 0;

int
increment_ivar()
{
	return ++g_ivar;
}

long
increment_lvar()
{
	return ++g_lvar;
}

local void
init(fb8 *b, pfn_os_write fn_os_write)
{
	s8 str;

	os_write = fn_os_write;

	str = s8("libtest: init");
	fb8_append(b, str);
	fb8_append_lf(b);
}

void
close(fb8 *b)
{
	s8 str;

	str = s8("libtest: close");
	fb8_append(b, str);
	fb8_append_lf(b);
}

