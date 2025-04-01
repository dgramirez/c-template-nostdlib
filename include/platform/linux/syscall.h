#ifndef INCLUDE_PLATFORM_LINUX_SYSCALL_H
#define INCLUDE_PLATFORM_LINUX_SYSCALL_H

#define IS_SYSCALL_ERR(x) (((isz)(x) < 0) && ((isz)(x) > -4096))
#define SYSCALL_ERR_VAL(x) (-(x))

extern ssize_t
sys_read(int fd,
         void *buf,
         size_t count);

extern ssize_t
sys_write(int fd,
          void *buf,
          size_t count);

extern int
sys_close(int fd);

extern off_t
sys_lseek(int fd,
          off_t offset,
          int whence);

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

extern int
sys_rt_sigaction(int sig,
                 struct sigaction_t *act,
                 struct sigaction_t *oact,
                 size_t sigset_t_size);

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
sys_gettimeofday(struct timeval *tv,
                 struct timezone *tz);

extern long
sys_futex(uint32_t *uaddr,
          int op,
          uint32_t val,
          const struct timespec *utime,
          uint32_t *uaddr2,
          uint32_t val3);

extern int
sys_clock_gettime(const __clockid_t clockid,
                  struct timespec *tp);

extern int
sys_openat(int dirfd,
           const char *filename,
           int flags,
           mode_t mode);

// Note: THIS ISNT A SYSCALL, ITS AN ASM CALL!
//       I will find a place for this in a later date!
extern void
cpuid_native(unsigned int *eax,
             unsigned int *ebx,
             unsigned int *ecx,
             unsigned int *edx);

#endif //INCLUDE_PLATFORM_LINUX_SYSCALL_H

