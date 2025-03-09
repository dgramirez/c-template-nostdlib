#ifndef INCLUDE_ELF_DL_H
#define INCLUDE_ELF_DL_H

#include "dso.h"

/////////////////
// Main Dl API //
/////////////////
local void
dl_init(u8 *b,
        usz len);

local int
dlopen(Solib *out,
       const char *filepath);

local void *
dl_sym(Solib *solib,
       const char *name);

local void
dlclose(Solib *lib);

////////////////////
// DL Definitions //
////////////////////
local void
dl_init(u8 *b,
        usz len)
{
	Dso *ldso;
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	usz   phnum;

	assert(align_pad(b, KB(4)) == 0, "Memory should be aligned to 4KB!");
	assert(align_pad(b, KB(4)) == 0, "Size should be aligned to 4KB!");
	assert(len >= KB(128), "Minimum Size for dl_init not met!")

	marena_init(&_gma_dso_load, b, len - KB(32), KB(4));
	marena_init(&_gma_tls_data, _gma_dso_load.end, KB(16), KB(4));
	marena_init(&_gma_dso_temp, _gma_tls_data.end, KB(8), 8);
	mpool_init( &_gmp_dso_data, _gma_dso_temp.end, KB(4), sizeof(Dso), 8);
	mpool_init( &_gmp_dso_node, _gmp_dso_data.end, KB(4), sizeof(DsoNode), 8);
	_gdso_tail = 0;

	if (!auxv) {
		auxv = environ;
		while (*auxv++);

		memzerou(&_g_auxv, sizeof(AuxV));
		FillGlobalAuxV();
	}

	ehdr  = (Elf64_Ehdr *)_g_auxv.at_base;
	phdr  = (Elf64_Phdr *)((Elf64_Addr)ehdr + ehdr->e_phoff);
	phnum = ehdr->e_phnum;
	while (phnum-- && phdr->p_type != PT_DYNAMIC)
		phdr++;

	ldso            = (Dso *)mpool_alloc(&_gmp_dso_data);
	ldso->name_hash = elf64_hash((const unsigned char *)"ld.so");
	ldso->dyn       = (Elf64_Dyn *)((char*)ehdr + phdr->p_vaddr);
	ldso->dep_list  = 0;
	ldso->base_len  = 0;

	ldso->lmap.l_addr = (Elf64_Addr)(ehdr);
	ldso->lmap.l_ld   = ldso->dyn;
	ldso->lmap.l_name = "ld.so";
	ldso->lmap.l_next = 0;
	ldso->lmap.l_prev = 0;

	_gdso_tail = (DsoNode *)mpool_alloc(&_gmp_dso_node);
	_gdso_tail->dso  = ldso;
	_gdso_tail->next = 0;
	_gdso_tail->prev = 0;
}

local void
dlclose(Solib *lib)
{
	char *lib_base = (char *)lib->dso->lmap.l_addr;
	sys_munmap(lib_base, lib->dso->base_len);
	memzerou(lib, sizeof(Dso));
}

local void *
dl_sym(Solib *solib,
       const char *name)
{
	Dso *lib = solib->dso;
	uint32_t  hash      =  elf64_hash((const unsigned char *)name);
	uint32_t  nbucket   =  lib->hash_table[0];
	uint32_t *bucket    = &lib->hash_table[2];
	uint32_t *chain     = &lib->hash_table[2 + nbucket];
	uint32_t  sym_index = bucket[hash % nbucket];
	char     *lib_base  = (char *)lib->lmap.l_addr;
	while(sym_index) {
		const char *sym_name = &lib->str_table[lib->sym_table[sym_index].st_name];
		if (c_streq(sym_name, name))
			return (void *)(lib_base + lib->sym_table[sym_index].st_value);

		sym_index = chain[sym_index];
	}

	return 0;
}

local int
dlopen(Solib *out,
       const char *filepath)
{
	Dso         dso = {0};
	Elf64_Ehdr  ehdr = {0};
	Elf64_Phdr *phdr;
	Dso        *real_dso;
	DsoNode    *node;
	int         fd;
	int         err;
	u32         hash;

	assert(_gdso_tail, "Please initialize dl using dl_init function!");

	hash = _dyn_hash_and_validate_existance(filepath);
	if (hash == 0xBADDF00D) //NOTE: Magic inside hash & validate function!
	    return 0;           //      Kinda Wild if a legit solib matches!

	fd = _dl_open_file_and_validate_elf(&ehdr, (char *)filepath);
	if (IS_SYSCALL_ERR(fd))
		return fd;

	if (!out->dso)
		out->dso = &dso;

	err = _dl_setup_phdr_and_loads(out->dso,
	                               &phdr,
	                               ehdr.e_phnum,
	                               ehdr.e_phoff,
	                               fd);

	sys_close(fd);
	if (err) {
		if (out->dso->lmap.l_addr)
			sys_munmap((void*)out->dso->lmap.l_addr, out->dso->base_len);

		memzerou(out->dso, sizeof(Dso));
		return err;
	}

	if (out->dso == &dso) {
		real_dso = (Dso *)mpool_alloc(&_gmp_dso_data);
		memcpyu(real_dso, &dso, sizeof(Dso));
		out->dso = real_dso;
	}

	node = (DsoNode *)mpool_alloc(&_gmp_dso_node);
	node->dso  = out->dso;
	node->prev = _gdso_tail;
	node->next = 0;

	_gdso_tail->next = node;
	_gdso_tail = node;

	_dyn_setup_dso_structure(out->dso);
	if (!out->dso->lmap.l_name)
		_dl_get_name_from_filepath(out->dso, filepath);
	out->dso->name_hash = hash;

	_dyn_load_dependencies(out->dso);
	_dyn_setup_relocations(out->dso);

	return 0;
}

#endif // INCLUDE_ELF_DL_H

