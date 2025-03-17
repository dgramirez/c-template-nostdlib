#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	PlatformData pd = {0};
	os_write = fb8_write;

	HANDLE lib = LoadLibraryA("libapp.dll");
	assert(lib, "Failed to load library: libapp.dll");

	deffn_dll(lib, app_init);
	deffn_dll(lib, app_update);
	deffn_dll(lib, app_close);

	assert(app_init,   "Failed to get Fn from libapp.dll: app_init");
	assert(app_update, "Failed to get Fn from libapp.dll: app_update");
	assert(app_close,  "Failed to get Fn from libapp.dll: app_close");

	pd.bufapp.len  = MB(16);
	pd.bufapp.data = VirtualAlloc(0,
	                              pd.bufapp.len,
	                              MEM_COMMIT | MEM_RESERVE,
	                              PAGE_READWRITE);
	pd.os_write = fb8_write;
	pd.get_cpu_vendor = get_cpu_vendor;
	pd.std_out = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
	pd.run_app = 1;

	app_init(&pd);
	while(pd.run_app) {
		app_update();
		Sleep(1);
	}
	app_close();

	unref(argc);
	unref(argv);
	return 0;
}

local i32
fb8_write(fb8 *b)
{
	DWORD written;
	return WriteFile(b->fd, b->data, (DWORD)b->len, &written, 0);
}

local b8
get_cpu_vendor(u8 *buffer, usz len)
{
	b8 b;
	u32 *bptr;

	if (len < 12)
		return s8("");
	b.data = buffer;
	b.len  = 12;

	unsigned int eax, ebx, ecx, edx;
	eax = 0;
	cpuid_native(&eax, &ebx, &ecx, &edx);

	bptr  = (u32 *)&b.data[0];
	*bptr = ebx;

	bptr  = (u32 *)&b.data[4];
	*bptr = edx;

	bptr  = (u32 *)&b.data[8];
	*bptr = ecx;
	if (len >= 16) {
		bptr  = (u32 *)&b.data[12];
		*bptr = 0;
	}

	return b;
}

