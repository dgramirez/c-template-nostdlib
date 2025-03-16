#include "main-win32.h"

local int
Win32Main(int argc, s8 *argv)
{
	s8 hw;
	s8 hs;

	os_write = fb8_write;

	hw = s8("Hello, World!\n");
	s8_print(hw);

	fb8 hw2 = {0};
	hw2.cap = KB(4);
	hw2.data = VirtualAlloc(0, hw2.cap, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	hw2.fd = GetStdHandle(STD_OUTPUT_HANDLE);

	hs = s8("Pi: ");
	float p = 3.14159269;
	fb8_append(&hw2, hs);
	fb8_append_f64(&hw2, p);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Positive size_t Value: ");
	usz uval = WIN32_USZ_VALUE;
	fb8_append(&hw2, hs);
	fb8_append_usz(&hw2, uval);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Negative size_t Value: ");
	isz ival = WIN32_ISZ_VALUE;
	fb8_append(&hw2, hs);
	fb8_append_isz(&hw2, ival);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Hex Value (lower): ");
	usz hval = 0xCAFEBABE;
	fb8_append(&hw2, hs);
	fb8_append_hex(&hw2, hval);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Hex Value (capital): ");
	usz hcval = 0xCAFED00D;
	fb8_append(&hw2, hs);
	fb8_append_hex_cap(&hw2, hcval);
	fb8_append_lf(&hw2);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	// CPUID
	unsigned int eax, ebx, ecx, edx;

	eax = 0;
	cpuid_native(&eax, &ebx, &ecx, &edx);
	
	hs = s8("CPUID Vendor: ");
	fb8_append(&hw2, hs);
	fb8_append_cstr(&hw2, (u8 *)(&ebx), sizeof(unsigned int));
	fb8_append_cstr(&hw2, (u8 *)(&edx), sizeof(unsigned int));
	fb8_append_cstr(&hw2, (u8 *)(&ecx), sizeof(unsigned int));
	fb8_append_lf(&hw2);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	char buftest[256] = {0};

	hs = s8("Current Pointer: ");
	fb8_append(&hw2, hs);
	fb8_append_hex(&hw2, (usz)(&buftest[1]));
	fb8_append_lf(&hw2);

	hs = s8("In-Alignment Pointer for page alignment: ");
	fb8_append(&hw2, hs);
	fb8_append_hex(&hw2, align_addr(&buftest[1], page_size));
	fb8_append_lf(&hw2);

	hs = s8("Next-Alignment Pointer for page alignment: ");
	fb8_append(&hw2, hs);
	fb8_append_hex(&hw2, align_next(&buftest[1], page_size));
	fb8_append_lf(&hw2);

	hs = s8("Pointer value over for page alignment: ");
	fb8_append(&hw2, hs);
	fb8_append_usz(&hw2, align_over(&buftest[1], page_size));
	fb8_append_lf(&hw2);

	hs = s8("Padding value for page alignment: ");
	fb8_append(&hw2, hs);
	fb8_append_usz(&hw2, align_pad(&buftest[1], page_size));
	fb8_append_lf(&hw2);
	fb8_append_lf(&hw2);

	fb8_flush(&hw2);

	buftest[0] = '>';
	buftest[1] = ' ';
	hs = s8("Hello, Master!\n");
	memcpyu_usz(&buftest[2], hs.data, hs.len);
	fb8_append_cstr(&hw2, (u8*)buftest, hs.len + 2);
	fb8_flush(&hw2);

	MArena arena;
	marena_init(&arena, &buftest[2], 256, 8);

	void *mem1 = marena_alloc(&arena, 69, 8);
	mem1 = marena_alloc(&arena, 42, 8);

	for (int i = 0; i < 256; ++i)
		buftest[i] = (i % 94) + 32;

	buftest[0] = '>';
	buftest[1] = ' ';
	buftest[128] = '>';
	buftest[129] = ' ';

	mem1 = marena_realloc(&arena, mem1, 69, 32);
	mem1 = marena_realloc(&arena, mem1, 200 - 6 - 69 - 3, 8);
	marena_reset(&arena);

	MPool pool = {0};
	mpool_init(&pool, buftest, 256, 32, 8);

	for (int i = 0; i < 9; ++i)
		mem1 = mpool_alloc(&pool);

	for (int i = 0; i < 256; ++i)
		buftest[i] = (i % 94) + 32;

	buftest[0] = '>';
	buftest[1] = ' ';
	buftest[128] = '>';
	buftest[129] = ' ';
	memmoveu_usz(&buftest[2], &buftest[130], 126);
	fb8_append_cstr(&hw2, (u8 *)buftest, 256);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	for (int i = 0; i < 256; ++i)
		buftest[i] = (i % 94) + 32;

	buftest[0] = '>';
	buftest[1] = ' ';
	buftest[128] = '>';
	buftest[129] = ' ';
	memmoveu_usz(&buftest[130], &buftest[2], 126);
	fb8_append_cstr(&hw2, (u8 *)buftest, 256);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	unref(argc);
	unref(argv);
	return 0;
}

local i32
s8_print(s8 s)
{
	HANDLE out;
	DWORD written;

	out = GetStdHandle(STD_OUTPUT_HANDLE);
	return WriteFile(out, s.data, (DWORD)s.len, &written, 0);
}

local i32
fb8_write(fb8 *b)
{
	DWORD written;
	return WriteFile(b->fd, b->data, (DWORD)b->len, &written, 0);
}

