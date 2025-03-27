#ifndef INCLUDE_PLATFORM_WIN32_LOG_H
#define INCLUDE_PLATFORM_WIN32_LOG_H

local void
win32_init_logger(MArena *arena,
                  u32   flags_level,
                  u32   flags_format)
{
	int bufcount;
	if (!(flags_format & 0xF))
		return;
	if (!(flags_level & 0xFF))
		return;

	_g_logger.flags_level  = flags_level;
	_g_logger.flags_format = flags_format;

	bufcount = 0;
	if (flags_format & LOG_FORMAT_CONSOLE)
		bufcount++;
	if (flags_format & LOG_FORMAT_FILE)
		bufcount++;
	if (flags_format & LOG_FORMAT_FILE_7Z)
		bufcount++;
	if (flags_format & LOG_FORMAT_NETWORK)
		bufcount++;

	if (bufcount == 3)
		bufcount = 4;
	if (flags_format & LOG_FORMAT_CONSOLE) {
		_g_logger.cb.fd   = GetStdHandle(STD_OUTPUT_HANDLE);
		_g_logger.cb.cap  = page_size;
		_g_logger.cb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_FILE) {
		_g_logger.fb.cap  = page_size;
		_g_logger.fb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_FILE_7Z) {
		_g_logger.fb.cap  = page_size;
		_g_logger.fb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_NETWORK) {
		_g_logger.nb.cap  = page_size;
		_g_logger.nb.data = marena_alloc(arena, page_size, page_size);
	}
}

local void
win32_log(u32 level,
          const char *msg,
          const char *file,
          usz linenum,
          const char *fnname)
{
	SYSTEMTIME lt = {0};
	FILETIME   ft = {0};
	ULONGLONG  time_100ns;
	CONSOLE_SCREEN_BUFFER_INFO csbi = {0};
	usz is_assert;
	if (!_g_logger.flags_level || !_g_logger.flags_format)
		return;

	// TODO: Write to File
	if (flag_has(_g_logger.flags_format, LOG_FORMAT_FILE) ||
        flag_has(_g_logger.flags_format, LOG_FORMAT_FILE_7Z))
	{
		
	}

	// TODO: Send over Network; Protocol Needed!
//	if (flag_has(_g_logger.flags_format, LOG_FORMAT_NETWORK)) {
//		
//	}

	if (!flag_has(_g_logger.flags_format, LOG_FORMAT_CONSOLE))
		return;

	is_assert = 0;
	if (flag_has(_g_logger.flags_level, LOG_LEVEL_ASSERT)) {
		is_assert = 1;
		flag_rem(_g_logger.flags_level, LOG_LEVEL_ASSERT);
	}

	GetConsoleScreenBufferInfo(_g_logger.cb.fd, &csbi);

	// "[Level]" + [TODO: Colors]
	switch(level) {
		case LOG_LEVEL_GOOFY: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_RED | FOREGROUND_BLUE |FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "==============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= DEBUG =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= INFO =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_RED | FOREGROUND_BLUE);
			fb8_append_cstr(&_g_logger.cb, "============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= WARNING =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_RED | FOREGROUND_INTENSITY);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= ERROR =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			SetConsoleTextAttribute(_g_logger.cb.fd, FOREGROUND_RED);
			fb8_append_cstr(&_g_logger.cb, "===============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===============\n", 0);
		} break;

		default: assert(0, "Must be a single level!");
	}

	fb8_flush(&_g_logger.cb);
	SetConsoleTextAttribute(_g_logger.cb.fd, csbi.wAttributes);

	//	"[10:00:00]"
	GetLocalTime(&lt);

	fb8_append_byte(&_g_logger.cb, '[');
	fb8_append_usz(&_g_logger.cb, lt.wHour);
	fb8_append_byte(&_g_logger.cb, ':');
	fb8_append_usz(&_g_logger.cb, lt.wMinute);
	fb8_append_byte(&_g_logger.cb, ':');
	fb8_append_usz(&_g_logger.cb, lt.wSecond);
	fb8_append_byte(&_g_logger.cb, '.');
	#if EXE_ARCH == 32
		fb8_append_usz(&_g_logger.cb, lt.wMilliseconds);
		unref(time_100ns);
		unref(ft);
	#else
		GetSystemTimeAsFileTime(&ft);
		time_100ns = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		time_100ns %= 10000000;
		fb8_append_usz(&_g_logger.cb, time_100ns);
	#endif
	fb8_append_byte(&_g_logger.cb, ']');

	// "file(line_num): fnname"
	if (is_assert) {
		fb8_append_byte(&_g_logger.cb, '\n');
		fb8_append_cstr(&_g_logger.cb, file, 0);
		fb8_append_byte(&_g_logger.cb, '(');
		fb8_append_usz( &_g_logger.cb, linenum);
		fb8_append_byte(&_g_logger.cb, ')');
		fb8_append_byte(&_g_logger.cb, ':');
		fb8_append_byte(&_g_logger.cb, ' ');
		fb8_append_cstr(&_g_logger.cb, fnname, 0);
	}

	fb8_append_lf(&_g_logger.cb);
	fb8_append_cstr(&_g_logger.cb, msg, 0);

	fb8_append_lf(&_g_logger.cb);
	fb8_append_lf(&_g_logger.cb);
	fb8_flush(&_g_logger.cb);
}

