#ifndef INCLUDE_PLATFORM_LINUX_DEFINES_H
#define INCLUDE_PLATFORM_LINUX_DEFINES_H

#ifndef USING_LIBC
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

// asm-generic/signal.h
#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
#define	SIGUNUSED	31

#define SA_NOCLDSTOP         0x00000001
#define SA_NOCLDWAIT         0x00000002
#define SA_SIGINFO           0x00000004
#define SA_UNSUPPORTED       0x00000400
#define SA_EXPOSE_TAGBITS    0x00000800
#define SA_RESTORER          0x04000000
#define SA_ONSTACK           0x08000000
#define SA_RESTART           0x10000000
#define SA_NODEFER           0x40000000
#define SA_RESETHAND         0x80000000

#define ELF64_MAGIC_ENDIAN_L    0x464C457F
#define ELF64_MAGIC_ENDIAN_B    0x7F454C46
#define ELF64_R_SYM(info)       ((info) >> 32)
#define ELF64_R_TYPE(info)      ((info) & 0xFFFFFFFFL)
#define ELF64_R_INFO(sym, type) (((sym) << 32) + ((type) & 0xFFFFFFFFL))

#else
	#ifndef SA_RESTORER
		#define SA_RESTORER          0x04000000
	#endif
#endif // USING_LIBC

#endif // INCLUDE_PLATFORM_LINUX_DEFINES_H

