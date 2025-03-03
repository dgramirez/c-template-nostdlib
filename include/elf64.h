#ifndef INCLUDE_ELF64_H
#define INCLUDE_ELF64_H

#include "platform.h"
#include "global.h"
#include "asm/linux-syscall.h"

// I finally found a good Elf64 Documentation:
//    - https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf
//
// Additional Docs:
//    - https://refspecs.linuxfoundation.org/elf/index.html

#define ELF64_MAGIC_ENDIAN_L    0x464C457F
#define ELF64_MAGIC_ENDIAN_B    0x7F454C46
#define ELF64_R_SYM(info)       ((info) >> 32)
#define ELF64_R_TYPE(info)      ((info) & 0xFFFFFFFFL)
#define ELF64_R_INFO(sym, type) (((sym) << 32) + ((type) & 0xFFFFFFFFL))

//////////////////////////
// Elf64 Primitive Type //
//////////////////////////
typedef usz Elf64_Addr;
typedef usz Elf64_Off;
typedef u16 Elf64_Half;
typedef u32 Elf64_Word;
typedef i32 Elf64_Sword;
typedef usz Elf64_Xword;
typedef isz Elf64_Sxword;

//////////////////////////
// Helpful Enumerations //
//////////////////////////
typedef enum {
	AT_NULL   = 0,
	AT_IGNORE = 1,
	AT_EXECFD = 2,
	AT_PHDR   = 3,
	AT_PHENT  = 4,
	AT_PHNUM  = 5,
	AT_PAGESZ = 6,
	AT_BASE   = 7,
	AT_FLAGS  = 8,
	AT_ENTRY  = 9
} AuxvLocations;

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

local unsigned long
elf64_hash(const unsigned char *name)
{
	unsigned long h;
	unsigned long g;

	h = 0;
	while (*name) {
		h = (h << 4) + *name++;

		g = h & 0xF0000000;
		if (g)
			h ^= g >> 24;

		h &= 0x0FFFFFFF;
	}

	return h;
}

//////////////////////////////////////////////////////
// GDB - Here until there's a better place for this //
//////////////////////////////////////////////////////
struct link_map {
	Elf64_Addr  l_addr;
	char       *l_name;
	Elf64_Dyn  *l_ld;
	struct link_map *l_next, *l_prev;
};

struct r_debug {
	int r_version;
	struct link_map *r_map;
	Elf64_Addr r_brk;
	enum { RT_CONSISTENT, RT_ADD, RT_DELETE } r_state;
	Elf64_Addr r_ldbase;
};

struct r_debug _r_debug;
struct r_debug *_dl_debug_addr = &_r_debug;

//////////////////////////////
// Shared Library Structure //
//////////////////////////////
typedef struct _DsoNode DsoNode;
typedef struct _Dso {
	u8      *base;
	struct _DsoNode *deps;
	usz      base_len;
	u32      name_hash;
	u32      ref_count;

	Elf64_Dyn  *dyn;
	Elf64_Word *hash_table;
	Elf64_Sym  *sym_table;
	const char *str_table;
} Dso;

typedef struct _DsoNode {
	Dso *dso;
	struct _DsoNode *next;
} DsoNode;

typedef struct {
	struct link_map *l_map;
	struct link_map **l_map_deps;
	usz base_len;
	u32 name_hash, ref_count;

	Elf64_Dyn  *dyn;
	Elf64_Word *hash_table;
	Elf64_Sym  *sym_table;
	const char *str_table;
} Dso2;

global MArena   _gma_dl_load;
global MArena   _gma_dl_temp;
global MPool    _gmp_dl_dso;
global MPool    _gmp_dl_dso_node;
global MPool    _gmp_gdb_linkmap;
global DsoNode *_gdso_tail;
global struct link_map *l_map_tail;

/////////////////
// Main Dl API //
/////////////////
local void
dl_init(u8 *b,
        usz len);

local int
dl_open(Dso *out,
        const char *filepath);

local void *
dl_sym(Dso *lib,
       const char *name);

local void
dl_close(Dso *lib);

//////////////////////
// Helper Functions //
//////////////////////
local int
_dl_open_file_and_validate_elf(Elf64_Ehdr  *ehdr,
                               const char *filepath);

