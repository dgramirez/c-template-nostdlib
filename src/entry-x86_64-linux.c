#include "entry-x86_64-linux.h"

int
_entry(int    argc,
       char **argv)
{
	b8  arg = {0};
	b8  mem = {0};
	int rval;

	mem = setup_memory_profile(ENTRY_MEMORY_PROFILE_MINIMUM);
	if (!mem.len || !mem.data)
		return -1;

	arg.len  = argc - 1;
	arg.data = (u8 *)argv[0];
	do {
		argc--;
		arg.len += c_strlen(argv[argc]);
	} while (argc > 0);

	// TODO: CPU Library Initialization
	os_write        = fb8_write;
	logsz           = linux_logc;
	logs8           = linux_log;
	thread_wait     = futex_wait;
	thread_wake_one = futex_wake_one;
	thread_wake_all = futex_wake_all;
	thread_create   = linux_thread_create;
	thread_exit     = sys_exit;
	mlock_init      = mlock_init_mcslock;
	mlock_acquire   = mlock_acquire_mcslock;
	mlock_release   = mlock_release_mcslock;
	tp_post         = tp_post_generic;
	tp_quit         = tp_quit_generic;
	tp_wait         = tp_wait_generic;
	get_cpu_freq    = tsc_get_cpu_freq;
	get_os_freq     = linux_get_freq;
	get_os_counter  = linux_get_counter;

	rval = cmain(arg, mem);
	return rval;
}

local b8
setup_memory_profile(int profile) {
	b8 ret = {0};

	switch(profile) {
		case ENTRY_MEMORY_PROFILE_DETECT: {
			ret.len  = MB(64);
			ret.len += buddy_sizeof(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_MINIMUM: {
			ret.len  = MB(64);
			ret.len += buddy_sizeof(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_LOW: {
			ret.len  = MB(256);
			ret.len += buddy_sizeof(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_MEDIUM: {
			ret.len  = GB(1);
			ret.len += buddy_sizeof(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_HIGH: {
			ret.len  = GB(4);
			ret.len += buddy_sizeof(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_CUSTOM: {
			ret.len  = 0;
			ret.len += buddy_sizeof(ret.len);
		} break;

		default: {
			ret.len = MB(64);
			ret.len += buddy_sizeof(ret.len);
		} break;
	}

	ret.data = mmap_anon(ret.len);
	return ret;
}

