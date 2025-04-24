#ifndef INCLUDE_PLATFORM_LINUX_LOG_H
#define INCLUDE_PLATFORM_LINUX_LOG_H

local void
linux_init_logger_terminal(void *buf,
                           usz   len,
                           usz   flags)
{
	if (!buf || !len)
		return;

	_glog.fb_file.fd       = 0;
	_glog.fb_terminal.data = buf;
	_glog.fb_terminal.cap  = len;
	_glog.flags_terminal   = flags;
}

local void
linux_init_logger_file(void *buf,
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
linux_init_logger_network(void *buf,
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
linux_log(u32 level,
          s8 msg,
          const char *file,
          usz linenum,
          const char *fnname)
{
	MCSLock me;
	LogTime lt;
	usz     is_assert;

	__mcs_lock(&_glock_terminal, &me);

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

	switch(level) {
		case LOG_LEVEL_GOOFY: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[97m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= EASTER EGG =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==============\n", 0);
		} break;
		case LOG_LEVEL_DEBUG: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[96m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= DEBUG =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
		} break;

		case LOG_LEVEL_INFO: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[94m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= INFO =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "========\n", 0);
		} break;

		case LOG_LEVEL_SUCCESS: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[92m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= SUCCESS =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
		} break;

		case LOG_LEVEL_ANOMALLY: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[35m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= ANOMALLY =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "============\n", 0);
		} break;

		case LOG_LEVEL_WARNING: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[93m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= WARNING =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===========\n", 0);
		} break;

		case LOG_LEVEL_ERROR: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[91m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "=========\n", 0);
		} break;

		case LOG_LEVEL_FATAL: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[31m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===============\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= FATAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "===============\n", 0);
		} break;

		default: {
			fb8_append_cstr(&_glog.fb_terminal, "\033[31m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==================\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "= INTERNAL ERROR =\n", 0);
			fb8_append_cstr(&_glog.fb_terminal, "==================\n", 0);
			fb8_flush(&_glog.fb_terminal);

			fb8_append_cstr(&_glog.fb_terminal, "\033[0m", 0);
			fb8_append_cstr(&_glog.fb_terminal, "Must be a single level!\n", 0);
			fb8_flush(&_glog.fb_terminal);
			sys_exit(0);
		}
	}

	fb8_flush(&_glog.fb_terminal);
	fb8_append_cstr(&_glog.fb_terminal, "\033[0m", 0);


	//	"[10:00:00]"
	linux_get_datetime(&lt);

	fb8_append_byte(&_glog.fb_terminal, '[');
	fb8_append_usz(&_glog.fb_terminal, lt.year);
	fb8_append_byte(&_glog.fb_terminal, '/');
	fb8_append_usz(&_glog.fb_terminal, lt.month);
	fb8_append_byte(&_glog.fb_terminal, '/');
	fb8_append_usz(&_glog.fb_terminal, lt.day);
	fb8_append_byte(&_glog.fb_terminal, ' ');

	fb8_append_usz(&_glog.fb_terminal, lt.hour);
	fb8_append_byte(&_glog.fb_terminal, ':');
	fb8_append_usz(&_glog.fb_terminal, lt.min);
	fb8_append_byte(&_glog.fb_terminal, ':');
	fb8_append_usz(&_glog.fb_terminal, lt.sec);
	fb8_append_byte(&_glog.fb_terminal, '.');
	fb8_append_usz(&_glog.fb_terminal, lt.nsec);
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

