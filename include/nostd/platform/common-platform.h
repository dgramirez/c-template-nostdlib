#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_PLATFORM_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_PLATFORM_H

///////////////////
// Core & Common //
///////////////////
#include "../core/core.h"
#include "../common/common.h"

//////////////////////////////////
// Common Defines, Types & Data //
//////////////////////////////////
#include "common/define.h"

////////////////////////////////////////
// Common Assembly & Helper Functions //
////////////////////////////////////////
#include "common/atomics.h"
#include "common/cpuid.h"
#include "common/lock_mcs.h"
#include "common/globals.h"

////////////////////////////////
// Remaining Common Functions //
////////////////////////////////
#include "common/appjob_queue.h"
#include "common/appjob_threadpool.h"

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_PLATFORM_H

