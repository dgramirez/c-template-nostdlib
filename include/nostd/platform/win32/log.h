#ifndef INCLUDE_PLATFORM_WIN32_LOG_H
#define INCLUDE_PLATFORM_WIN32_LOG_H

local void
win32_init_logger_terminal(void *buf,
                           usz   len,
                           usz   flags)
{
	if (!buf || !len)
		return;

	_glog.fb_terminal.fd   = GetStdHandle(STD_OUTPUT_HANDLE);
	_glog.fb_terminal.data = buf;
	_glog.fb_terminal.cap  = len;
	_glog.flags_terminal   = flags;
}

local void
win32_init_logger_file(void *buf,
                       usz   len,
                       usz   fd,
                       usz   flags)
{
	if (!buf || !len || !fd)
		return;

	_glog.fb_file.fd   = (void *)fd;
	_glog.flags_file   = flags;
	_glog.fb_file.data = buf;
	_glog.fb_file.cap  = len;
}

local void
win32_init_logger_network(void *buf,
                          usz   len,
                          usz   fd,
                          usz   flags)
{
	if (!buf || !len || !fd)
		return;

	_glog.fb_network.fd   = (void *)fd;
	_glog.flags_network   = flags;
	_glog.fb_network.data = buf;
	_glog.fb_network.cap  = len;
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
	MCSMutex                   mutex;
	usz                        is_assert;

	mutex.lock = &_glock_terminal;
	if (mlock_acquire)
		mlock_acquire(&mutex);

	if (_glog.fb_file.data || _glog.fb_file.cap < page_size) {
		// TODO: Write to File
		
	}
	if (_glog.fb_network.data || _glog.fb_network.cap < page_size) {
		// TODO: Send over Network; Protocol Needed!
		
	}

	if (!_glog.fb_terminal.data || _glog.fb_terminal.cap < page_size)
		return;
	if (!flag_has(_glog.flags_terminal, level))
		return;

	is_assert = 0;
	if (flag_has(level, LOG_LEVEL_ASSERT)) {
		is_assert = 1;
		flag_rem(level, LOG_LEVEL_ASSERT);
	}

	// "[Level]" + [TODO: Colors]
	GetConsoleScreenBufferInfo(_glog.fb_terminal.fd, &csbi);
	switch(level) {
		case LOG_LEVEL_GOOFY: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED |
			                                         FOREGROUND_BLUE |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "==============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_BLUE |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= DEBUG =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_BLUE |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= INFO =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED |
			                                         FOREGROUND_BLUE);
			fb8_append_cstr(&_glog.fb_terminal, "============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED |
			                                         FOREGROUND_GREEN |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= WARNING =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED |
			                                         FOREGROUND_INTENSITY);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED);
			fb8_append_cstr(&_glog.fb_terminal, "===============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===============\n", 0);
		} break;

		default: {
			SetConsoleTextAttribute(_glog.fb_terminal.fd, FOREGROUND_RED);
			fb8_append_cstr(&_glog.fb_terminal, "==================\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= INTERNAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==================\n", 0);

			SetConsoleTextAttribute(_glog.fb_terminal.fd, csbi.wAttributes);
			fb8_append_cstr(&_glog.fb_terminal, "Must be a single level!\n", 0);
			fb8_flush(&_glog.fb_terminal);
			ExitProcess(0);
		}
	}

	fb8_flush(&_glog.fb_terminal);
	SetConsoleTextAttribute(_glog.fb_terminal.fd, csbi.wAttributes);

	//	"[10:00:00]"
	GetLocalTime(&lt);

	fb8_append_byte(&_glog.fb_terminal, '[');
	fb8_append_usz(&_glog.fb_terminal, lt.wHour);
	fb8_append_byte(&_glog.fb_terminal, ':');
	fb8_append_usz(&_glog.fb_terminal, lt.wMinute);
	fb8_append_byte(&_glog.fb_terminal, ':');
	fb8_append_usz(&_glog.fb_terminal, lt.wSecond);
	fb8_append_byte(&_glog.fb_terminal, '.');
	#if EXE_ARCH == 32
		fb8_append_usz(&_glog.fb_terminal, lt.wMilliseconds);
		unref(time_100ns);
		unref(ft);
	#else
		GetSystemTimeAsFileTime(&ft);
		time_100ns = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		time_100ns %= 10000000;
		fb8_append_usz(&_glog.fb_terminal, time_100ns);
	#endif
	fb8_append_byte(&_glog.fb_terminal, ']');

	fb8_append_lf(&_glog.fb_terminal);
	fb8_append(&_glog.fb_terminal, msg);

	fb8_append_lf(&_glog.fb_terminal);
	fb8_append_lf(&_glog.fb_terminal);
	fb8_flush(&_glog.fb_terminal);

	// "file(line_num): fnname"
	if (is_assert) {
		fb8_append(&_glog.fb_terminal, s8("Assert: True\n"));

		fb8_append_cstr(&_glog.fb_terminal, file, 0);
		fb8_append_byte(&_glog.fb_terminal, '(');
		fb8_append_usz( &_glog.fb_terminal, linenum);
		fb8_append_byte(&_glog.fb_terminal, ')');
		fb8_append_byte(&_glog.fb_terminal, ':');
		fb8_append_byte(&_glog.fb_terminal, ' ');
		fb8_append_cstr(&_glog.fb_terminal, fnname, 0);
		fb8_append_lf(&_glog.fb_terminal);
		fb8_append_lf(&_glog.fb_terminal);

		fb8_flush(&_glog.fb_terminal);
	}

	if (mlock_release)
		mlock_release(&mutex);
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
	fb.fd   = GetStdHandle(STD_OUTPUT_HANDLE); 

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