local int
_dl_setup_phdr_and_loads(Dso *lib,
                         Elf64_Phdr **_phdr,
                         Elf64_Half e_phnum,
						 Elf64_Half e_phoff,
                         int fd);

local int
_dyn_setup_relocations(Dso *lib);

local int
_dyn_load_dependencies(Dso *lib);

local int
_dyn_hash_and_validate_existance(Dso *lib,
                                 const char *filepath);

local void
_dyn_setup_dso_structure(Dso *lib);

local int
_dl_open_file_using_env(const char *_filepath);

////////////////////
// DL Definitions //
////////////////////

local void
dl_init(u8 *b,
        usz len)
{
	struct link_map *ldso;
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	usz   phnum;

	assert(align_pad(b, KB(4)) == 0, "Memory should be aligned to 4KB!");
	assert(align_pad(b, KB(4)) == 0, "Size should be aligned to 4KB!");
	assert(len >= KB(64), "Minimum Size for dl_init not met!")

	marena_init(&_gma_dl_load, b, len - KB(16), KB(4));
	marena_init(&_gma_dl_temp, _gma_dl_load.end, KB(8), 8);

	// Note: This will absolutely be re-done as this file gets re-done.
	mpool_init(&_gmp_dl_dso, _gma_dl_temp.end, KB(4), sizeof(Dso), 8);
	mpool_init(&_gmp_gdb_linkmap,
	            _gmp_dl_dso.end,
	            KB(3),
	            sizeof(struct link_map),
	            8);
	mpool_init(&_gmp_dl_dso_node,
	            _gmp_gdb_linkmap.end,
	            KB(1),
	            sizeof(DsoNode),
	            8);
	_gdso_tail = 0;

	if (!auxv) {
		auxv = environ;
		while (*auxv++);
	}

	ehdr  = (Elf64_Ehdr *)GetAuxvValue(AT_BASE);
	phdr  = (Elf64_Phdr *)((Elf64_Addr)ehdr + ehdr->e_phoff);
	phnum = ehdr->e_phnum;
	while (phnum-- && phdr->p_type != PT_DYNAMIC)
		phdr++;

	ldso = (struct link_map *)mpool_alloc(&_gmp_gdb_linkmap);
	ldso->l_addr = (Elf64_Addr)(ehdr);
	ldso->l_ld   = (Elf64_Dyn *)(ldso->l_addr + phdr->p_vaddr);
	ldso->l_name = "template";
	ldso->l_next = 0;
	ldso->l_prev = 0;

	_r_debug.r_version = 1;
	_r_debug.r_map = ldso;
	_r_debug.r_brk = (Elf64_Addr)cpu_mfence;
	_r_debug.r_state = RT_CONSISTENT;
	_r_debug.r_ldbase = ldso->l_addr;
	cpu_mfence();

	l_map_tail = ldso;
}

local void
dl_close(Dso *lib)
{
	sys_munmap(lib->base, lib->base_len);
	memzerou(lib, sizeof(Dso));
}

local void *
dl_sym(Dso *lib,
       const char *name)
{
	uint32_t  hash      =  elf64_hash((const unsigned char *)name);
	uint32_t  nbucket   =  lib->hash_table[0];
	uint32_t *bucket    = &lib->hash_table[2];
	uint32_t *chain     = &lib->hash_table[2 + nbucket];
	uint32_t  sym_index = bucket[hash % nbucket];
	
	while(sym_index) {
		const char *sym_name = &lib->str_table[lib->sym_table[sym_index].st_name];
		if (c_streq(sym_name, name))
			return (void *)(lib->base + lib->sym_table[sym_index].st_value);

		sym_index = chain[sym_index];
	}

	return 0;
}

