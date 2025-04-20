#ifndef SRC_MAIN_LINUX_H
#define SRC_MAIN_LINUX_H

#include "nostdp.h"

local void
mmm_donuts(void *arg, ThreadAppJobData *thread);

local void
mmm_cake(void *arg, ThreadAppJobData *thread);

local void
mmm_pies(void *arg, ThreadAppJobData *thread);

local void
print_fn_addresses(MArena *a);

local void *
setup_shared_lib_app();

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, unref(pd);return -1;, PlatformData *pd);
declfn(void, app_close,);

local void
test_mfreelist(MArena *a);
#endif // SRC_MAIN_LINUX_H

