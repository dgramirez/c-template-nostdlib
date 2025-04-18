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
		_glog.cb.fd   = 0;
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
linux_log(u32 level,
          s8 msg,
          const char *file,
          usz linenum,
          const char *fnname)
{
	MCSLock me;
	LogTime lt;

	__mcs_lock(&_glock_terminal, &me);

	usz is_assert;
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

	// "[Level]" + [TODO: Colors]
	switch(level) {
		case LOG_LEVEL_GOOFY: {
			fb8_append_cstr(&_glog.cb, "\033[97m", 0);
			fb8_append_cstr(&_glog.cb, "==============\n", 0);
			fb8_append_cstr(&_glog.cb, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_glog.cb, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			fb8_append_cstr(&_glog.cb, "\033[96m", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
			fb8_append_cstr(&_glog.cb, "= DEBUG =\n", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			fb8_append_cstr(&_glog.cb, "\033[94m", 0);
			fb8_append_cstr(&_glog.cb, "========\n", 0);
			fb8_append_cstr(&_glog.cb, "= INFO =\n", 0);
			fb8_append_cstr(&_glog.cb, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			fb8_append_cstr(&_glog.cb, "\033[92m", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
			fb8_append_cstr(&_glog.cb, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			fb8_append_cstr(&_glog.cb, "\033[35m", 0);
			fb8_append_cstr(&_glog.cb, "============\n", 0);
			fb8_append_cstr(&_glog.cb, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_glog.cb, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			fb8_append_cstr(&_glog.cb, "\033[93m", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
			fb8_append_cstr(&_glog.cb, "= WARNING =\n", 0);
			fb8_append_cstr(&_glog.cb, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			fb8_append_cstr(&_glog.cb, "\033[91m", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
			fb8_append_cstr(&_glog.cb, "= ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			fb8_append_cstr(&_glog.cb, "\033[31m", 0);
			fb8_append_cstr(&_glog.cb, "===============\n", 0);
			fb8_append_cstr(&_glog.cb, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "===============\n", 0);
		} break;

		default: {
			fb8_append_cstr(&_glog.cb, "\033[31m", 0);
			fb8_append_cstr(&_glog.cb, "==================\n", 0);
			fb8_append_cstr(&_glog.cb, "= INTERNAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.cb, "==================\n", 0);
			fb8_flush(&_glog.cb);

			fb8_append_cstr(&_glog.cb, "\033[0m", 0);
			fb8_append_cstr(&_glog.cb, "Must be a single level!\n", 0);
			fb8_flush(&_glog.cb);
			sys_exit(0);
		}
	}

	fb8_flush(&_glog.cb);
	fb8_append_cstr(&_glog.cb, "\033[0m", 0);


	//	"[10:00:00]"
	linux_get_datetime(&lt);

	fb8_append_byte(&_glog.cb, '[');
	fb8_append_usz(&_glog.cb, lt.year);
	fb8_append_byte(&_glog.cb, '/');
	fb8_append_usz(&_glog.cb, lt.month);
	fb8_append_byte(&_glog.cb, '/');
	fb8_append_usz(&_glog.cb, lt.day);
	fb8_append_byte(&_glog.cb, ' ');

	fb8_append_usz(&_glog.cb, lt.hour);
	fb8_append_byte(&_glog.cb, ':');
	fb8_append_usz(&_glog.cb, lt.min);
	fb8_append_byte(&_glog.cb, ':');
	fb8_append_usz(&_glog.cb, lt.sec);
	fb8_append_byte(&_glog.cb, '.');
	fb8_append_usz(&_glog.cb, lt.nsec);
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
linux_logc(u32 level,
           const char *msg,
           const char *file,
           usz linenum,
           const char *fnname)
{
	s8 s_msg;
	s_msg.data    = (u8 *)msg;
	s_msg.len     = c_strlen(msg);
	linux_log(level, s_msg, file, linenum, fnname);
}

local void
linux_crash_print_registers(fb8 *fb, struct ucontext_t *ctx)
{
	greg_t *greg;
	greg = ctx->uc_mcontext.gregs;
	usz *rbp;
	usz  rip;
	int frame;

	fb8_append(fb, s8("Register Values: "));
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rax: "));
	fb8_append_hex(fb, greg[REG_RAX]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rbx: "));
	fb8_append_hex(fb, greg[REG_RBX]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rcx: "));
	fb8_append_hex(fb, greg[REG_RCX]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rdx: "));
	fb8_append_hex(fb, greg[REG_RDX]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rsi: "));
	fb8_append_hex(fb, greg[REG_RSI]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rdi: "));
	fb8_append_hex(fb, greg[REG_RDI]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rsp: "));
	fb8_append_hex(fb, greg[REG_RSP]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rbp: "));
	fb8_append_hex(fb, greg[REG_RBP]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r8: "));
	fb8_append_hex(fb, greg[REG_R8]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r9: "));
	fb8_append_hex(fb, greg[REG_R9]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r10: "));
	fb8_append_hex(fb, greg[REG_R10]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r11: "));
	fb8_append_hex(fb, greg[REG_R11]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r12: "));
	fb8_append_hex(fb, greg[REG_R12]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r13: "));
	fb8_append_hex(fb, greg[REG_R13]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r14: "));
	fb8_append_hex(fb, greg[REG_R14]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- r15: "));
	fb8_append_hex(fb, greg[REG_R15]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rip: "));
	fb8_append_hex(fb, greg[REG_RIP]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("\t- rflags: "));
	fb8_append_hex(fb, greg[REG_EFL]);
	fb8_append_lf(fb);

	fb8_append(fb, s8("Stack Frames:"));
	fb8_append_lf(fb);

	rbp = (usz *)greg[REG_RBP];
	for (frame = 0; frame < 20; ++frame) {
		if (!rbp)
			break;

		rip = *(rbp + 1);
		rbp = (usz *)(*rbp);

		if (rip == 0)
			break;

		fb8_append(fb, s8("\t- Frame ["));
		fb8_append_isz(fb, frame);
		fb8_append(fb, s8("]: "));
		fb8_append_hex(fb, rip);
		fb8_append_lf(fb);
	}

}

local void
linux_crash_handler(int signo,
                    siginfo_t *info,
                    void* context)
{
	struct ucontext_t* ctx;

	fb8 fb = {0};
	u8 buffer[KB(4)];

	fb.data = buffer;
	fb.cap  = KB(4);
	fb.fd   = 0;

	fb8_append(&fb, s8("A Crash Has Ocurred!"));
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);

	ctx = (struct ucontext_t *)context;
	linux_crash_print_registers(&fb, ctx);

	fb8_append_lf(&fb);
	fb8_append_lf(&fb);
	fb8_append(&fb, s8("TODO: Create xxd of All Mapped Memory. Then "));
	fb8_append(&fb, s8("Compress it, Then write crash info to file(s)"));

	log_fatal(fb.b);
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