local int
dl_open(Dso *out, const char *filepath)
{
	Elf64_Ehdr  ehdr;
	Elf64_Phdr *phdr;
	int         fd;
	int         err;

	assert(_gma_dl_load.start && _gma_dl_temp.start && _gmp_dl_dso.start,
	      "The Memory Arenas & Pool for dl are not initialized [dl_init]");

	_r_debug.r_state = RT_ADD;
	cpu_mfence();

	memzerou(out, sizeof(Dso));
	if (_dyn_hash_and_validate_existance(out, filepath))
		return 0;

	fd = _dl_open_file_and_validate_elf(&ehdr, (char *)filepath);
	if (IS_SYSCALL_ERR(fd))
		return fd;

	err = _dl_setup_phdr_and_loads(out,
	                               &phdr,
	                               ehdr.e_phnum,
	                               ehdr.e_phoff,
	                               fd);

	sys_close(fd);
	if (err) {
		if (out->base)
			sys_munmap(out->base, out->base_len);

		memzerou(out, sizeof(Dso));
		return err;
	}

	_dyn_setup_dso_structure(out);
	_dyn_load_dependencies(out);

	struct link_map *map = mpool_alloc(&_gmp_gdb_linkmap);
	map->l_addr = (Elf64_Addr)out->base;
	map->l_name = (char*)filepath;
	map->l_ld   = out->dyn;
	map->l_prev = l_map_tail;
	map->l_next = 0;

	l_map_tail->l_next = map;
	l_map_tail = map;

	_dyn_setup_relocations(out);

	_r_debug.r_state = RT_CONSISTENT;
	cpu_mfence();

	return 0;
}

local int
_dl_open_file_and_validate_elf(Elf64_Ehdr  *ehdr,
                               const char *filepath)
{
	usz written;
	int fd;

	if (!ehdr)
		return -4095;

	fd = sys_openat(AT_FDCWD, filepath, O_RDONLY, 0);
	if (IS_SYSCALL_ERR(fd)) {
		fd = _dl_open_file_using_env(filepath);
		if (IS_SYSCALL_ERR(fd))
			return fd;
	}

	written = sys_read(fd, ehdr, sizeof(Elf64_Ehdr));
	if (written != sizeof(Elf64_Ehdr)) {
		sys_close(fd);
		return -4094;
	}

	if ((ehdr->e_ident_mag != ELF64_MAGIC_ENDIAN_L) &&
		(ehdr->e_ident_mag != ELF64_MAGIC_ENDIAN_B))
	{
		sys_close(fd);
		return -4093;
	}

	if (ehdr->e_type != ET_DYN) {
		sys_close(fd);
		return -4092;
	}

	return fd;
}

local int
_dl_setup_phdr_and_loads(Dso *lib,
                         Elf64_Phdr **_phdr,
                         Elf64_Half e_phnum,
						 Elf64_Half e_phoff,
                         int fd)
{
	Elf64_Phdr *phdr;
	Elf64_Phdr *phdr_tls;
	Elf64_Addr load_min;
	Elf64_Addr load_max;
	Elf64_Off  load_off;
	void *mapped;
	usz written;
	int prot;
	int err;
	Elf64_Half i;

	*_phdr = marena_alloc(&_gma_dl_temp, sizeof(Elf64_Phdr) * e_phnum, 64);
	if (!(*_phdr))
		return -4091;

	phdr = *_phdr;
	sys_lseek(fd, e_phoff, SEEK_SET);
	written = sys_read(fd, phdr, sizeof(Elf64_Phdr) * e_phnum);
	if (written != (sizeof(Elf64_Phdr) * e_phnum))
		return -4090;

	i = e_phnum;
	phdr_tls = 0;
	load_min = 0xFFFFFFFFFFFFFFFF;
	load_max = 0x0;
	load_off = 0;
	while (i--) {
		switch(phdr->p_type) {
			case PT_LOAD: {
				load_max = imax(load_max, phdr->p_vaddr + phdr->p_memsz);
				if (phdr->p_vaddr < load_min) {
					load_min = phdr->p_vaddr;
					load_off = phdr->p_offset;
				}
			} break;

			case PT_DYNAMIC: {
				lib->dyn = (Elf64_Dyn *)phdr->p_vaddr;
			} break;

			case PT_TLS: {
				phdr_tls = phdr;
			} break;
		}

		phdr++;
	}

	load_min       = align_addr(load_min, page_size);
	load_max       = align_addr(load_max + (page_size - 1), page_size);
	lib->base_len  = (load_max - load_min) +  load_off;
	lib->base      = sys_mmap(0,
	                          lib->base_len,
	                          PROT_NONE,
	                          MAP_PRIVATE | MAP_ANONYMOUS,
	                         -1,
	                          0);

	if (IS_SYSCALL_ERR(lib->base)) {
		err = (int)((usz)lib->base & 0xFFFFFFFF);
		lib->base = 0;
		lib->base_len = 0;
		return err;
	}

	i = e_phnum;
	phdr -= i;
	while (i--) {
		if (phdr->p_type == PT_LOAD) {
			prot = 0;
			if (phdr->p_flags & PF_X)
				flag_set(prot, PROT_EXEC);
			if (phdr->p_flags & PF_W)
				flag_set(prot, PROT_WRITE);
			if (phdr->p_flags & PF_R)
				flag_set(prot, PROT_READ);

			load_off = align_addr(phdr->p_offset, page_size);
			load_min = align_addr(phdr->p_vaddr, page_size);
			load_max = align_addr((phdr->p_vaddr + phdr->p_memsz) +
			                      (page_size - 1),
			                       page_size);

			mapped = sys_mmap(lib->base + load_min,
			                  load_max - load_min,
			                  prot,
			                  MAP_PRIVATE | MAP_FIXED,
			                  fd,
			                  load_off);

			if (IS_SYSCALL_ERR(mapped))
				return 0xBAD + i;

			if (phdr->p_memsz > phdr->p_filesz) {
				memzerou(lib->base + phdr->p_vaddr + phdr->p_filesz,
						(phdr->p_memsz - phdr->p_filesz) & (page_size - 1));
			}
		}

		phdr++;
	}

	if (phdr_tls) {
		memzerou(lib->base + phdr_tls->p_vaddr + phdr_tls->p_filesz,
		         (phdr_tls->p_memsz - phdr_tls->p_filesz) & (page_size - 1));
	}

	lib->dyn = (Elf64_Dyn *)(lib->base + (isz)lib->dyn);
	return 0;
}

