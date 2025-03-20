#ifndef SRC_LINUX_SYSCALL_H
#define SRC_LINUX_SYSCALL_H

#include "platform.h"

#ifdef USING_LIBC
	#include <linux/time.h>
	#include <linux/futex.h>
	#include <linux/sched.h>
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <errno.h>
#else
// standard fd arguments
#define	STDIN_FILENO     0  /* Standard input. */
#define	STDOUT_FILENO    1  /* Standard output. */
#define	STDERR_FILENO    2  /* Standard error output. */

// lseek
#define SEEK_SET    0  /* Seek from beginning of file. */
#define SEEK_CUR    1  /* Seek from current position. */
#define SEEK_END    2  /* Seek from end of file. */

// fcntl
#define O_ACCMODE  0003
#define O_RDONLY     00
#define O_WRONLY     01
#define O_RDWR	     02

#define AT_FDCWD                -100  /* Special value used to indicate the *at functions should use the current working directory. */
#define AT_SYMLINK_NOFOLLOW    0x100  /* Do not follow symbolic links. */
#define AT_REMOVEDIR           0x200  /* Remove directory instead of unlinking file. */
#define AT_SYMLINK_FOLLOW      0x400  /* Follow symbolic links. */
#define AT_NO_AUTOMOUNT        0x800  /* Suppress terminal automount traversal. */
#define AT_EMPTY_PATH         0x1000  /* Allow empty relative pathname. */
#define AT_STATX_SYNC_TYPE    0x6000
#define AT_STATX_SYNC_AS_STAT 0x0000
#define AT_STATX_FORCE_SYNC   0x2000
#define AT_STATX_DONT_SYNC    0x4000
#define AT_RECURSIVE          0x8000  /* Apply to the entire subtree.  */
#define AT_EACCESS             0x200  /* Test access permitted for effective IDs, not real IDs.  */

// mman (sys_mmap)
#define MAP_SHARED                0x01  /* Share changes */
#define MAP_PRIVATE               0x02  /* Changes are private */
#define MAP_SHARED_VALIDATE       0x03  /* share + validate extension flags */
#define MAP_TYPE                  0x0f  /* Mask for type of mapping */
#define MAP_FIXED                 0x10  /* Interpret addr exactly */
#define MAP_ANONYMOUS             0x20  /* don't use a file */
#define MAP_POPULATE          0x008000  /* populate (prefault) pagetables */
#define MAP_NONBLOCK          0x010000  /* do not block on IO */
#define MAP_STACK             0x020000  /* give out an address that is best suited for process/thread stacks */
#define MAP_HUGETLB           0x040000  /* create a huge page mapping */
#define MAP_SYNC              0x080000  /* perform synchronous page faults for the mapping */
#define MAP_FIXED_NOREPLACE	  0x100000  /* MAP_FIXED which doesn't unmap underlying mapping */
#define MAP_UNINITIALIZED    0x4000000  /* For anonymous mmap, memory could be */

#define PROT_NONE              0x0  /* page can not be accessed */
#define PROT_READ              0x1  /* page can be read */
#define PROT_WRITE             0x2  /* page can be written */
#define PROT_EXEC              0x4  /* page can be executed */
#define PROT_SEM               0x8  /* page may be used for atomic ops */
#define PROT_GROWSDOWN  0x01000000  /* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP    0x02000000  /* mprotect flag: extend change to end of growsup vma */

