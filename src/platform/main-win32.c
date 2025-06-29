#include "main-win32.h"

local int
Win32Main(b8 args, b8 mem)
{
	PlatformData pdata = {0};
	MBuddy mbuddy      = {0};	
	TPData tpdata      = {0};

	main_setup_memory(mem, &mbuddy);
	cpuid_init();

	win32_init_logger_terminal(mbuddy_alloc(&mbuddy, page_size),
	                           page_size,
	                           LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                           LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                           LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                           LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL);
	win32_init_logger_file(mbuddy_alloc(&mbuddy, page_size),
	                       page_size,
	                       0,
	                       LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                       LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                       LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                       LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL   |
	                       LOG_FORMAT_FILE_7Z);
	win32_init_logger_network(mbuddy_alloc(&mbuddy, page_size),
	                          page_size,
	                          0,
	                          LOG_LEVEL_GOOFY    | LOG_LEVEL_DEBUG   |
	                          LOG_LEVEL_INFO     | LOG_LEVEL_SUCCESS |
	                          LOG_LEVEL_ANOMALLY | LOG_LEVEL_WARNING |
	                          LOG_LEVEL_ERROR    | LOG_LEVEL_FATAL   |
	                          LOG_FORMAT_FILE_7Z);

	HANDLE app = LoadLibraryA("libapp.dll");
	assert(app, "Failed to load the app dll. . .");

	deffn_dll(app, app_init);
	deffn_dll(app, app_update);
	deffn_dll(app, app_close);

	assert(app_init,   "Unable to locate inside dll: app_init");
	assert(app_update, "Unable to locate inside dll: app_update");
	assert(app_close,  "Unable to locate inside dll: app_close");

//	tp_init_generic(&tpdata, 1, KB(16), KB(4), 0);
	win32_setup_platform_data(&pdata, &mbuddy);
	pdata.tp_data = &tpdata;

	app_init(&pdata);
	while (pdata.run_app) {
		app_update(&pdata);
	}
	app_close();

	tp_wait(&tpdata, 0);
	tp_quit(&tpdata);

	unref(args);
	return 0;
}

local void
win32_setup_platform_data(PlatformData *pdata,
                          MBuddy       *mbuddy)
{
	pdata->bufapp.len = mbuddy->len >> 2;
	ceilto_pow2(pdata->bufapp.len);
	pdata->bufapp.data = mbuddy_alloc(mbuddy, pdata->bufapp.len);
	if (!pdata->bufapp.data)
		return;

	pdata->os_write        = Win32WriteFile;
	pdata->logsz           = logsz;
	pdata->logs8           = logs8;
	pdata->mlock_init      = mlock_init;
	pdata->mlock_acquire   = mlock_acquire;
	pdata->mlock_release   = mlock_release;
	pdata->mlock_free      = mlock_free;
	pdata->tp_post         = tp_post;
	pdata->tp_quit         = tp_quit;
	pdata->tp_wait         = tp_wait;
	cpuid_setup(&pdata->cpuid);

	// Others
	pdata->tlock_terminal = &_glock_terminal;
	pdata->std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	pdata->run_app = 1;
}