local int
_dyn_setup_relocations(Dso *lib)
{
	Elf64_Dyn  *dyn;
	Elf64_Rela *rela;
	Elf64_Rela *jmprel;
	Elf64_Xword relasz;
	Elf64_Xword pltrelsz;

	Elf64_Addr (*resolver)();

	void ** got;
	usz *target;
	const char *sym_name;
	Elf64_Sym *sym;
	Elf64_Addr sym_addr;
	Elf64_Word rtype;
	Elf64_Word sym_idx;

	rela = 0;
	relasz = 0;
	jmprel = 0;
	pltrelsz = 0;
	dyn = lib->dyn;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_RELA: {
				rela = (Elf64_Rela *)(lib->base + dyn->d_un.d_ptr);
			} break;

			case DT_RELASZ: {
				relasz = dyn->d_un.d_val / sizeof(Elf64_Rela);
			} break;

			case DT_JMPREL: {
				jmprel = (Elf64_Rela *)(lib->base + dyn->d_un.d_ptr);
			} break;

			case DT_PLTRELSZ: {
				pltrelsz = dyn->d_un.d_val / sizeof(Elf64_Rela);
			} break;
		}

		dyn++;
	}

	while(relasz--) {
		target = (usz*)(lib->base + rela->r_offset);
		rtype  = ELF64_R_TYPE(rela->r_info);
		sym_idx   = ELF64_R_SYM(rela->r_info);

		sym     = &lib->sym_table[sym_idx];
		sym_addr = (Elf64_Addr)(lib->base + sym->st_value);

		switch(rtype) {
			case R_X86_64_64: {
				*target = sym_addr + rela->r_addend;
			} break;

			case R_X86_64_RELATIVE: {
				*target = (usz)(lib->base + rela->r_addend);
			} break;

			case R_X86_64_GLOB_DAT: {
				*target = sym_addr;
			} break;

			case R_X86_64_IRELATIVE: {
				resolver = (Elf64_Addr(*)())(lib->base + rela->r_addend);
				*target = resolver();
			} break;

			default: {
				assert(0, "RType value is not handled!");
			}
		}

		rela++;
	}

	while(pltrelsz--) {
		got       = (void **)(lib->base + jmprel->r_offset);
		rtype     = ELF64_R_TYPE(jmprel->r_info);
		sym_idx   = ELF64_R_SYM(jmprel->r_info);

		sym      = &lib->sym_table[sym_idx];
		sym_name = lib->str_table + sym->st_name;
		sym_addr = (Elf64_Addr)dl_sym(lib, sym_name);

		switch(rtype) {
			case R_X86_64_JUMP_SLOT: {
				*got = (void*)sym_addr;
			} break;

			default: {
				assert(0, "RType value is not handled!");
			}
		}

		jmprel++;
	}

	return 0;
}