// sched.h (sys_clone)
# define CSIGNAL              0x000000ff  /* Signal mask to be sent at exit.  */
# define CLONE_VM             0x00000100  /* Set if VM shared between processes.  */
# define CLONE_FS             0x00000200  /* Set if fs info shared between processes.  */
# define CLONE_FILES          0x00000400  /* Set if open files shared between processes.  */
# define CLONE_SIGHAND        0x00000800  /* Set if signal handlers shared.  */
# define CLONE_PIDFD          0x00001000  /* Set if a pidfd should be placed in parent.  */
# define CLONE_PTRACE         0x00002000  /* Set if tracing continues on the child.  */
# define CLONE_VFORK          0x00004000  /* Set if the parent wants the child to wake it up on mm_release.  */
# define CLONE_PARENT         0x00008000  /* Set if we want to have the same parent as the cloner.  */
# define CLONE_THREAD         0x00010000  /* Set to add to same thread group.  */
# define CLONE_NEWNS          0x00020000  /* Set to create new namespace.  */
# define CLONE_SYSVSEM        0x00040000  /* Set to shared SVID SEM_UNDO semantics.  */
# define CLONE_SETTLS         0x00080000  /* Set TLS info.  */
# define CLONE_PARENT_SETTID  0x00100000  /* Store TID in userlevel buffer before MM copy.  */
# define CLONE_CHILD_CLEARTID 0x00200000  /* Register exit futex and memory location to clear.  */
# define CLONE_DETACHED       0x00400000  /* Create clone detached.  */
# define CLONE_UNTRACED       0x00800000  /* Set if the tracing process can't force CLONE_PTRACE on this clone.  */
# define CLONE_CHILD_SETTID   0x01000000  /* Store TID in userlevel buffer in the child.  */
# define CLONE_NEWCGROUP      0x02000000  /* New cgroup namespace.  */
# define CLONE_NEWUTS         0x04000000  /* New utsname group.  */
# define CLONE_NEWIPC         0x08000000  /* New ipcs.  */
# define CLONE_NEWUSER        0x10000000  /* New user namespace.  */
# define CLONE_NEWPID         0x20000000  /* New pid namespace.  */
# define CLONE_NEWNET         0x40000000  /* New network namespace.  */
# define CLONE_IO             0x80000000  /* Clone I/O context.  */

// linux/futex.h
#define FUTEX_WAIT               0
#define FUTEX_WAKE               1
#define FUTEX_FD                 2
#define FUTEX_REQUEUE            3
#define FUTEX_CMP_REQUEUE        4
#define FUTEX_WAKE_OP            5
#define FUTEX_LOCK_PI            6
#define FUTEX_UNLOCK_PI          7
#define FUTEX_TRYLOCK_PI         8
#define FUTEX_WAIT_BITSET        9
#define FUTEX_WAKE_BITSET       10
#define FUTEX_WAIT_REQUEUE_PI   11
#define FUTEX_CMP_REQUEUE_PI    12
#define FUTEX_LOCK_PI2          13

#define FUTEX_PRIVATE_FLAG      128
#define FUTEX_CLOCK_REALTIME    256
#define FUTEX_CMD_MASK          ~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)

#define FUTEX_WAIT_PRIVATE               (FUTEX_WAIT | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_PRIVATE               (FUTEX_WAKE | FUTEX_PRIVATE_FLAG)
#define FUTEX_REQUEUE_PRIVATE            (FUTEX_REQUEUE | FUTEX_PRIVATE_FLAG)
#define FUTEX_CMP_REQUEUE_PRIVATE        (FUTEX_CMP_REQUEUE | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_OP_PRIVATE            (FUTEX_WAKE_OP | FUTEX_PRIVATE_FLAG)
#define FUTEX_LOCK_PI_PRIVATE            (FUTEX_LOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_LOCK_PI2_PRIVATE           (FUTEX_LOCK_PI2 | FUTEX_PRIVATE_FLAG)
#define FUTEX_UNLOCK_PI_PRIVATE          (FUTEX_UNLOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_TRYLOCK_PI_PRIVATE         (FUTEX_TRYLOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAIT_BITSET_PRIVATE        (FUTEX_WAIT_BITSET | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_BITSET_PRIVATE        (FUTEX_WAKE_BITSET | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAIT_REQUEUE_PI_PRIVATE    (FUTEX_WAIT_REQUEUE_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_CMP_REQUEUE_PI_PRIVATE     (FUTEX_CMP_REQUEUE_PI | FUTEX_PRIVATE_FLAG)

// linux/time.h
#define CLOCK_REALTIME              0
#define CLOCK_MONOTONIC             1
#define CLOCK_PROCESS_CPUTIME_ID    2
#define CLOCK_THREAD_CPUTIME_ID     3
#define CLOCK_MONOTONIC_RAW         4
#define CLOCK_REALTIME_COARSE       5
#define CLOCK_MONOTONIC_COARSE      6
#define CLOCK_BOOTTIME              7
#define CLOCK_REALTIME_ALARM        8
#define CLOCK_BOOTTIME_ALARM        9

/* cloning flags intersect with CSIGNAL so can be used only with unshare and
   clone3 syscalls.  */
#define CLONE_NEWTIME	0x00000080      /* New time namespace */

struct timeval {
	time_t tv_sec;
	time_t tv_usec;
};

struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};

#endif // USING_LIBC

#define IS_SYSCALL_ERR(x) (((isz)(x) < 0) && ((isz)(x) > -4096))
#define SYSCAL_ERR_VAL(x) (-(x))

