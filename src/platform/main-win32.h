#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "nostdp.h"

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);

local void
atomic_test();

local void
queue_test(MArena *sysmem);

local void
test_fn1(void *args, ThreadAppJobData *tdata)
{
	unref(args);
	unref(tdata);
	log_pass(s8("This is Function #1"));
}

local void
test_fn2(void *args, ThreadAppJobData *tdata)
{
	unref(args);
	unref(tdata);
	log_odd(s8("This is Function #2"));
}

local void
test_fn3(void *args, ThreadAppJobData *tdata)
{
	unref(args);
	unref(tdata);
	log_warn(s8("This is Function #3"));
}

#endif // SRC_ENTRY_WIN32_H

