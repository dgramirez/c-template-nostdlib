#ifndef INCLUDE_PLATFORM_LINUX_TYPE_KERNEL_LIBC_H
#define INCLUDE_PLATFORM_LINUX_TYPE_KERNEL_LIBC_H

#ifdef USING_LIBC
	#include <linux/time.h>
	#include <linux/futex.h>
	#include <linux/sched.h>
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <signal.h>
	#include <unistd.h>
	#include <ucontext.h>
#else
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))

typedef void (*__sighandler_t) (int);

typedef int   pid_t;
typedef pid_t uid_t;
typedef usz   clock_t;
typedef long long int greg_t;
typedef greg_t gregset_t[23];
typedef struct _libc_fpstate *fpregset_t;

struct timeval {
	time_t tv_sec;
	time_t tv_usec;
};

struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};

union sigval
{
	int sival_int;
	void *sival_ptr;
};
typedef union sigval sigval_t;

typedef struct
{
	unsigned long int __val[_SIGSET_NWORDS];
} sigset_t;

struct fpxreg
{
	unsigned short int significand[4];
	unsigned short int exponent;
	unsigned short int reserved1[3];
};

struct xmmreg
{
	u32	element[4];
};

struct _libc_fpstate
{
	/* 64-bit FXSAVE format.  */
	u16 cwd;
	u16 swd;
	u16 ftw;
	u16 fop;
	usz rip;
	usz rdp;
	u32 mxcsr;
	u32 mxcr_mask;
	struct fpxreg _st[8];
	struct xmmreg _xmm[16];
	u32 reserved1[24];
};

typedef struct
{
	gregset_t gregs;
	/* Note that fpregs is a pointer.  */
	fpregset_t fpregs;
	unsigned long long __reserved1 [8];
} mcontext_t;

typedef struct sigaltstack {
	void *ss_sp;
	int ss_flags;
	usz ss_size;
} stack_t;

typedef struct ucontext_t
{
	unsigned long int uc_flags;
	struct ucontext_t *uc_link;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
	sigset_t uc_sigmask;
	struct _libc_fpstate __fpregs_mem;
	unsigned long long int __ssp[4];
} ucontext_t;

