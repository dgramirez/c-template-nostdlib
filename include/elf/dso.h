#ifndef INCLUDE_ELF_DSO_H
#define INCLUDE_ELF_DSO_H

#include "elf64.h"

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

global MArena   _gma_tls_data;
global MArena   _gma_dso_load;
global MArena   _gma_dso_temp;
global MPool    _gmp_dso_data;
global MPool    _gmp_dso_node;
global DsoNode *_gdso_tail;

global usz _gdso_tls_modid  = 2;
global usz _gdso_tls_offset = 0x0;

//////////////////////////
// Forward Declarations //
//////////////////////////

local int
dlopen(Solib *out,
       const char *filepath);

local void *
dlsym(Solib *solib,
      const char *name);

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

local void
_dyn_setup_dso_structure(Dso *lib);

local int
_dyn_setup_relocations(Dso *lib);

local int
_dyn_load_dependencies(Dso *lib);

local u32
_dyn_hash_and_validate_existance(const char *filepath);

local int
_dl_open_file_using_env(const char *_filepath);

////////////////////////
// Helper Definitions //
////////////////////////
local u32
_dyn_hash_and_validate_existance(const char *filepath)
{
	s8  path;
	s8  file;
	u8 *c;
	u32 hash;
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

	hash = elf64_hash((const unsigned char *)file.data);

	node = _gdso_tail;
	while(node) {
		if (node->dso->name_hash == hash)
			return 0xDEADF00D;

		node = node->prev;
	}

	return hash;
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
	MArenaTemp temp;
	Elf64_Phdr *phdr;
	Elf64_Phdr *phdr_tls;
	Elf64_Addr load_min;
	Elf64_Addr load_max;
	Elf64_Off  load_off;
	void *lib_base;
	void *mapped;
	usz written;
	int prot;
	int err;
	Elf64_Half i;

	marena_save(&temp, &_gma_dso_temp);

	*_phdr = marena_alloc(&_gma_dso_temp, sizeof(Elf64_Phdr) * e_phnum, 64);
	if (!(*_phdr))
		return -4091;

	phdr = *_phdr;
	sys_lseek(fd, e_phoff, SEEK_SET);
	written = sys_read(fd, phdr, sizeof(Elf64_Phdr) * e_phnum);
	if (written != (sizeof(Elf64_Phdr) * e_phnum)) {
		marena_load(&temp);
		return -4090;
	}

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
				lib->tls_modid = _gdso_tls_modid++;
				lib->tls_len   = phdr->p_memsz;
				lib->tls_align = phdr->p_align;

				marena_alloc(&_gma_tls_data, lib->tls_len, KB(4));
				lib->tls_offset = _gma_tls_data.previous - _gma_tls_data.start;
			} break;
		}

		phdr++;
	}

	load_min           = align_addr(load_min, page_size);
	load_max           = align_addr(load_max + (page_size - 1), page_size);
	lib->base_len      = (load_max - load_min) +  load_off;
	lib_base = sys_mmap(0,
	           lib->base_len,
	           PROT_NONE,
	           MAP_PRIVATE | MAP_ANONYMOUS,
	          -1,
	           0);

	if (IS_SYSCALL_ERR(lib_base)) {
		err = (int)((usz)lib_base & 0xFFFFFFFF);
		lib->lmap.l_addr = 0;
		lib->base_len = 0;

		marena_load(&temp);
		return err;
	}
	lib->lmap.l_addr = (Elf64_Addr)lib_base;

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

			mapped = sys_mmap(lib_base + load_min,
			                  load_max - load_min,
			                  prot,
			                  MAP_PRIVATE | MAP_FIXED,
			                  fd,
			                  load_off);

			if (IS_SYSCALL_ERR(mapped)) {
				marena_load(&temp);
				return 0xBAD + i;
			}

			if (phdr->p_memsz > phdr->p_filesz) {
				memzerou(lib_base + phdr->p_vaddr + phdr->p_filesz,
						(phdr->p_memsz - phdr->p_filesz) & (page_size - 1));
			}
		}

		phdr++;
	}

	if (phdr_tls) {
		memzerou(lib_base + phdr_tls->p_vaddr + phdr_tls->p_filesz,
		         (phdr_tls->p_memsz - phdr_tls->p_filesz) & (page_size - 1));
	}

	lib->dyn = (Elf64_Dyn *)(lib_base + (isz)lib->dyn);
	marena_load(&temp);
	return 0;
}

