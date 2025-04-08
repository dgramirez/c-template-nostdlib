#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	MArena sysmem;
	PlatformData pd = {0};
	void *buffer;
	AppJobQueue *q;

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
	queue_test(&sysmem);
	q = appjob_init_threadpool(&sysmem, 4, KB(16), KB(4));

	queue_appjob_enqueue(q, test_fn1, 0, 0);
	queue_appjob_enqueue(q, test_fn2, 0, 0);
	queue_appjob_enqueue(q, test_fn3, 0, 0);

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

	// Others
	pd.tlock_terminal = &_glock_terminal;
	pd.std_out        = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
	pd.run_app        = 1;

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
	isz x;
	isz y;
	isz z;

	atomic_store(&x, 1337);
	assert(x == 1337, "Atomic Store has Failed...");

	y = 403;
	atomic_inc(&y);
	assert(y == 404, "Atomic Increment has failed...");

	z = 1338;
	atomic_dec(&z);
	assert(z == 1337, "Atomic Decrement has failed...");

	x = 1337;
	atomic_cas(&x, z, y);
	assert(x == y, "Atomic Compare & Exchange has failed...");
}

local void
queue_test(MArena *sysmem)
{
	MPool p;
	MArenaTemp temp;
	AppJobQueue q;
	u32 f;
	AppJobNode *job;

	marena_save(&temp, sysmem);
	mpool_init(&p,
	            marena_alloc(sysmem, KB(4), 8),
	            KB(4),
	            sizeof(AppJobNode),
	            8);

	queue_appjob_init(&q, &p, &f);

	log_debug(s8("Queue #1"));
	queue_appjob_enqueue(&q, test_fn1, 0, 0);

	log_debug(s8("Queue #2"));
	queue_appjob_enqueue(&q, test_fn2, 0, 0);

	log_debug(s8("Queue #3"));
	queue_appjob_enqueue(&q, test_fn3, 0, 0);

	log_debug(s8("Dequeue #1"));
	queue_appjob_dequeue(&q, &job);
	assert(job->fn == test_fn1, "First Queue is NOT the first item!");

	log_debug(s8("Dequeue #2"));
	queue_appjob_dequeue(&q, &job);
	assert(job->fn == test_fn2, "Second Queue is NOT the second item!");

	log_debug(s8("Dequeue #3"));
	queue_appjob_dequeue(&q, &job);
	assert(job->fn == test_fn3, "Third Queue is NOT the third item!");

	log_debug(s8("Dequeue #4"));
	queue_appjob_dequeue(&q, &job);
	assert(job == 0, "Fourth Queue is NOT 0, or null!");

	marena_load(&temp);
}
