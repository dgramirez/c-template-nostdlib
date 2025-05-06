#ifndef SRC_ENTRY_X86_64_WIN32_H
#define SRC_ENTRY_X86_64_WIN32_H

#include "platform/main-win32.c"

local b8
Win32SetupMemoryProfile(int profile);

local void
Win32ParseCmdLine(int    *argc,
                  s8     *argv,
                  int   argmax,
                  char *buffer,
                  usz   buflen);

#endif // SRC_ENTRY_X86_64_WIN32_H