local void
_dyn_setup_dso_structure(Dso *lib)
{
	Elf64_Dyn *dyn;
	char *base;

	dyn = lib->dyn;
	base = (char *)lib->lmap.l_addr;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_SYMTAB: {
				lib->sym_table = (Elf64_Sym *)(base + dyn->d_un.d_ptr);
			} break;

			case DT_STRTAB: {
				lib->str_table = (const char *)(base + dyn->d_un.d_ptr);
			} break;

			case DT_HASH: {
				lib->hash_table = (Elf64_Word *)(base + dyn->d_un.d_ptr);
			} break;
		}
		dyn++;
	}

	// NOTE: str_table must be filled prior to getting the solib name.
	if (!lib->lmap.l_name) {
		dyn = lib->dyn;
		while (dyn->d_tag && !lib->lmap.l_name) {
			switch(dyn->d_tag) {
				case DT_SONAME: {
					lib->lmap.l_name = (char*)(lib->str_table + dyn->d_un.d_ptr);
				} break;
			}
			dyn++;
		}
	}
}

local void
_dl_get_name_from_filepath(Dso *dso, const char *filepath)
{
	s8 path;
	s8 file;
	u8 *c;
	
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

	dso->lmap.l_name = marena_alloc(&_gma_dso_temp, file.len, 8);
	memcpyu(dso->lmap.l_name, file.data, file.len);
}

local int
_dyn_setup_relocations(Dso *lib)
{
	Elf64_Dyn  *dyn;
	Elf64_Rela *rela;
	Elf64_Rela *jmprel;
	Elf64_Xword relasz;
	Elf64_Xword pltrelsz;
	char *lib_base;
	Solib solib;

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
	lib_base = (char *)lib->lmap.l_addr;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_RELA: {
				rela = (Elf64_Rela *)(lib_base + dyn->d_un.d_ptr);
			} break;

			case DT_RELASZ: {
				relasz = dyn->d_un.d_val / sizeof(Elf64_Rela);
			} break;

			case DT_JMPREL: {
				jmprel = (Elf64_Rela *)(lib_base + dyn->d_un.d_ptr);
			} break;

			case DT_PLTRELSZ: {
				pltrelsz = dyn->d_un.d_val / sizeof(Elf64_Rela);
			} break;
		}

		dyn++;
	}

	while(relasz--) {
		target = (usz*)(lib_base + rela->r_offset);
		rtype  = ELF64_R_TYPE(rela->r_info);
		sym_idx   = ELF64_R_SYM(rela->r_info);

		sym     = &lib->sym_table[sym_idx];
		sym_addr = (Elf64_Addr)(lib_base + sym->st_value);

		switch(rtype) {
			case R_X86_64_64: {
				*target = sym_addr + rela->r_addend;
			} break;

			case R_X86_64_RELATIVE: {
				*target = (usz)(lib_base + rela->r_addend);
			} break;

			case R_X86_64_GLOB_DAT: {
				*target = sym_addr;
			} break;

			case R_X86_64_IRELATIVE: {
				resolver = (Elf64_Addr(*)())(lib_base + rela->r_addend);
				*target = resolver();
			} break;

			case R_X86_64_DTPMOD64: {
				*target = lib->tls_modid;
			} break;

			case R_X86_64_TPOFF64: {
				*target = sym->st_value - lib->tls_offset;
			} break;

			case R_X86_64_GOTTPOFF: {
				*target = sym->st_value + lib->tls_offset - (usz)_gma_tls_data.start;
			} break;

			default: {
				assert(0, "RType value is not handled!");
			}
		}

		rela++;
	}

	solib.dso = lib;
	while(pltrelsz--) {
		got       = (void **)(lib_base + jmprel->r_offset);
		rtype     = ELF64_R_TYPE(jmprel->r_info);
		sym_idx   = ELF64_R_SYM(jmprel->r_info);

		sym      = &lib->sym_table[sym_idx];
		sym_name = lib->str_table + sym->st_name;
		sym_addr = (Elf64_Addr)dlsym(&solib, sym_name);

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
	Dso *dso;
	Solib solib;
	char *l_name;

	dyn = lib->dyn;
	while (dyn->d_tag) {
		switch(dyn->d_tag) {
			case DT_NEEDED: {
				l_name = (char*)(lib->str_table + dyn->d_un.d_val);
				if (_dyn_hash_and_validate_existance(l_name) == 0xBADF00D)
					continue;

				dso = mpool_alloc(&_gmp_dso_data);
				if (!dso)
					return -1;
				dso->lmap.l_name = l_name;


				solib.dso = dso;
				dlopen(&solib, dso->lmap.l_name);
			} break;
		}

		dyn++;
	}

	return 0;
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

	marena_save(&temp, &_gma_dso_temp);
	mb8_init(&file, marena_alloc(&_gma_dso_temp, 256, 8), 256);

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

#endif // INCLUDE_ELF_DSO_H

