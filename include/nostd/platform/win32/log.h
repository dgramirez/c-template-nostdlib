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

	_glog.flags_level  = flags_level;
	_glog.flags_format = flags_format;

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
		_glog.cb.fd   = GetStdHandle(STD_OUTPUT_HANDLE);
		_glog.cb.cap  = page_size;
		_glog.cb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_FILE) {
		_glog.fb.cap  = page_size;
		_glog.fb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_FILE_7Z) {
		_glog.fb.cap  = page_size;
		_glog.fb.data = marena_alloc(arena, page_size, page_size);
	}
	if (flags_format & LOG_FORMAT_NETWORK) {
		_glog.nb.cap  = page_size;
		_glog.nb.data = marena_alloc(arena, page_size, page_size);
	}
}

local void
win32_log(u32 level,
          s8 msg,
          const char *file,
          usz linenum,
          const char *fnname)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi        = {0};
	SYSTEMTIME                 lt          = {0};
	FILETIME                   ft          = {0};
	ULONGLONG                  time_100ns;
	MCSLock                    me;
	usz                        is_assert;

	__mcs_lock(&_glock_terminal, &me);
	if (!_glog.flags_level || !_glog.flags_format)
		return;

	// TODO: Write to File
	if (flag_has(_glog.flags_format, LOG_FORMAT_FILE) ||
        flag_has(_glog.flags_format, LOG_FORMAT_FILE_7Z))
	{
		
	}

	// TODO: Send over Network; Protocol Needed!
