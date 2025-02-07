#ifndef SRC_LINUX_SYSCALL_H
#define SRC_LINUX_SYSCALL_H

#include "platform.h"
#include <sys/time.h>
#include <unistd.h>


extern ssize_t
sys_write(int fd,
          void *buf,
          size_t count);

extern void *
sys_mmap(void *addr,
         size_t length,
         int prot,
         int flags,
         int fd,
         off_t offset);

extern int
sys_munmap(void *addr,
           size_t length);

extern long
sys_nanosleep(const struct timespec *req,
              struct timespec *rem);

// MAJOR NOTE: This is for x86_64 architecture. [man clone(2) -> Notes]
extern long
sys_clone(unsigned long flags,
          void *stack,
          int *parent_tid,
          int *child_tid,
          unsigned long tls);

extern void
sys_exit(int status);

extern long
sys_futex(uint32_t *uaddr,
          int op,
          uint32_t val,
          const struct timespec *utime,
          uint32_t *uaddr2,
          uint32_t val3);

static void *
mmap_anon(size_t size)
{
	return sys_mmap(0, size, 0x3, 0x22, -1, 0);
}

inline void
munmap_anon(void *buffer,
            size_t size)
{
	sys_munmap(buffer, size);
}

#endif // SRC_LINUX_SYSCALL_H

