#ifndef SRC_MAIN_LINUX_H
#define SRC_MAIN_LINUX_H

#include "nostdp.h"


////////////////////
// Platform Setup //
////////////////////
local void *
linux_setup_shared_lib_app();

local void
linux_setup_memory(b8 mem,
                   MBuddy    *mbuddy);

local void
linux_setup_platform_data(PlatformData *pdata,
                          MBuddy *buddy);

local void
mmm_donuts(void *arg, TData *thread);

local void
mmm_cake(void *arg, TData *thread);

local void
mmm_pies(void *arg, TData *thread);

local void
print_fn_addresses(MArena *a);

local void
test_mbuddy(MArena *a);

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, unref(pd);return -1;, PlatformData *pd);
declfn(void, app_close,);

local void
test_mfreelist(MArena *a);

local void
test_mstack(MArena *a);

#endif // SRC_MAIN_LINUX_H

