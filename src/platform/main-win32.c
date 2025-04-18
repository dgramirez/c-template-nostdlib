#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	MArena sysmem;
	PlatformData pd = {0};
	void *buffer;
	TpAppStack *tpas = {0};

	SetUnhandledExceptionFilter(win32_crash_handler);

	os_write = Win32WriteFile;
	logsz = win32_logc;
	logs8 = win32_log;

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
	atomic_test();
	tpas = appjob_init_threadpool(&sysmem, 1, KB(32), KB(16));

	appjob_post(tpas, test_fn1, 0, 0);
	appjob_post(tpas, test_fn2, 0, 0);
	appjob_post(tpas, test_fn3, 0, 0);

	// Buffer
	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);

	// PFN
	pd.os_write       = Win32WriteFile;
	pd.cpuid_vendor   = Win32CpuidGetVendor;
	pd.mlock_init     = mlock_init_mcslock;
	pd.mlock_acquire  = mlock_acquire_mcslock;
	pd.mlock_release  = mlock_release_mcslock;
	pd.logsz          = win32_logc;
	pd.logs8          = win32_log;
	pd.tp_post        = (PFN_tp_post)appjob_post;
	pd.tp_quit        = (PFN_tp_quit)appjob_quit;
	pd.tp_wait_all    = (PFN_tp_wait_all)appjob_wait;


	// Others
	pd.tlock_terminal = &_glock_terminal;
	pd.tp_data        = tpas;
	pd.std_out        = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
	pd.run_app        = 1;

	app_init(&pd);
	while(pd.run_app) {
		app_update();
	}
	app_close();

	appjob_wait(tpas);
	appjob_quit(tpas);

	usz *___killme = 0;
	*___killme = 0xDEADA55E;

	unref(argc);
	unref(argv);
	return 0;
}

local void
atomic_test()
{
	isz x;
	isz y;
	isz z;

	_afn_atstoreW(&x, 1337);
	assert(x == 1337, "Atomic Store has Failed...");

	y = 403;
	_afn_atincW(&y);
	assert(y == 404, "Atomic Increment has failed...");

	z = 1338;
	_afn_atdecW(&z);
	assert(z == 1337, "Atomic Decrement has failed...");

	x = 1337;
	_afn_atcasW(&x, z, y);
	assert(x == y, "Atomic Compare & Exchange has failed...");
}

