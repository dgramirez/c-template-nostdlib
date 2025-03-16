#ifndef SRC_MAIN_LINUX_H
#define SRC_MAIN_LINUX_H

#include "asm/linux-syscall.h"
#include "asm/cpuid.h"
#include "mem/s8.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"
#include "cpu/thread-linux.h"
#include "par.h"

#ifndef USING_LIBC
	#include "elf/dl.h"
#else
	#include <dlfcn.h>
#endif

local void
mmm_sleep(void *args);

local i32
fb8_write(fb8 *b);

local b8
get_cpu_vendor(u8 *buffer, usz len);

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);

#endif // SRC_MAIN_LINUX_H

