#ifndef INCLUDE_PLATFORM_LINUX_HELPER_FN_H
#define INCLUDE_PLATFORM_LINUX_HELPER_FN_H

static void *
mmap_anon(size_t size)
{
	return sys_mmap(0,
	                size,
	                PROT_READ | PROT_WRITE,
	                MAP_PRIVATE | MAP_ANONYMOUS,
	                -1,
	                0);
}

inline void
munmap_anon(void *buffer,
            size_t size)
{
	sys_munmap(buffer, size);
}

static b8
linux_cpuid_getvendor(u8 *buffer,
                      usz len)
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

local i32
fb8_write(fb8 *b)
{
	return sys_write((usz)b->fd, b->data, b->len);
}

local u32
linux_thread_create(void *addr, void *args, u32 stack_size) {
	unref(addr);
	unref(stack_size);
	return sys_clone(0x50f00, args, 0, 0, 0);
}

#endif // INCLUDE_PLATFORM_LINUX_HELPER_FN_H