typedef struct
{
	int si_signo;
	int si_errno;
	int si_code;
	int __pad0;
	union
	{
		int _pad[28];

		/* kill().  */
		struct {
			pid_t si_pid;	/* Sending process ID.  */
			uid_t si_uid;	/* Real user ID of sending process.  */
		} _kill;

		/* POSIX.1b timers.  */
		struct {
			int si_tid;		/* Timer ID.  */
			int si_overrun;	/* Overrun count.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _timer;

		/* POSIX.1b signals.  */
		struct {
			pid_t si_pid;	/* Sending process ID.  */
			uid_t si_uid;	/* Real user ID of sending process.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _rt;

		/* SIGCHLD.  */
		struct {
			pid_t si_pid;	/* Which child.	 */
			uid_t si_uid;	/* Real user ID of sending process.  */
			int si_status;	/* Exit value or signal.  */
			clock_t si_utime;
			clock_t si_stime;
		} _sigchld;

		/* SIGILL, SIGFPE, SIGSEGV, SIGBUS.  */
		struct {
			void *si_addr;	    /* Faulting insn/memory ref.  */
			short int si_addr_lsb;  /* Valid LSB of the reported address.  */
			union {
				/* used when si_code=SEGV_BNDERR */
				struct {
					void *_lower;
					void *_upper;
				} _addr_bnd;
				/* used when si_code=SEGV_PKUERR */
				u32 _pkey;
			} _bounds;
		} _sigfault;

		/* SIGPOLL.  */
		struct {
			long int si_band;	/* Band event for SIGPOLL.  */
			int si_fd;
		} _sigpoll;

		/* SIGSYS.  */
		struct
		{
			void *_call_addr;	/* Calling user insn.  */
			int _syscall;	/* Triggering system call number.  */
			unsigned int _arch; /* AUDIT_ARCH_* of syscall.  */
		} _sigsys;
	} _sifields;
} siginfo_t;

////////////////////////////
// Elf64 Types & Typedefs //
////////////////////////////
#if EXE_ARCH == 64
	typedef usz Elf64_Addr;
	typedef usz Elf64_Off;
	typedef u16 Elf64_Half;
	typedef u32 Elf64_Word;
	typedef i32 Elf64_Sword;
	typedef usz Elf64_Xword;
	typedef isz Elf64_Sxword;

	typedef enum {
		EI_MAG0             = 0,
		EI_MAG1             = 1,
		EI_MAG2             = 2,
		EI_MAG3             = 3,
		EI_CLASS            = 4,
		EI_DATA             = 5,
		EI_VERSION          = 6,
		EI_OSABI            = 7,
		EI_OSABIVERSION     = 8,

		ELFCLASS32          = 0x01,
		ELFCLASS64          = 0x02,

		ELFDATA2LSB         = 0x01,
		ELFDATA2MSB         = 0x02,

		ELFOSABI_SYSV       = 0x00,
		ELFOSABI_HPUX       = 0x01,
		ELFOSABI_STANDALONE = 0xFF,
	} El_eident;

	typedef enum {
		ET_NONE   = 0x00,
		ET_REL    = 0x01,
		ET_EXEC   = 0x02,
		ET_DYN    = 0x03,
		ET_CORE   = 0x04,
		ET_LOOS   = 0xFE00,
		ET_HIOS   = 0xFEFF,
		ET_LOPROC = 0xFF00,
		ET_HIPROC = 0xFFFF,
	} Elf64_etype;

	typedef enum {
		SHN_UNDEF  = 0,
		SHN_LOPROC = 0xFF00,
		SHN_HIPROC = 0xFF1F,
		SHN_LOOS   = 0xFF20,
		SHN_HIOS   = 0xFF3F,
		SHN_ABS    = 0xFFF1,
		SHN_COMMON = 0xFFF2,
	} Elf64_ssi;

	typedef enum {
		SHT_NULL     = 0x00,
		SHT_PROGBITS = 0x01,
		SHT_SYMTAB   = 0x02,
		SHT_STRTAB   = 0x03,
		SHT_RELA     = 0x04,
		SHT_HASH     = 0x05,
		SHT_DYNAMIC  = 0x06,
		SHT_NOTE     = 0x07,
		SHT_NOBITS   = 0x08,
		SHT_REL      = 0x09,
		SHT_SHLIB    = 0x0A,
		SHT_DYNSYM   = 0x0B,

		SHT_LOOS     = 0x60000000,
		SHT_HIOS     = 0x6FFFFFFF,
		SHT_LOPROC   = 0x70000000,
		SHT_HIPROC   = 0x7FFFFFFF,
	} Elf64_shtype;

	typedef enum {
		SHF_WRITE     = 0x01,
		SHF_ALLOC     = 0x02,
		SHF_EXECINSTR = 0x04,

		SHF_MASKOS    = 0x0F000000,
		SHF_MASKPROC  = 0xF0000000,
	} Elf64_shflags;

	typedef enum {
		STB_LOCAL  = 0x0,
		STB_GLOBAL = 0x1,
		STB_WEAK   = 0x2,
		STB_LOOS   = 0xA,
		STB_HIOS   = 0xC,
		STB_LOPROC = 0xD,
		STB_HIPROC = 0xF
	} Elf64_symbind;

	typedef enum {
		STT_NOTYPE  = 0x0,
		STT_OBJECT  = 0x1,
		STT_FUNC    = 0x2,
		STT_SECTION = 0x3,
		STT_FILE    = 0x4,
		STT_LOOS    = 0xA,
		STT_HIOS    = 0xC,
		STT_LOPROC  = 0xD,
		STT_HIPROC  = 0xF
	} Elf64_symtypes;

	typedef enum {
		PT_NULL    = 0x00,
		PT_LOAD    = 0x01,
		PT_DYNAMIC = 0x02,
		PT_INTERP  = 0x03,
		PT_NOTE    = 0x04,
		PT_SHLIB   = 0x05,
		PT_PHDR    = 0x06,
		PT_TLS     = 0x07,
		PT_NUM     = 0x08,

		PT_LOOS    = 0x60000000,
		PT_HIOS    = 0x6FFFFFFF,
		PT_LOPROC  = 0x70000000,
		PT_HIPROC  = 0x7FFFFFFF,
	} Elf64_ptype;

	typedef enum {
		PF_X        = 0x01,
		PF_W        = 0x02,
		PF_R        = 0x04,
		PF_MASKOK   = 0x00FF0000,
		PF_MASKPROC = 0xFF000000,
	} Elf64_pflags;

	typedef enum {
		DT_NULL         = 0,
		DT_NEEDED       = 1,
		DT_PLTRELSZ     = 2,
		DT_PLTGOT       = 3,
		DT_HASH         = 4,
		DT_STRTAB       = 5,
		DT_SYMTAB       = 6,
		DT_RELA         = 7,
		DT_RELASZ       = 8,
		DT_RELAENT      = 9,
		DT_STRSZ        = 10,
		DT_SYMENT       = 11,
		DT_INIT         = 12,
		DT_FINI         = 13,
		DT_SONAME       = 14,
		DT_RPATH        = 15,
		DT_SYMBOLIC     = 16,
		DT_REL          = 17,
		DT_RELSZ        = 18,
		DT_RELENT       = 19,
		DT_PLTREL       = 20,
		DT_DEBUG        = 21,
		DT_TEXTREL      = 22,
		DT_JMPREL       = 23,
		DT_BIND_NOW     = 24,
		DT_INIT_ARRAY   = 25,
		DT_FINI_ARRAY   = 26,
		DT_INIT_ARRAYSZ = 27,
		DT_FINI_ARRAYSZ = 28,
		DT_LOOS         = 0x60000000,
		DT_HIOS         = 0x6FFFFFFF,
		DT_LOPROC       = 0x70000000,
		DT_HIPROC       = 0x7FFFFFFF,
	} Elf64_dyntable;

	typedef enum {
		R_X86_64_NONE            = 0,
		R_X86_64_64              = 1,
		R_X86_64_PC32            = 2,
		R_X86_64_GOT32           = 3,
		R_X86_64_PLT32           = 4,
		R_X86_64_COPY            = 5,
		R_X86_64_GLOB_DAT        = 6,
		R_X86_64_JUMP_SLOT       = 7,
		R_X86_64_RELATIVE        = 8,
		R_X86_64_GOTPCREL        = 9,
		R_X86_64_32              = 10,
		R_X86_64_32S             = 11,
		R_X86_64_16              = 12,
		R_X86_64_PC16            = 13,
		R_X86_64_8               = 14,
		R_X86_64_PC8             = 15,
		R_X86_64_DTPMOD64        = 16,
		R_X86_64_DTPOFF64        = 17,
		R_X86_64_TPOFF64         = 18,
		R_X86_64_TLSGD           = 19,
		R_X86_64_TLSLD           = 20,
		R_X86_64_DTPOFF32        = 21,
		R_X86_64_GOTTPOFF        = 22,
		R_X86_64_TPOFF32         = 23,
		R_X86_64_PC64            = 24,
		R_X86_64_GOTOFF64        = 25,
		R_X86_64_GOTPC32         = 26,
		R_X86_64_SIZE32          = 32,
		R_X86_64_SIZE64          = 33,
		R_X86_64_GOTPC32_TLSDESC = 34,
		R_X86_64_TLSDESC_CALL    = 35,
		R_X86_64_TLSDESC         = 36,
		R_X86_64_IRELATIVE       = 37,
	} Elf64_relatype;

	//////////////////////
	// ELF64 Structures //
	//////////////////////
	typedef struct
	{
		union {
			unsigned char e_ident[16];  /* ELF identification */
			struct {
				Elf64_Word    e_ident_mag;
				unsigned char e_ident_class;
				unsigned char e_ident_data;
				unsigned char e_ident_version;
				unsigned char e_ident_osabi;
				unsigned char e_ident_abiversion;
				unsigned char e_ident_ospad[7];
			};
		};

		Elf64_Half e_type;          /* Object file type */
		Elf64_Half e_machine;       /* Machine type */
		Elf64_Word e_version;       /* Object file version */
		Elf64_Addr e_entry;         /* Entry point address */
		Elf64_Off  e_phoff;         /* Program header offset */
		Elf64_Off  e_shoff;         /* Section header offset */
		Elf64_Word e_flags;         /* Processor-specific flags */
		Elf64_Half e_ehsize;        /* ELF header size */
		Elf64_Half e_phentsize;     /* Size of program header entry */
		Elf64_Half e_phnum;         /* Number of program header entries */
		Elf64_Half e_shentsize;     /* Size of section header entry */
		Elf64_Half e_shnum;         /* Number of section header entries */
		Elf64_Half e_shstrndx;      /* Section name string table index */
	} Elf64_Ehdr;

	typedef struct
	{
		Elf64_Word  p_type;    /* Type of segment */
		Elf64_Word  p_flags;   /* Segment attributes */
		Elf64_Off   p_offset;  /* Offset in file */
		Elf64_Addr  p_vaddr;   /* Virtual address in memory */
		Elf64_Addr  p_paddr;   /* Reserved */
		Elf64_Xword p_filesz;  /* Size of segment in file */
		Elf64_Xword p_memsz;   /* Size of segment in memory */
		Elf64_Xword p_align;   /* Alignment of segment */
	} Elf64_Phdr;

	typedef struct
	{
		Elf64_Word  sh_name;       /* Section name */
		Elf64_Word  sh_type;       /* Section type */
		Elf64_Xword sh_flags;      /* Section attributes */
		Elf64_Addr  sh_addr;       /* Virtual address in memory */
		Elf64_Off   sh_offset;     /* Offset in file */
		Elf64_Xword sh_size;       /* Size of section */
		Elf64_Word  sh_link;       /* Link to other section */
		Elf64_Word  sh_info;       /* Miscellaneous information */
		Elf64_Xword sh_addralign;  /* Address alignment boundary */
		Elf64_Xword sh_entsize;    /* Size of entries, if section has table */
	} Elf64_Shdr;

	typedef struct
	{
		Elf64_Word    st_name;   /* Symbol name */
		unsigned char st_info;   /* Type and Binding attributes */
		unsigned char st_other;  /* Reserved */
		Elf64_Half    st_shndx;  /* Section table index */
		Elf64_Addr    st_value;  /* Symbol value */
		Elf64_Xword   st_size;   /* Size of object (e.g., common) */
	} Elf64_Sym;

	typedef struct
	{
		Elf64_Addr  r_offset;  /* Address of reference */
		Elf64_Xword r_info;    /* Symbol index and type of relocation */
	} Elf64_Rel;

	typedef struct
	{
		Elf64_Addr   r_offset;  /* Address of reference */
		Elf64_Xword  r_info;    /* Symbol index and type of relocation */
		Elf64_Sxword r_addend;  /* Constant part of expression */
	} Elf64_Rela;

	typedef struct
	{
		Elf64_Sxword d_tag;
		union {
			Elf64_Xword d_val;
			Elf64_Addr d_ptr;
		} d_un;
	} Elf64_Dyn;
#else
	#error i386 / x86 is not setup!
#endif // EXE_ARCH == 64

//////////////////////////////
// Shared Library Structure //
//////////////////////////////
typedef struct link_map {
	Elf64_Addr  l_addr;
	char       *l_name;
	Elf64_Dyn  *l_ld;
	struct link_map *l_next, *l_prev;
} LinkMap;

typedef struct _DsoNode DsoNode;
typedef struct {
	LinkMap lmap;
	DsoNode ** dep_list;
	usz base_len;
	u32 name_hash, ref_count;

	Elf64_Dyn  *dyn;
	Elf64_Word *hash_table;
	Elf64_Sym  *sym_table;
	const char *str_table;

	usz tls_len;
	usz tls_align;
	usz tls_modid;
	usz tls_offset;
} Dso;

typedef struct {
	Dso *dso;
} Solib, Dynlib, Shlib;

typedef struct _DsoNode {
	Dso *dso;
	struct _DsoNode *next;
	struct _DsoNode *prev;
} DsoNode;

//////////
// Auxv //
//////////
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

#endif // USING_LIBC

////////////
// Common //
////////////
struct sigaction_t {
	union {
		void (*handler_fn)(int);
		void (*sigaction_fn)(int, siginfo_t *, void*);
	};
	int sa_flags;
	void (*restorer_fn)(void);
	sigset_t sa_mask;
};

typedef enum {
	REG_R8 = 0,
	REG_R9,
	REG_R10,
	REG_R11,
	REG_R12,
	REG_R13,
	REG_R14,
	REG_R15,
	REG_RDI,
	REG_RSI,
	REG_RBP,
	REG_RBX,
	REG_RDX,
	REG_RAX,
	REG_RCX,
	REG_RSP,
	REG_RIP,
	REG_EFL,
	REG_CSGSFS,		/* Actually short cs, gs, fs, __pad0.  */
	REG_ERR,
	REG_TRAPNO,
	REG_OLDMASK,
	REG_CR2
} GeneralRegisters;

#endif // INCLUDE_PLATFORM_LINUX_TYPE_KERNEL_LIBC_H

