#ifndef INCLUDE_NOSTD_PLATFORM_H
#define INCLUDE_NOSTD_PLATFORM_H

#ifdef _WIN32
	#include "platform/win32.h"
#endif

#ifdef __linux__
	#include "platform/linux.h"
#endif

#endif // INCLUDE_NOSTD_PLATFORM_H