///////////
//  Auxv //
///////////
extern char **environ;
static char **auxv = 0;

typedef enum {
	AT_NULL              =  0,
	AT_IGNORE            =  1,
	AT_EXECFD            =  2,
	AT_PHDR              =  3,
	AT_PHENT             =  4,
	AT_PHNUM             =  5,
	AT_PAGESZ            =  6,
	AT_BASE              =  7,
	AT_FLAGS             =  8,
	AT_ENTRY             =  9,
	AT_NOTELF            = 10,
	AT_UID               = 11,
	AT_EUID              = 12,
	AT_GID               = 13,
	AT_EGID              = 14,
	AT_PLATFORM          = 15,
	AT_HWCAP             = 16,
	AT_CLKTCK            = 17,
	AT_FPUCW             = 18,
	AT_DCACHEBSIZE       = 19,
	AT_ICACHEBSIZE       = 20,
	AT_UCACHEBSIZE       = 21,
	AT_IGNOREPPC         = 22,
	AT_SECURE            = 23,
	AT_BASE_PLATFORM     = 24,
	AT_RANDOM            = 25,
	AT_HWCAP2            = 26,
	AT_RSEQ_FEATURE_SIZE = 27,
	AT_RSEQ_ALIGN        = 28,
	AT_PAD1              = 29,
	AT_PAD2              = 30,
	AT_EXECFN            = 31,
	AT_SYSINFO           = 32,
	AT_SYSINFO_EHDR      = 33,
	AT_L1I_CACHESHAPE    = 34,
	AT_L1D_CACHESHAPE    = 35,
	AT_L2_CACHESHAPE     = 36,
	AT_L3_CACHESHAPE     = 37,
	AT_PAD3              = 38,
	AT_PAD4              = 39,
	AT_L1I_CACHESIZE     = 40,
	AT_L1I_CACHEGEOMETRY = 41,
	AT_L1D_CACHESIZE     = 42,
	AT_L1D_CACHEGEOMETRY = 43,
	AT_L2_CACHESIZE      = 44,
	AT_L2_CACHEGEOMETRY  = 45,
	AT_L3_CACHESIZE      = 46,
	AT_L3_CACHEGEOMETRY  = 47,

	AT_LAST,
} AuxvLocations;

typedef union {
	void *value[AT_LAST];
	struct {
		char *at_null;
		void *at_ignore;
		usz   at_execfd;
		char *at_phdr;
		usz   at_phent;
		usz   at_phnum;
		usz   at_pagesz;
		char *at_base;
		usz   at_flags;
		char *at_entry;
		usz   at_notelf;
		usz   at_uid;
		usz   at_euid;
		usz   at_gid;
		usz   at_egid;
		char *at_platform;
		usz   at_hwcap;
		usz   at_clktck;
		usz   at_fpucw;
		usz   at_dcachebsize;
		usz   at_icachebsize;
		usz   at_ucachebsize;
		usz   at_ignoreppc;
		usz   at_secure;
		char *at_base_platform;
		char *at_random;
		usz   at_hwcap2;
		usz   at_rseq_feature_size;
		usz   at_rseq_align;
		void *at_pad1;
		void *at_pad2;
		char *at_execfn;
		char *at_sysinfo;
		char *at_sysinfo_ehdr;
		usz   at_l1i_cacheshape;
		usz   at_l1d_cacheshape;
		usz   at_l2_cacheshape;
		usz   at_l3_cacheshape;
		void *at_pad3;
		void *at_pad4;
		usz   at_l1i_cachesize;
		usz   at_l1i_cachegeometry;
		usz   at_l1d_cachesize;
		usz   at_l1d_cachegeometry;
		usz   at_l2_cachesize;
		usz   at_l2_cachegeometry;
		usz   at_l3_cachesize;
		usz   at_l3_cachegeometry;
	};
} AuxV;
static AuxV _g_auxv;

static void
FillGlobalAuxV()
{
	char **av;
	for (int i = 0; i < AT_LAST; ++i)
		_g_auxv.value[i] = 0;

	av = auxv;
	while (av[0]) {
		_g_auxv.value[(usz)av[0]] = av[1];
		av += 2;
	}
}

static void *
GetAuxvValue(usz key)
{
	char **av;

	av = auxv;
	while (av[0]) {
		if ((usz)av[0] == key)
			return (void *)av[1];

		av += 2;
	}

	return (void *)-1;
}

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

extern void
cpu_mfence();

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

#endif // SRC_LINUX_SYSCALL_H

