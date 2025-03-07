#include "asm/linux-syscall.h"
#include "asm/cpuid.h"
#include "mem/s8.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"
#include "cpu/thread-linux.h"

local void
mmm_sleep(void *args)
{
	struct timespec *ts = (struct timespec *)args;
	sys_nanosleep(ts, 0);
}

local i32
s8_print(s8 *s)
{
	return sys_write(0, s->data, s->len);
}

local i32
fb8_write(fb8 *b)
{
	return sys_write((usz)b->fd, b->data, b->len);
}

int
main(int argc,
     char **argv)
{
	
	StackHead *sh = (StackHead *)((u8 *)mmap_anon(KB(4)) + KB(4) - sizeof(StackHead));
	struct timespec ts;
	ts.tv_sec = 3;
	create_thread(sh, mmm_sleep, &ts);

	// Static s8
	s8 hw = s8("Hello, World!\n");
	s8_print(&hw);

	// Dynamic fb8. Note: hw2->fd == 0, aka stdout.
	os_write = fb8_write;

	fb8 hw2 = {0};
	hw2.cap = KB(4);
	hw2.data = mmap_anon(hw2.cap);

	s8 hs = s8("Waiting for nanosleep futex. . .");
	fb8_append(&hw2, hs);
	fb8_append_lf(&hw2);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	futex_wait(&sh->join_futex);

	hs = s8("Pi: ");	
	float p = 3.14159265;
	fb8_append(&hw2, hs);
	fb8_append_f64(&hw2, p);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Positive size_t Value: ");
	usz uval = 12345678901234567;
	fb8_append(&hw2, hs);
	fb8_append_usz(&hw2, uval);
	fb8_append_lf(&hw2);
	fb8_flush(&hw2);

	hs = s8("Negative size_t Value: ");
	isz ival = -9876543210987654;
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

	sys_munmap(&hw2.data, hw2.cap);
	return 0;
}

void *memset(void *s, int c, size_t n)
{
	// NOTE [32-Bit]: Bad for 32-bit support
	usz csz;

	csz= c | ((usz)c << 32);
	memsetu(s, csz, n);
	return s;
}

