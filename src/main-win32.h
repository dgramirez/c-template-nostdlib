#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "asm/cpuid.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"
#include "par.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/////////////
// Helpers //
/////////////
local i32
fb8_write(fb8 *b);

local b8
get_cpu_vendor(u8 *buffer, usz len);

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);


#endif // SRC_ENTRY_WIN32_H

