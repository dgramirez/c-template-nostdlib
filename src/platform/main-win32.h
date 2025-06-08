#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "nostdp.h"
#include "app/include/cross_app.h"

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);

local void
win32_setup_platform_data(PlatformData *pdata,
                          MBuddy       *buddy);

#endif // SRC_ENTRY_WIN32_H

