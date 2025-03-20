#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	MArena sysmem;
	PlatformData pd = {0};
	void *buffer;

	os_write = fb8_write;
	logsys = win32_log;

	HANDLE lib = LoadLibraryA("libapp.dll");
	assert(lib, "Failed to load library: libapp.dll");

	deffn_dll(lib, app_init);
	deffn_dll(lib, app_update);
	deffn_dll(lib, app_close);

	assert(app_init,   "Failed to get Fn from libapp.dll: app_init");
	assert(app_update, "Failed to get Fn from libapp.dll: app_update");
	assert(app_close,  "Failed to get Fn from libapp.dll: app_close");

	buffer = VirtualAlloc(0,
	                      MB(128),
	                      MEM_COMMIT | MEM_RESERVE,
	                      PAGE_READWRITE);
	marena_init(&sysmem, buffer, MB(128), page_size);

	win32_init_logger(&sysmem, 0xFF, 0xF);
	log_debug("Test!");

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);
	pd.os_write = fb8_write;
	pd.get_cpu_vendor = get_cpu_vendor;
	pd.std_out = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
	pd.run_app = 1;
	pd.logsys = win32_log;

	app_init(&pd);
	while(pd.run_app) {
		app_update();
		Sleep(1);
	}
	app_close();

	unref(argc);
	unref(argv);
	return 0;
}

local i32
fb8_write(fb8 *b)
{
	DWORD written;
	return WriteFile(b->fd, b->data, (DWORD)b->len, &written, 0);
}

local b8
get_cpu_vendor(u8 *buffer, usz len)
{
	b8 b;
	u32 *bptr;

	if (len < 12)
		return s8("");
	b.data = buffer;
	b.len  = 12;

	unsigned int eax, ebx, ecx, edx;
	eax = 0;
	cpuid_native(&eax, &ebx, &ecx, &edx);

	bptr  = (u32 *)&b.data[0];
	*bptr = ebx;

	bptr  = (u32 *)&b.data[4];
	*bptr = edx;

	bptr  = (u32 *)&b.data[8];
	*bptr = ecx;
	if (len >= 16) {
		bptr  = (u32 *)&b.data[12];
		*bptr = 0;
	}

	return b;
}

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
	fb8_append_usz(&_g_logger.cb, lt.wMilliseconds);
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

