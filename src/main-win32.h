#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "asm/cpuid.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if EXE_ARCH == 32
	#define WIN32_USZ_VALUE 1234567890
	#define WIN32_ISZ_VALUE -1876543290
#else
	#define WIN32_USZ_VALUE 12345678901234567
	#define WIN32_ISZ_VALUE -9876543210987654
#endif

/////////////
// Helpers //
/////////////
local i32
s8_print(s8 s);

local i32
fb8_write(fb8 *b);

#endif // SRC_ENTRY_WIN32_H

