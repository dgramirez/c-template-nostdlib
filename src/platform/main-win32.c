#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	MArena sysmem;
	PlatformData pd = {0};
	void *buffer;

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

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = marena_alloc(&sysmem, pd.bufapp.len, page_size);
	pd.os_write = Win32WriteFile;
	pd.get_cpu_vendor = Win32CpuidGetVendor;
	pd.std_out = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
	pd.run_app = 1;
	pd.logsz = win32_logc;
	pd.logs8 = win32_log;

	app_init(&pd);
	while(pd.run_app) {
		app_update();
		Sleep(1);
	}
	app_close();

	usz *___killme = 0;
	*___killme = 0xDEADA55E;

	unref(argc);
	unref(argv);
	return 0;
}

local void
atomic_test()
{
	int x;
	int y;
	int z;

	atomic_store(&x, 1337);
	assert(x == 1337, "Atomic Store has Failed...");

	y = 403;
	atomic_inc(&y);
	assert(y == 404, "Atomic Increment has failed...");

	z = 1338;
	atomic_dec(&z);
	assert(z == 1337, "Atomic Decrement has failed...");

//	x = 1337;
//	atomic_cmpxchg(&x, z, y);
//	assert(x == y, "Atomic Compare & Exchange has failed...");
}

