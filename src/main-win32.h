#ifndef SRC_ENTRY_WIN32_H
#define SRC_ENTRY_WIN32_H

#include "asm/cpuid.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/////////////
// Helpers //
/////////////
local i32
s8_print(s8 s);

local i32
fb8_write(fb8 *b);

#endif // SRC_ENTRY_WIN32_H

