#ifndef SRC_MAIN_LINUX_H
#define SRC_MAIN_LINUX_H

#include "nostd_platform.h"

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, unref(pd);return -1;, PlatformData *pd);
declfn(void, app_close,);

#endif // SRC_MAIN_LINUX_H

