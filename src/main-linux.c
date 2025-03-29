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
	logsz = linux_logc;
	logs8 = linux_log;

	buffer = mmap_anon(MB(128));
	marena_init(&sysmem, buffer, MB(128), page_size);

	linux_init_logger(&sysmem, 0xFF, 0xF);
	linux_setup_crash_handler();
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
	assert(0, "Shouldn't really pass until custom dl is completed. . .");
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

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);
	pd.os_write = fb8_write;
	pd.get_cpu_vendor = linux_cpuid_getvendor;
	pd.std_out = 0;
	pd.run_app = 1;
	pd.logsz  = logsz;

	app_init(&pd);
	rest.tv_sec = 1;
	while (pd.run_app) {
		app_update(&pd);
		sys_nanosleep(&rest, 0);
	}
	app_close();

	usz *_killme = 0;
	*_killme = 0xDEADA55E;

	return 0;
}

