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

/////////////
// Structs //
/////////////
typedef struct {
	b8     mb;
	fb8    cb;
	fb8    fb;
	fb8    nb;
	u32    flags_level;
	u32    flags_format;
} Logger;
global Logger _g_logger = {0};

typedef struct {
	usz msec;
	usz usec;
	usz nsec;

	u8 hour;
	u8 min;
	u8 sec;
	u8 dst;

	u8 month;
	u8 day;
	u16 year;
} LogTime;

local void
mmm_sleep(void *args);

local i32
fb8_write(fb8 *b);

local b8
get_cpu_vendor(u8 *buffer, usz len);

local void
linux_init_logger(MArena *arena,
                  u32   flags_level,
                  u32   format_level);

local void
linux_log(u32 level,
          const char *msg,
          const char *file,
          usz linenum,
          const char *fnname);

local void
linux_get_time(LogTime *lt);

local void
linux_get_date(LogTime *lt);

local void
linux_get_datetime(LogTime *lt);

local int
is_leap_year(int year);

local void
linux_setup_crash_handler();

local void
linux_crash_handler(int signo,
                    siginfo_t *info,
                    void* context);

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);

#endif // SRC_MAIN_LINUX_H

