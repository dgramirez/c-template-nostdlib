#ifndef INCLUDE_PLATFORM_LINUX_GLOBAL_VAR_H
#define INCLUDE_PLATFORM_LINUX_GLOBAL_VAR_H

extern char **environ;
static char **auxv = 0;
global Logger _g_logger = {0};

#ifndef USING_LIBC
	static AuxV _g_auxv;

	global MArena   _gma_tls_data;
	global MArena   _gma_dso_load;
	global MArena   _gma_dso_temp;
	global MPool    _gmp_dso_data;
	global MPool    _gmp_dso_node;
	global DsoNode *_gdso_tail;

	global usz _gdso_tls_modid  = 2;
	global usz _gdso_tls_offset = 0x0;
#endif

#endif // INCLUDE_PLATFORM_LINUX_GLOBAL_VAR_H

