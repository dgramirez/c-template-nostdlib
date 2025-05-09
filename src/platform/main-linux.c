#include "main-linux.h"

int
cmain(b8 args,
      b8 mem)
{
	PlatformData pdata;
	MBuddy       mbuddy;
	TPData       tpdata = {0};

	main_setup_memory(mem, &mbuddy);

	cpuid_init();
	linux_init_logger_terminal(mbuddy_alloc(&mbuddy, page_size),
	                           page_size,
	                           LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                           LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                           LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                           LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL);
	linux_init_logger_file(mbuddy_alloc(&mbuddy, page_size),
	                       page_size,
	                       0,
	                       LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                       LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                       LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                       LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL   |
	                       LOG_FORMAT_FILE_7Z);
	linux_init_logger_network(mbuddy_alloc(&mbuddy, page_size),
	                          page_size,
	                          0,
	                          LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                          LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                          LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                          LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL   |
	                          LOG_FORMAT_FILE_7Z);
	linux_setup_crash_handler();

	linux_setup_shared_lib_app();
	assert(app_init,   "Failed to get app_init from libapp.so");
	assert(app_update, "Failed to get app_update from libapp.so");
	assert(app_close,  "Failed to get app_close from libapp.so");

	tp_init_generic(&tpdata, 1, KB(16), KB(4), 0);

	linux_setup_platform_data(&pdata, &mbuddy);
	pdata.tp_data = &tpdata;
	app_init(&pdata);
	while (pdata.run_app) {
		app_update(&pdata);
	}
	app_close();

	tp_wait(&tpdata, 0);
	tp_quit(&tpdata);
	return 0;
}

local void *
linux_setup_shared_lib_app()
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
linux_setup_platform_data(PlatformData *pdata,
                          MBuddy       *mbuddy)
{
	// Buffer
	pdata->bufapp.len  = mbuddy->len >> 2;
	ceilto_pow2(pdata->bufapp.len);
	pdata->bufapp.data = mbuddy_alloc(mbuddy, pdata->bufapp.len);
	if (!pdata->bufapp.data)
		return;

	// PFN
	pdata->os_write      = fb8_write;
	pdata->logsz         = logsz;
	pdata->logs8         = logs8;
	pdata->mlock_init    = mlock_init_mcslock;
	pdata->mlock_acquire = mlock_acquire_mcslock;
	pdata->mlock_release = mlock_release_mcslock;
	pdata->mlock_free    = mlock_free_mcslock;
	pdata->tp_post       = tp_post_generic;
	pdata->tp_quit       = tp_quit_generic;
	pdata->tp_wait       = tp_wait_generic;
	cpuid_setup(&pdata->cpuid);
	// Others
	pdata->tlock_terminal = &_glock_terminal;
	pdata->std_out = 0;
	pdata->run_app = 1;
}

