#ifndef INCLUDE_PLATFORM_WIN32_H
#define INCLUDE_PLATFORM_WIN32_H

///////////////////
// Core & Common //
///////////////////
#include "../core/core.h"
#include "../common/common.h"

/////////////////////////////
// Core Win32 Types & Data //
/////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "win32/type.h"
#include "win32/global_var.h"
#include "win32/helper_fn.h"

/////////////////////
// Common-Platform //
/////////////////////
#include "common-platform.h"

///////////////////////////////
// Remaining Win32 Functions //
///////////////////////////////
#include "win32/log.h"

#endif // INCLUDE_PLATFORM_WIN32_H