local int
_dyn_load_dependencies(Dso *lib)
{
	Elf64_Dyn  *dyn;
	const char *filename;
	Dso *dso;

	dyn = lib->dyn;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_NEEDED: {
				dso = mpool_alloc(&_gmp_dl_dso);
				if (!dso)
					return -1;

				filename = lib->str_table + dyn->d_un.d_val;
				dl_open(dso, filename);
			} break;
		}

		dyn++;
	}

	return 0;
}

local int
_dyn_hash_and_validate_existance(Dso *lib,
                                 const char *filepath)
{
	s8  path;
	s8  file;
	u8 *c;
	DsoNode *node;

	path.data = (u8 *)filepath;
	path.len  = c_strlen(filepath);
	file.data = path.data + path.len - 1;
	file.len  = 0;

	c = s8_rchr(file, '/', path.len - file.len);
	if (c) {
		file.data = c + 1;
		file.len  = (path.data + path.len) - file.data;
	}
	else {
		file.data = path.data;
		file.len  = path.len;
	}

	lib->name_hash = elf64_hash((const unsigned char *)file.data);

	if (!_gdso_tail) {
		node = mpool_alloc(&_gmp_dl_dso_node);
		node->next = 0;
		node->dso = lib;

		_gdso_tail = node;
		return 0;
	}

	node = _gdso_tail;
	while(node) {
		if (node->dso->name_hash == lib->name_hash)
			return lib->name_hash;

		node = node->next;
	}

	node = mpool_alloc(&_gmp_dl_dso_node);
	node->next = _gdso_tail;
	node->dso  = lib;
	_gdso_tail = node;

	return 0;
}

local void
_dyn_setup_dso_structure(Dso *lib)
{
	Elf64_Dyn *dyn;

	dyn = lib->dyn;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_SYMTAB: {
				lib->sym_table = (Elf64_Sym *)(lib->base + dyn->d_un.d_ptr);
			} break;

			case DT_STRTAB: {
				lib->str_table = (const char *)(lib->base + dyn->d_un.d_ptr);
			} break;

			case DT_HASH: {
				lib->hash_table = (Elf64_Word *)(lib->base + dyn->d_un.d_ptr);
			} break;
		}
		dyn++;
	}
}

local int
_dl_open_file_using_env(const char *_filepath)
{
	MArenaTemp temp;
	mb8 file;
	s8 filepath;
	s8 ld_library_path;
	s8 system_libs;
	char **env;
	int fd;

	if (*_filepath == '/')
		return -2;

	// NOTE: We NEED the null terminator, as its will be interpreted as
	//       a c string.
	filepath.len  = c_strlen(_filepath) + 1;
	filepath.data = (u8 *)_filepath;

	env = environ;
	while(*env) {
		ld_library_path.data = (u8 *)*env;
		ld_library_path.len  = c_strlen(*env);

		if (s8_eq(ld_library_path, s8("LD_LIBRARY_PATH"), 15))
			break;

		env++;
	}

	if (!(*env)) {
		ld_library_path.data = 0;
		ld_library_path.len  = 0;
	}

	marena_save(&temp, &_gma_dl_temp);
	mb8_init(&file, marena_alloc(&_gma_dl_temp, 256, 8), 256);

	if (ld_library_path.data) {
		// TODO: LD_LIBRARY_PATH
	}

	// TODO: Lets also read /etc/ld.so.conf and /etc/ld.so.conf.d/*.conf
	//       to find libraries

	system_libs = s8("/lib/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/lib/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/lib64/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/lib64/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/local/lib/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/local/lib64/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/opt/lib/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/opt/lib64/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	// NOTE: This is specific for Debian/Ubuntu
	system_libs = s8("/lib/x86_64-linux-gnu/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	system_libs = s8("/usr/lib/x86_64-linux-gnu/");
	mb8_cpy(&file, system_libs);
	mb8_cat(&file, filepath);
	fd = sys_openat(AT_FDCWD, (char *)file.data, O_RDONLY, 0);
	if (!IS_SYSCALL_ERR(fd))
		return fd;

	marena_load(&temp);
	return -2;
}


#endif // INCLUDE_ELF64_H

