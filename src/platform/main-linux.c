#include "main-linux.h"

int
main(int argc,
     char **argv)
{
	PlatformData pd = {0};
	MArena sysmem;
	struct timespec rest = {0};
	void *buffer;
	TpAppHandle tph;

	os_write = fb8_write;
	logsz = linux_logc;
	logs8 = linux_log;

	buffer = mmap_anon(MB(128));
	marena_init(&sysmem, buffer, MB(128), page_size);
	linux_init_logger(&sysmem, 0xFF, 0xF);
	linux_setup_crash_handler();

	print_fn_addresses(&sysmem);

	unref(tph);
	tph = appjob_init_threadpool(&sysmem, 4, KB(32), KB(16));
	appjob_post(tph, mmm_donuts, 0, 0);
	appjob_post(tph, mmm_cake, 0, 0);
	appjob_post(tph, mmm_pies, 0, 0);

	setup_shared_lib_app();
	assert(app_init,   "Failed to get app_init from libapp.so");
	assert(app_update, "Failed to get app_update from libapp.so");
	assert(app_close,  "Failed to get app_close from libapp.so");

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);
	pd.os_write = fb8_write;
	pd.cpuid_vendor = linux_cpuid_getvendor;
	pd.std_out = 0;
	pd.run_app = 1;
	pd.logsz  = logsz;

	app_init(&pd);
	rest.tv_sec = 5;
	while (pd.run_app) {
		app_update(&pd);
		sys_nanosleep(&rest, 0);
	}
	app_close();

//	linux_job_wait(tph);
//	linux_job_quit(tph);
	usz *_killme = 0;
	*_killme = 0xDEADA55E;

	return 0;
}

local void *
setup_shared_lib_app()
{
#ifndef USING_LIBC
	// A few notes for me:
	// 1.) Maybe I should somewhat follow "dlopen"'s abi.
	//     As in: return a (void *) with the necessary data to use for
	//     dlsym and dlclose.
	// 2.) Also, Perhaps a fresh perspective may be in order. I may want
	//     to bring in new allocators (free-list, buddy, slab, etc.)
	// 3.) I have to understand how much of a time sink this will be, as
	//     I am unaware on how to handle certain aspects still (like TLS)
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

	return 0;
#else
	const char *solib = "./libapp.so";
	void *app = dlopen(solib, RTLD_NOW);
	assert(app, "DL has failed to open!");

	deffn_dlsym(app, app_init);
	deffn_dlsym(app, app_update);
	deffn_dlsym(app, app_close);

	return app;
#endif
}

local void
mmm_donuts(void *arg, ThreadAppJobData *thread)
{
	logc_pass("Mmm Donuts...");
}

local void
mmm_cake(void *arg, ThreadAppJobData *thread)
{
	logc_odd("Mmm Cake...");
}

local void
mmm_pies(void *arg, ThreadAppJobData *thread)
{
	logc_warn("Mmm Pies...");
}

local void
print_fn_addresses(MArena *a)
{
	MArenaTemp at = {0};
	fb8 fb = {0};

	marena_save(&at, a);

	fb.data = marena_alloc(a, KB(16), 8);
	fb.cap = KB(16);

	fb8_append(&fb, s8("main:                "));
	fb8_append_hex(&fb, (usz)main);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("mcs_lock:            "));
	fb8_append_hex(&fb, (usz)mcs_lock);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("mcs_unlock:          "));
	fb8_append_hex(&fb, (usz)mcs_unlock);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_load:         "));
	fb8_append_hex(&fb, (usz)atomic_load);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_store:        "));
	fb8_append_hex(&fb, (usz)atomic_store);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("futex_wait:          "));
	fb8_append_hex(&fb, (usz)futex_wait);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("futex_wake:          "));
	fb8_append_hex(&fb, (usz)futex_wake);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("cpu_relax:           "));
	fb8_append_hex(&fb, (usz)cpu_relax);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_inc:          "));
	fb8_append_hex(&fb, (usz)atomic_inc);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_dec:          "));
	fb8_append_hex(&fb, (usz)atomic_dec);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_cas:      "));
	fb8_append_hex(&fb, (usz)atomic_cas);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("atomic_swap:       "));
	fb8_append_hex(&fb, (usz)atomic_swap);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("__mcs_lock:           "));
	fb8_append_hex(&fb, (usz)__mcs_lock);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("__mcs_unlock:         "));
	fb8_append_hex(&fb, (usz)__mcs_unlock);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("linux_log:           "));
	fb8_append_hex(&fb, (usz)linux_log);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("linux_logc:          "));
	fb8_append_hex(&fb, (usz)linux_logc);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("mmm_donuts:          "));
	fb8_append_hex(&fb, (usz)mmm_donuts);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("mmm_cake:            "));
	fb8_append_hex(&fb, (usz)mmm_cake);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("mmm_pies:            "));
	fb8_append_hex(&fb, (usz)mmm_pies);
	fb8_append_lf(&fb);

	log_debug(fb.b);
	marena_load(&at);
}

