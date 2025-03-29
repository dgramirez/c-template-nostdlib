#ifndef INCLUDE_LINUX_H
#define INCLUDE_LINUX_H

#include "platform/common.h"

#include "platform/linux/define.h"
#include "platform/linux/type.h"
#include "platform/linux/global_var.h"

#include "platform/linux/syscall.h"
#include "platform/linux/atomic.h"
#include "platform/linux/datetime.h"
#include "platform/linux/helper_fn.h"
#include "platform/linux/log.h"
#include "cpu/thread-linux.h"

#ifndef USING_LIBC
	#include "platform/linux/dl.h"
#else
	#include <dlfcn.h>
#endif

#endif // INCLUDE_LINUX_H

