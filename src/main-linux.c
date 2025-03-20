#include "main-linux.h"

int
main(int argc,
     char **argv)
{
	PlatformData pd = {0};
	MArena sysmem;
	struct timespec rest = {0};
	void *buffer;

	os_write = fb8_write;
	logsys = linux_log;

	// TODO: Turn this entire section into a threadpool!
//	StackHead *sh = (StackHead *)((u8 *)mmap_anon(KB(4)) + KB(4) - sizeof(StackHead));
//	struct timespec ts;
//	ts.tv_sec = 3;
//	create_thread(sh, mmm_sleep, &ts);
//
//	s8 hs = s8("Waiting for nanosleep futex. . .");
//	fb8_append(&hw2, hs);
//	fb8_append_lf(&hw2);
//	fb8_append_lf(&hw2);
//	fb8_flush(&hw2);
//
//	futex_wait(&sh->join_futex);
	// --- SECTION--------------------------------------
#ifndef USING_LIBC
	Solib app = {0};
	b8 dlmem;
	dlmem.len  = MB(8);
	dlmem.data = mmap_anon(dlmem.len);

	dlinit(dlmem.data, dlmem.len);
	dlopen(&app, "libapp.so");

	deffn_dlsym(&app, app_init);
	deffn_dlsym(&app, app_update);
	deffn_dlsym(&app, app_close);
#else
	const char *solib = "./libapp.so";
	void *app = dlopen(solib, RTLD_NOW);
	assert(app, "DL has failed to open!");

	deffn_dlsym(app, app_init);
	deffn_dlsym(app, app_update);
	deffn_dlsym(app, app_close);
#endif

	assert(app_init,   "Failed to get app_init from libapp.so");
	assert(app_update, "Failed to get app_update from libapp.so");
	assert(app_close,  "Failed to get app_close from libapp.so");

	buffer = mmap_anon(MB(128));
	marena_init(&sysmem, buffer, MB(128), page_size);
	linux_init_logger(&sysmem, 0xFF, 0xF);

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);
	pd.os_write = fb8_write;
	pd.get_cpu_vendor = get_cpu_vendor;
	pd.std_out = 0;
	pd.run_app = 1;
	pd.logsys  = logsys;

	app_init(&pd);
	rest.tv_sec = 1;
	while (pd.run_app) {
		app_update(&pd);
		sys_nanosleep(&rest, 0);
	}
	app_close();

	return 0;
}

local void
mmm_sleep(void *args)
{
	struct timespec *ts = (struct timespec *)args;
	sys_nanosleep(ts, 0);
}

local i32
fb8_write(fb8 *b)
{
	return sys_write((usz)b->fd, b->data, b->len);
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
linux_init_logger(MArena *arena,
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
		_g_logger.cb.fd   = 0;
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
linux_log(u32 level,
          const char *msg,
          const char *file,
          usz linenum,
          const char *fnname)
{
	LogTime lt;

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

	// "[Level]" + [TODO: Colors]
	switch(level) {
		case LOG_LEVEL_GOOFY: {
			fb8_append_cstr(&_g_logger.cb, "\033[97m", 0);
			fb8_append_cstr(&_g_logger.cb, "==============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			fb8_append_cstr(&_g_logger.cb, "\033[96m", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= DEBUG =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			fb8_append_cstr(&_g_logger.cb, "\033[94m", 0);
			fb8_append_cstr(&_g_logger.cb, "========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= INFO =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			fb8_append_cstr(&_g_logger.cb, "\033[92m", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			fb8_append_cstr(&_g_logger.cb, "\033[35m", 0);
			fb8_append_cstr(&_g_logger.cb, "============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			fb8_append_cstr(&_g_logger.cb, "\033[93m", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= WARNING =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			fb8_append_cstr(&_g_logger.cb, "\033[91m", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= ERROR =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			fb8_append_cstr(&_g_logger.cb, "\033[31m", 0);
			fb8_append_cstr(&_g_logger.cb, "===============\n", 0);
			fb8_append_cstr(&_g_logger.cb, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_g_logger.cb, "===============\n", 0);
		} break;

		default: assert(0, "Must be a single level!");
	}

	fb8_flush(&_g_logger.cb);
	fb8_append_cstr(&_g_logger.cb, "\033[0m", 0);


	//	"[10:00:00]"
	linux_get_datetime(&lt);

	fb8_append_byte(&_g_logger.cb, '[');
	fb8_append_usz(&_g_logger.cb, lt.year);
	fb8_append_byte(&_g_logger.cb, '/');
	fb8_append_usz(&_g_logger.cb, lt.month);
	fb8_append_byte(&_g_logger.cb, '/');
	fb8_append_usz(&_g_logger.cb, lt.day);
	fb8_append_byte(&_g_logger.cb, ' ');

	fb8_append_usz(&_g_logger.cb, lt.hour);
	fb8_append_byte(&_g_logger.cb, ':');
	fb8_append_usz(&_g_logger.cb, lt.min);
	fb8_append_byte(&_g_logger.cb, ':');
	fb8_append_usz(&_g_logger.cb, lt.sec);
	fb8_append_byte(&_g_logger.cb, '.');
	fb8_append_usz(&_g_logger.cb, lt.nsec);
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

local void
linux_get_time(LogTime *lt)
{
	struct timespec ts;
	usz time;

	sys_clock_gettime(CLOCK_REALTIME, &ts);
	time = ts.tv_sec % 86400;
	
	lt->hour = time / 3600;
	time = time % 3600;

	lt->min = time / 60;
	lt->sec = time % 60;
	
	lt->msec = ts.tv_nsec / 1000000;
	lt->usec = ts.tv_nsec / 1000;
	lt->nsec = ts.tv_nsec;
}

local void
linux_get_date(LogTime *lt)
{
	struct timeval t;
	int days;
	int leap;
	sticky const int days_per_month[2][12] = {
	    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Non-leap
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Leap
	};

	sys_gettimeofday(&t, 0);
	days = t.tv_sec / 86400;
	lt->year = 1970;

	leap = is_leap_year(lt->year);
	while (days >= (365 + leap)) {
		days -= 365 + leap;
		lt->year++;
		leap = is_leap_year(lt->year);
	}

	lt->month = 0;
	while (days >= days_per_month[leap][lt->month]) {
		days -= days_per_month[leap][lt->month];
		lt->month++;
	}

	lt->day = days + 1;
	lt->month++;
}

local void
linux_get_datetime(LogTime *lt)
{
	linux_get_date(lt);
	linux_get_time(lt);
}

local int
is_leap_year(int year)
{
	return ((year % 4 == 0 ) && (year % 100 != 0)) || (year % 400 == 0);
}

void *memset(void *s, int c, size_t n)
{
	// NOTE [32-Bit]: Bad for 32-bit support
	usz csz;

	csz= c | ((usz)c << 32);
	memsetu(s, csz, n);
	return s;
}

