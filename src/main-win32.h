#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "asm/cpuid.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"
#include "par.h"
#include "log.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

/////////////
// Helpers //
/////////////
local i32
fb8_write(fb8 *b);

local b8
get_cpu_vendor(u8 *buffer, usz len);

local void
win32_init_logger(MArena *arena,
                  u32   flags_level,
                  u32   format_level);

local void
win32_log(u32 level,
          const char *msg,
          const char *file,
          usz linenum,
          const char *fnname);

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);


#endif // SRC_ENTRY_WIN32_H

