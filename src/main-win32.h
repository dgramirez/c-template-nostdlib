#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "platform/win32.h"


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

