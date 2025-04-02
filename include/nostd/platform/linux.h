#ifndef INCLUDE_LINUX_H
#define INCLUDE_LINUX_H

////////////
// Common //
////////////
#include "../common/common.h"

/////////////////////////////
// Core Linux Types & Data //
/////////////////////////////
#include "linux/define.h"
#include "linux/type_kernel_libc.h"
#include "linux/type.h"
#include "linux/global_var.h"

////////////////////////////////////////////
// Core Linux Assembly & Helper Functions //
////////////////////////////////////////////
#include "linux/syscall.h"
#include "linux/atomic.h"
#include "linux/thread.h"
#include "linux/helper_fn.h"

////////////////////////////////
// Additional Linux Functions //
////////////////////////////////
#include "linux/mcs_lock.h"
#include "linux/threadpool_queue.h"
#include "linux/threadpool.h"
#include "linux/datetime.h"
#include "linux/log.h"

#ifndef USING_LIBC
	#include "linux/dl.h"
#else
	#include <dlfcn.h>
#endif

#endif // INCLUDE_LINUX_H

