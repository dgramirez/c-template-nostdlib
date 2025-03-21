#ifndef SRC_MAIN_LINUX_H
#define SRC_MAIN_LINUX_H

#include "platform/linux.h"

declfn(i32, app_init, unref(pd);return -1;, PlatformData *pd);
declfn(i32, app_update, return -1;);
declfn(void, app_close,);

#endif // SRC_MAIN_LINUX_H

