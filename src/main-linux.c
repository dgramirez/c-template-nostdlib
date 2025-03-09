#include "asm/linux-syscall.h"
#include "asm/cpuid.h"
#include "mem/s8.h"
#include "mem/memfn.h"
#include "mem/arena.h"
#include "mem/pool.h"
#include "cpu/thread-linux.h"
#include "elf/dl.h"
#include "global.h"

typedef int (*pfn_increment_ivar)();
pfn_increment_ivar increment_ivar;

typedef void (*pfn_open_window)();
pfn_open_window open_window;

int
main(int argc,
     char **argv)
{
	Solib lapp = {0};
	Solib lapp_x11 = {0};
	s8  current_dir;
	sb8 filepath;

	mem_master.len  = MB(100);
	mem_master.data = mmap_anon(mem_master.len);
	marena_init(&ma_master,
	            mem_master.data,
	            mem_master.len,
	            page_size);

	marena_init(&ma_temp,
	            marena_alloc(&ma_master, MB(40), page_size),
				MB(40),
				page_size);

	current_dir.data = (u8 *)argv[0];
	current_dir.len  = c_strlen(argv[0]);

	while(argv[0][--current_dir.len] != '/');
	current_dir.len++;

	mb8_init(&filepath,
	          marena_alloc(&ma_temp, page_size, page_size),
	          page_size);
	mb8_cpy(&filepath, current_dir);
	mb8_cat(&filepath, s8("libapp.so\0"));

	dl_init(marena_alloc(&ma_master, MB(20), KB(4)), MB(20));
	dlopen(&lapp, (const char *)filepath.data);
	marena_reset(&ma_temp);

	increment_ivar = (pfn_increment_ivar)dl_sym(&lapp, "increment_ivar");
	int x = increment_ivar();
	x = increment_ivar();
	x = increment_ivar();

	filepath.len -= 4;
	mb8_cat(&filepath, s8("_x11.so\0"));
	dlopen(&lapp_x11, (const char *)filepath.data);
	open_window = dl_sym(&lapp_x11, "open_window");
	open_window();

	sys_munmap(mem_master.data, mem_master.len);
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

