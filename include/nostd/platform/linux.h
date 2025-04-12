#ifndef INCLUDE_LINUX_H
#define INCLUDE_LINUX_H

///////////////////
// Core & Common //
///////////////////
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
#include "linux/thread.h"
#include "linux/helper_fn.h"

/////////////////////
// Common-Platform //
/////////////////////
#include "common-platform.h"

////////////////////////////////
// Additional Linux Functions //
////////////////////////////////
#include "linux/datetime.h"
#include "linux/log.h"

#ifndef USING_LIBC
	#include "linux/dl.h"
#else
	#include <dlfcn.h>
#endif

#endif // INCLUDE_LINUX_H

