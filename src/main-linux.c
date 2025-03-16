#include "main-linux.h"

int
main(int argc,
     char **argv)
{
	PlatformData pd = {0};
	struct timespec rest = {0};
	fb8 hw2 = {0};
	hw2.cap = KB(4);
	hw2.data = mmap_anon(hw2.cap);

	os_write = fb8_write;

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
	pd.bufapp.data = mmap_anon(pd.bufapp.len);
	pd.os_write = fb8_write;
	pd.get_cpu_vendor = get_cpu_vendor;
	pd.std_out = 0;
	pd.run_app = 1;

	app_init(&pd);
	rest.tv_sec = 1;
	while (pd.run_app) {
		app_update(&pd);
		sys_nanosleep(&rest, 0);
	}
	app_close();

	return 0;
}

local void
mmm_sleep(void *args)
{
	struct timespec *ts = (struct timespec *)args;
	sys_nanosleep(ts, 0);
}

local i32
fb8_write(fb8 *b)
{
	return sys_write((usz)b->fd, b->data, b->len);
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


void *memset(void *s, int c, size_t n)
{
	// NOTE [32-Bit]: Bad for 32-bit support
	usz csz;

	csz= c | ((usz)c << 32);
	memsetu(s, csz, n);
	return s;
}

