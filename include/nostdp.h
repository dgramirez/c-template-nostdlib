#ifndef INCLUDE_NOSTD_PLATFORM_H
#define INCLUDE_NOSTD_PLATFORM_H

#ifdef _WIN32
	#include "nostd/platform/win32.h"
#endif

#ifdef __linux__
	#include "nostd/platform/linux.h"
#endif

#endif // INCLUDE_NOSTD_PLATFORM_H

