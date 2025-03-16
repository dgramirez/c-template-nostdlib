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

#endif // INCLUDE_ELF64_H

