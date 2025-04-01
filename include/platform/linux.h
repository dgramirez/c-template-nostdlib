#ifndef INCLUDE_LINUX_H
#define INCLUDE_LINUX_H

////////////
// Common //
////////////
#include "platform/common.h"

/////////////////////////////
// Core Linux Types & Data //
/////////////////////////////
#include "platform/linux/define.h"
#include "platform/linux/type_kernel_libc.h"
#include "platform/linux/type.h"
#include "platform/linux/global_var.h"

////////////////////////////////////////////
// Core Linux Assembly & Helper Functions //
////////////////////////////////////////////
#include "platform/linux/syscall.h"
#include "platform/linux/atomic.h"
#include "platform/linux/thread.h"
#include "platform/linux/helper_fn.h"

////////////////////////////////
// Additional Linux Functions //
////////////////////////////////
#include "platform/linux/mcs_lock.h"
#include "platform/linux/threadpool_queue.h"
#include "platform/linux/threadpool.h"
#include "platform/linux/datetime.h"
#include "platform/linux/log.h"

#ifndef USING_LIBC
	#include "platform/linux/dl.h"
#else
	#include <dlfcn.h>
#endif

#endif // INCLUDE_LINUX_H

