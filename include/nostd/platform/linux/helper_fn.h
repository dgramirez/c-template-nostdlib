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

local i32
fb8_write(fb8 *b)
{
	return sys_write((usz)b->fd, b->data, b->len);
}

local void *
linux_thread_create(void *addr,
                    void *args,
                    usz stack_size)
{
	unref(addr);
	unref(stack_size);
	return (void *)sys_clone(0x50f00, args, 0, 0, 0);
}

local usz
linux_get_freq()
{
	return 1000000000;
}

local usz
linux_get_counter()
{
	struct timespec ts = {0};
	sys_clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * linux_get_freq()) + ts.tv_nsec;
}

#endif // INCLUDE_PLATFORM_LINUX_HELPER_FN_H