#ifdef _MSC_VER
#if EXE_ARCH == 32
declfn_type(EXCEPTION_DISPOSITION,
            Win32_SEH,
            struct _EXCEPTION_RECORD *,
            void *,
            struct _CONTEXT *,
            void *);

EXCEPTION_DISPOSITION
_except_handler3(struct _EXCEPTION_RECORD *ExceptionRecord,
                 void * EstablisherFrame,
                 struct _CONTEXT *ContextRecord,
                 void *DispatcherContext)
{
	static PFN_Win32_SEH fn;
	if (!fn) {
		HMODULE eww_c_runtime = LoadLibraryA("msvcrt.dll");
		fn = (PFN_Win32_SEH)GetProcAddress(eww_c_runtime,
		                                   "__C_specific_handler");
	}

	return fn(ExceptionRecord,
	          EstablisherFrame,
	          ContextRecord,
	          DispatcherContext);
}

UINT_PTR __security_cookie = 0xBB40E64E;
extern PVOID __safe_se_handler_table[];
extern BYTE __safe_se_handler_count;

typedef struct {
	DWORD      Size;
	DWORD      TimeDateStamp;
	WORD       MajorVersion;
	WORD       MinorVersion;
	DWORD      GlobalFlagsClear;
	DWORD      GlobalFlagsSet;
	DWORD      CriticalSectionDefaultTimeout;
	DWORD      DeCommitFreeBlockThreshold;
	DWORD      DeCommitTotalFreeThreshold;
	DWORD      LockPrefixTable;
	DWORD      MaximumAllocationSize;
	DWORD      VirtualMemoryThreshold;
	DWORD      ProcessHeapFlags;
	DWORD      ProcessAffinityMask;
	WORD       CSDVersion;
	WORD       Reserved1;
	DWORD      EditList;
	PUINT_PTR  SecurityCookie;
	PVOID     *SEHandlerTable;
	DWORD      SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY32_2;

const
IMAGE_LOAD_CONFIG_DIRECTORY32_2 _load_config_used = {
	sizeof(IMAGE_LOAD_CONFIG_DIRECTORY32_2),
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	&__security_cookie,
	__safe_se_handler_table,
	(DWORD)(DWORD_PTR)&__safe_se_handler_count
};
#else
declfn_type(EXCEPTION_DISPOSITION,
            Win32_SEH,
			struct _EXCEPTION_RECORD *,
            void *,
            struct _CONTEXT *,
            struct _DISPATCHER_CONTEXT *);

EXCEPTION_DISPOSITION
__C_specific_handler(struct _EXCEPTION_RECORD *ExceptionRecord,
					 void * EstablisherFrame,
					 struct _CONTEXT *ContextRecord,
					 struct _DISPATCHER_CONTEXT *DispatcherContext)
{
	static PFN_Win32_SEH fn;
	if (!fn) {
		HMODULE eww_c_runtime = LoadLibraryA("msvcrt.dll");
		fn = (PFN_Win32_SEH)GetProcAddress(eww_c_runtime,
		                                   "__C_specific_handler");
	}

	return fn(ExceptionRecord,
	          EstablisherFrame,
	          ContextRecord,
	          DispatcherContext);
}
#endif // EXE_ARCH == 32
#else
#endif // _MSC_VER

LONG WINAPI
win32_crash_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
	unref(ExceptionInfo);

	log_fatal("A Crash Has Occurred!\n");
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif // INCLUDE_PLATFORM_WIN32_LOG_H

