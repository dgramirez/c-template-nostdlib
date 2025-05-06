#include "entry-x86_64-win32.h"

void
_start() {
	b8  arg = {0};
	b8  mem = {0};
	int rval;

	SetUnhandledExceptionFilter(win32_crash_handler);

	mem = Win32SetupMemoryProfile(ENTRY_MEMORY_PROFILE_MINIMUM);
	if (!mem.len || !mem.data) {
		Win32WriteToConsole("Failed to allocate memory. . .\n", 31);
		ExitProcess((u32)-1);
	}

	arg.data = (u8*)GetCommandLineA();
	arg.len  = c_strlen((char *)arg.data);

	os_write        = Win32WriteFile;
	logsz           = win32_logc;
	logs8           = win32_log;
	thread_wait     = Win32AddressWait;
	thread_wake_one = Win32WakeByAddressSingle;
	thread_wake_all = Win32WakeByAddressAll;
	thread_create   = Win32ThreadCreate;
	mlock_init      = mlock_init_mcslock;
	mlock_acquire   = mlock_acquire_mcslock;
	mlock_release   = mlock_release_mcslock;
	tp_post         = tp_post_generic;
	tp_quit         = tp_quit_generic;
	tp_wait         = tp_wait_generic;

	rval = Win32Main(arg, mem);
	ExitProcess(rval);
}

local b8
Win32SetupMemoryProfile(int profile)
{
	b8 ret = {0};

	switch(profile) {
		case ENTRY_MEMORY_PROFILE_DETECT: {
			ret.len  = MB(64);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_MINIMUM: {
			ret.len  = MB(64);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_LOW: {
			ret.len  = MB(256);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_MEDIUM: {
			ret.len  = GB(1);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_HIGH: {
			ret.len  = GB(4);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		case ENTRY_MEMORY_PROFILE_CUSTOM: {
			ret.len  = 0;
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;

		default: {
			ret.len = MB(64);
			ret.len += mbuddy_get_bitmap_len(ret.len);
		} break;
	}

	ret.data = VirtualAlloc(0,
	                        ret.len,
	                        MEM_COMMIT | MEM_RESERVE,
	                        PAGE_READWRITE);
	return ret;
}

local void
Win32ParseCmdLine(int    *argc,
                  s8     *argv,
                  int   argmax,
                  char *buffer,
                  usz   buflen)
{
	s8 cmdline;
	u8 *delimiter;
	int i;

	cmdline.data = (u8*)GetCommandLineA();
	cmdline.len  = c_strlen((char*)cmdline.data);
	delimiter = s8_chr(cmdline, ' ', cmdline.len);
	if (!delimiter) {
		*argc = 1;
		argv[0].data = cmdline.data;
		argv[0].len  = cmdline.len; 
		return;
	}

	memzerou(buffer, buflen);
	buflen = imin(cmdline.len, buflen-1);
	memcpyu(buffer, cmdline.data, buflen);

	i = 0;
	cmdline.data = (u8 *)buffer;
	cmdline.len  = c_strlen((char*)cmdline.data);
	while (argmax && cmdline.len) {
		if (cmdline.data[0] < ' ') {
			cmdline.data += 1;
			cmdline.len  -= 1;
			continue;
		}

		switch(cmdline.data[0]) {
			case ' ': {
				
			} break;

			case '\'': {
				cmdline.data += 1;
				cmdline.len  -= 1;

				delimiter = s8_chr(cmdline, '\'', cmdline.len);
				assert(delimiter, "Failed to parse arguments with: '\n");

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;

				*delimiter     = 0;
				*(delimiter+1) = 0;

				cmdline.len -= argv[i].len + 2;
				cmdline.data = delimiter + 2;
			} break;

			case '"': {
				cmdline.data += 1;
				cmdline.len  -= 1;

				delimiter = s8_chr(cmdline, '"', cmdline.len);
				assert(delimiter, "Failed to parse arguments with: \"\n");

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;

				*delimiter     = 0;
				*(delimiter+1) = 0;

				cmdline.len -= argv[i].len + 2;
				cmdline.data = delimiter + 2;
			} break;
			
			default: {
				delimiter = s8_chr(cmdline, ' ', cmdline.len);
				if (!delimiter)
					delimiter = cmdline.data + cmdline.len;

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;
				*delimiter = 0;

				cmdline.len -= argv[i].len + 1;
				cmdline.data = delimiter + 1;
			}
		}

		i++;
		argmax--;
	}

	*argc = i+1;
}

