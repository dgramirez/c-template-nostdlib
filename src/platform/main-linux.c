#include "main-linux.h"

typedef struct _node_ptr_t {
	struct _node_ptr_t *next;
	usz count;
} pointer_t;

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

	test_mbuddy(&sysmem);
	test_mfreelist(&sysmem);
	return 0;

	print_fn_addresses(&sysmem);

	tph = appjob_init_threadpool(&sysmem, 4, KB(32), KB(16));
	appjob_post(tph, mmm_donuts, 0, 0);
	appjob_post(tph, mmm_cake, 0, 0);
	appjob_post(tph, mmm_pies, 0, 0);

	setup_shared_lib_app();
	assert(app_init,   "Failed to get app_init from libapp.so");
	assert(app_update, "Failed to get app_update from libapp.so");
	assert(app_close,  "Failed to get app_close from libapp.so");

	// Buffer
	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);

	// PFN
	pd.os_write       = fb8_write;
	pd.cpuid_vendor   = linux_cpuid_getvendor;
	pd.mlock_init     = (PFN_mlock_init)mlock_init_mcslock;
	pd.mlock_acquire  = (PFN_mlock_acquire)mlock_acquire_mcslock;
	pd.mlock_release  = (PFN_mlock_release)mlock_release_mcslock;
	pd.logsz          = logsz;
	pd.logs8          = logs8;
	pd.tp_post        = (PFN_tp_post)appjob_post;
	pd.tp_quit        = (PFN_tp_quit)appjob_quit;
	pd.tp_wait_all    = (PFN_tp_wait_all)appjob_wait;

	// Others
	pd.tlock_terminal = &_glock_terminal;
	pd.tp_data        = tph;
	pd.std_out = 0;
	pd.run_app = 1;

	app_init(&pd);
	rest.tv_sec = 5;
	while (pd.run_app) {
		sys_nanosleep(&rest, 0);
		app_update(&pd);
	}
	app_close();

	appjob_wait(tph);
	appjob_quit(tph);
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

	fb8_append(&fb, s8("_afn_atloadW:         "));
	fb8_append_hex(&fb, (usz)_afn_atloadW);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_atstoreW:        "));
	fb8_append_hex(&fb, (usz)_afn_atstoreW);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("futex_wait:          "));
	fb8_append_hex(&fb, (usz)futex_wait);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("futex_wake:          "));
	fb8_append_hex(&fb, (usz)futex_wake);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_cpurelax:           "));
	fb8_append_hex(&fb, (usz)_afn_cpurelax);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_atincW:          "));
	fb8_append_hex(&fb, (usz)_afn_atincW);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_atdecW:          "));
	fb8_append_hex(&fb, (usz)_afn_atdecW);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_atcasW:      "));
	fb8_append_hex(&fb, (usz)_afn_atcasW);
	fb8_append_lf(&fb);

	fb8_append(&fb, s8("_afn_atswapW:       "));
	fb8_append_hex(&fb, (usz)_afn_atswapW);
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

	pointer_t test = {(pointer_t *)0xDEADBEEF, 32};
	pointer_t test2;
	pointer_t test3 = {(pointer_t *)0xC001BABE, 64};
	_afn_atstoreD(&test2, &test);
	_afn_atcasD((void*)&test, (void*)&test2, (void*)&test3);
}

local void
test_mfreelist(MArena *a)
{
	MArenaTemp temp;
	MFreelist fl;
	void *b;
	usz l;
	void *flb[64];

	marena_save(&temp, a);

	l = KB(128); 
	b = marena_alloc(a, l, 8);

	mfreelist_init(&fl, b, l, 8);

	for (int i = 0; i < 64; ++i)
		flb[i] = mfreelist_alloc(&fl, KB(4), 8);

	for (int i = 0; i < 64; ++i)
		mfreelist_free(&fl, flb[i]);

	marena_load(&temp);
}

local void
test_mbuddy(MArena *a)
{
	MArenaTemp temp;
	MBuddy buddy;
	void *b;
	void *bitmap;
	void *bb[64] = {0};
	usz l;
	usz bitmap_len;

	marena_save(&temp, a);

	l = KB(128);
	b = marena_alloc(a, l, l);

	bitmap_len = mbuddy_get_bitmap_len(l);
	bitmap = marena_alloc(a, bitmap_len, 8);

	mbuddy_init(&buddy, b, l, bitmap);

//	bb[0] = mbuddy_alloc(&buddy, KB(7));
//	bb[1] = mbuddy_alloc(&buddy, KB(35));
//	bb[2] = mbuddy_alloc(&buddy, KB(45));
//
//	mbuddy_free(&buddy, bb[0]);
//	mbuddy_free(&buddy, bb[1]);

	for (int i = 0; i < 16; ++i)
		bb[i] = mbuddy_alloc(&buddy, KB(4));

	for (int i = 1; i < 15; ++i)
		mbuddy_free(&buddy, bb[i]);

	mbuddy_free(&buddy, bb[0]);
	mbuddy_free(&buddy, bb[15]);


	marena_load(&temp);
}