//	if (flag_has(_glog.flags_format, LOG_FORMAT_NETWORK)) {
//		
//	}

	if (!flag_has(_glog.flags_format, LOG_FORMAT_CONSOLE))
		return;

	is_assert = 0;
	if (flag_has(level, LOG_LEVEL_ASSERT)) {
		is_assert = 1;
		flag_rem(level, LOG_LEVEL_ASSERT);
	}

	GetConsoleScreenBufferInfo(_glog.cb.fd, &csbi);

	// "[Level]" + [TODO: Colors]
	switch(level) {
		case LOG_LEVEL_GOOFY: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED |
			                                         FOREGROUND_BLUE |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "==============\n", 0);
			fb8_append_cstr(&_glog.cb, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_glog.cb, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_BLUE |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
			fb8_append_cstr(&_glog.cb, "= DEBUG =\n", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_BLUE |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "========\n", 0);
			fb8_append_cstr(&_glog.cb, "= INFO =\n", 0);
			fb8_append_cstr(&_glog.cb, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
			fb8_append_cstr(&_glog.cb, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED |
			                                         FOREGROUND_BLUE);
			fb8_append_cstr(&_glog.cb, "============\n", 0);
			fb8_append_cstr(&_glog.cb, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_glog.cb, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
			fb8_append_cstr(&_glog.cb, "= WARNING =\n", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
			fb8_append_cstr(&_glog.cb, "= ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED);
			fb8_append_cstr(&_glog.cb, "===============\n", 0);
			fb8_append_cstr(&_glog.cb, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "===============\n", 0);
		} break;

		default: {
			SetConsoleTextAttribute(_glog.cb.fd, FOREGROUND_RED);
			fb8_append_cstr(&_glog.cb, "==================\n", 0);
			fb8_append_cstr(&_glog.cb, "= INTERNAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "==================\n", 0);

			SetConsoleTextAttribute(_glog.cb.fd, csbi.wAttributes);
			fb8_append_cstr(&_glog.cb, "Must be a single level!\n", 0);
			fb8_flush(&_glog.cb);
			ExitProcess(0);
		}
	}

	fb8_flush(&_glog.cb);
	SetConsoleTextAttribute(_glog.cb.fd, csbi.wAttributes);

	//	"[10:00:00]"
	GetLocalTime(&lt);

	fb8_append_byte(&_glog.cb, '[');
	fb8_append_usz(&_glog.cb, lt.wHour);
	fb8_append_byte(&_glog.cb, ':');
	fb8_append_usz(&_glog.cb, lt.wMinute);
	fb8_append_byte(&_glog.cb, ':');
	fb8_append_usz(&_glog.cb, lt.wSecond);
	fb8_append_byte(&_glog.cb, '.');
	#if EXE_ARCH == 32
		fb8_append_usz(&_glog.cb, lt.wMilliseconds);
		unref(time_100ns);
		unref(ft);
	#else
		GetSystemTimeAsFileTime(&ft);
		time_100ns = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		time_100ns %= 10000000;
		fb8_append_usz(&_glog.cb, time_100ns);
	#endif
	fb8_append_byte(&_glog.cb, ']');

	fb8_append_lf(&_glog.cb);
	fb8_append(&_glog.cb, msg);

	fb8_append_lf(&_glog.cb);
	fb8_append_lf(&_glog.cb);
	fb8_flush(&_glog.cb);

	// "file(line_num): fnname"
	if (is_assert) {
		fb8_append(&_glog.cb, s8("Assert: True\n"));

		fb8_append_cstr(&_glog.cb, file, 0);
		fb8_append_byte(&_glog.cb, '(');
		fb8_append_usz( &_glog.cb, linenum);
		fb8_append_byte(&_glog.cb, ')');
		fb8_append_byte(&_glog.cb, ':');
		fb8_append_byte(&_glog.cb, ' ');
		fb8_append_cstr(&_glog.cb, fnname, 0);
		fb8_append_lf(&_glog.cb);
		fb8_append_lf(&_glog.cb);

		fb8_flush(&_glog.cb);
	}

	__mcs_unlock(&_glock_terminal, &me);
}

local void
win32_logc(u32 level,
           const char *msg,
           const char *file,
           usz linenum,
           const char *fnname)
{
	s8 s_msg;
	s_msg.data    = (u8 *)msg;
	s_msg.len     = c_strlen(msg);
	win32_log(level, s_msg, file, linenum, fnname);
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

#if EXE_ARCH == 32
local void
win32_crash_print_registers(fb8 *fb, PCONTEXT ctx)
{
	DWORD *ebp;
	DWORD eip;
	int frame;

	fb8_append(fb, s8("Register Values: "));
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- eax: "));
	fb8_append_hex(fb, ctx->Eax);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- ebx: "));
	fb8_append_hex(fb, ctx->Ebx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- ecx: "));
	fb8_append_hex(fb, ctx->Ecx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- edx: "));
	fb8_append_hex(fb, ctx->Edx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- esi: "));
	fb8_append_hex(fb, ctx->Esi);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- edi: "));
	fb8_append_hex(fb, ctx->Edi);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- esp: "));
	fb8_append_hex(fb, ctx->Esp);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- ebp: "));
	fb8_append_hex(fb, ctx->Ebp);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- eip: "));
	fb8_append_hex(fb, ctx->Eip);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- eflags: "));
	fb8_append_hex(fb, ctx->EFlags);
	fb8_append_lf(fb);
	fb8_append_lf(fb);

	fb8_append(fb, s8("Stack Frames:"));
	fb8_append_lf(fb);
	ebp = (DWORD *)ctx->Ebp;
	for (frame = 0; frame < 20; ++frame) {
		if (!ebp || IsBadReadPtr(ebp, sizeof(DWORD) * 2))
			break;

		eip = *(ebp + 1);
		ebp = (DWORD *)(*ebp);

		if (eip == 0 || IsBadCodePtr((FARPROC)eip))
			break;

		fb8_append(fb, s8("\t- Frame ["));
		fb8_append_isz(fb, frame);
		fb8_append(fb, s8("]: "));
		fb8_append_hex(fb, eip);
		fb8_append_lf(fb);
	}
}

#else
local void
win32_crash_print_registers(fb8 *fb, PCONTEXT ctx)
{
	PRUNTIME_FUNCTION pfn;
	PVOID HandlerData;
	DWORD64 ImageBase;
	DWORD64 EstablisherFrame;
	PEXCEPTION_ROUTINE r;
	int i;

	fb8_append(fb, s8("Register Values: "));
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rax: "));
	fb8_append_hex(fb, ctx->Rax);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rbx: "));
	fb8_append_hex(fb, ctx->Rbx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rcx: "));
	fb8_append_hex(fb, ctx->Rcx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rdx: "));
	fb8_append_hex(fb, ctx->Rdx);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rsi: "));
	fb8_append_hex(fb, ctx->Rsi);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rdi: "));
	fb8_append_hex(fb, ctx->Rdi);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rsp: "));
	fb8_append_hex(fb, ctx->Rsp);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rbp: "));
	fb8_append_hex(fb, ctx->Rbp);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r8: "));
	fb8_append_hex(fb, ctx->R8);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r9: "));
	fb8_append_hex(fb, ctx->R9);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r10: "));
	fb8_append_hex(fb, ctx->R10);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r11: "));
	fb8_append_hex(fb, ctx->R11);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r12: "));
	fb8_append_hex(fb, ctx->R12);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r13: "));
	fb8_append_hex(fb, ctx->R13);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r14: "));
	fb8_append_hex(fb, ctx->R14);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r15: "));
	fb8_append_hex(fb, ctx->R15);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rip: "));
	fb8_append_hex(fb, ctx->Rip);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rflags: "));
	fb8_append_hex(fb, ctx->EFlags);
	fb8_append_lf(fb);

	fb8_append(fb, s8("Stack Walk: "));
	fb8_append_hex(fb, ctx->Rip);
	fb8_append_lf(fb);

	for (i = 0; i < 20; ++i) {
		pfn = RtlLookupFunctionEntry(ctx->Rip, &ImageBase, 0);
		if (!pfn) {
			fb8_append(fb, s8("\t- No unwind info found for RIP:"));
			fb8_append_hex(fb, ctx->Rip);
			break;
		}

		fb8_append(fb, s8("\t- Stack ["));
		fb8_append_isz(fb, i);
		fb8_append(fb, s8("]: "));
		fb8_append_hex(fb, ctx->Rip);

		r = RtlVirtualUnwind(UNW_FLAG_NHANDLER,
		                     ImageBase,
		                     ctx->Rip,
		                     pfn,
		                     ctx,
		                     &HandlerData,
		                     &EstablisherFrame,
		                     0);

		if (r == 0x0 || ctx->Rip == 0x0)
			break;
	}
}
#endif // EXE_ARCH == 32

local LONG WINAPI
win32_crash_handler(PEXCEPTION_POINTERS ExceptionInfo)
{
	CONTEXT *ctx = ExceptionInfo->ContextRecord;

	fb8 fb = {0};
	u8 buffer[KB(4)];

	fb.data = buffer;
	fb.cap  = KB(4);

	fb8_append(&fb, s8("A Crash Has Ocurred!"));
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);

	win32_crash_print_registers(&fb, ctx);

	fb8_append_lf(&fb);
	fb8_append_lf(&fb);
	fb8_append(&fb, s8("TODO: Create xxd of All Mapped Memory. Then "));
	fb8_append(&fb, s8("Compress it, Then write crash info to file(s)"));

	log_fatal(fb.b);
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif // INCLUDE_PLATFORM_WIN32_LOG_H

