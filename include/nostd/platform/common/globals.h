#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_GLOBALS_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_GLOBALS_H

global Logger    _glog           = {0};
global MCSLock   _glock_terminal = {0};
global CPUIDFull _cpuid          = {0};
global MFreelist _sysfl          = {0};
global usz       _cpu_freq       =  0;

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_GLOBALS_H

