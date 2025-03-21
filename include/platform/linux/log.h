#ifndef INCLUDE_PLATFORM_LINUX_LOG_H
#define INCLUDE_PLATFORM_LINUX_LOG_H

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
linux_crash_handler(int signo,
                    siginfo_t *info,
                    void* context)
{
//	struct ucontext_t* ctx = (struct ucontext_t*)context;
	log_fatal("A Crash Has Occurred!\n");
	sys_exit(0);
}
 
local void
linux_setup_crash_handler()
{
	struct sigaction_t sa = {0};
    sa.sigaction_fn = linux_crash_handler;
    sa.sa_flags      = 0x4 | SA_RESTORER;
	char b[256];
	fb8  fb = {0};
	int err;

	fb.data = (u8 *)b;
	fb.cap  = 256;

	err = sys_rt_sigaction(SIGSEGV, &sa, 0, 8);
    if (IS_SYSCALL_ERR(err)) {
		fb8_append_cstr(&fb, "SIGSEGV Registration has failed. . .", 0);
		fb8_append_lf(&fb);
		fb8_append_cstr(&fb, "Error: ", 0);
		fb8_append_isz(&fb, SYSCALL_ERR_VAL(err));
		fb8_append_byte(&fb, ' ');
		fb8_append_byte(&fb, '(');
		fb8_append_isz(&fb, sizeof(sigset_t));
		fb8_append_byte(&fb, ')');
		fb8_append_lf(&fb);
		fb8_append_lf(&fb);

		fb8_flush(&fb);
	}

	err = sys_rt_sigaction(SIGBUS, &sa, 0, 8);
    if (IS_SYSCALL_ERR(err)) {
		fb8_append_cstr(&fb, "SIGBUS Registration has failed. . .", 0);
		fb8_append_lf(&fb);
		fb8_append_cstr(&fb, "Error: ", 0);
		fb8_append_isz(&fb, SYSCALL_ERR_VAL(err));
		fb8_append_lf(&fb);
		fb8_append_lf(&fb);

		fb8_flush(&fb);
	}
}

#endif // INCLUDE_PLATFORM_LINUX_LOG_H

