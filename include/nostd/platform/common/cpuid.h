#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H

extern void
cpuid_native(unsigned int *eax,
             unsigned int *ebx,
             unsigned int *ecx,
             unsigned int *edx);

extern i32
has_cpuid();

local void
cpuid_exec(void *out,
           u32   cpuid_fn,
           u32   ecx)
{
	CPUIDRegisters *reg;
	if (!out)
		return;

	reg = (CPUIDRegisters *)out;
	reg->eax = cpuid_fn;
	reg->ecx = ecx;
	reg->ebx = reg->edx = 0;
	cpuid_native(&reg->eax, &reg->ebx, &reg->ecx, &reg->edx);
}

local void
cpuid_exec_vendor(CPUIDVendor *out,
                  u32   cpuid_fn)
{
	if (!out)
		return;

	if (cpuid_fn != 0x00000000 && cpuid_fn != 0x80000000)
		return;

	out->eax = cpuid_fn;
	out->ebx = out->edx = out->ecx = 0;
	cpuid_native(&out->eax, &out->ebx, &out->ecx, &out->edx);
}

local i32
cpuid_get_vendor_id(CPUIDVendor *v)
{
	if (!v)
		return -1;

	cpuid_exec_vendor(v, 0x00000000);
	if (v->ebx == 0x68747541 &&
		v->edx == 0x69746E65 &&
		v->ecx == 0x444D4163)
	{
		return CPUID_VENDOR_AuthenticAMD;
	}
	if (v->ebx == 0x756e6547 &&
		v->edx == 0x49656e69 &&
		v->ecx == 0x6c65746e)
	{
		return CPUID_VENDOR_GenuineIntel;
	}

	return CPUID_VENDOR_UNKNOWN;
}

local void
cpuid_fill_amd(CPUID_AuthenticAMD *amd)
{
	CPUID_AuthenticAMD_0x8000001Dh topology_cache = {0};
	i32 count;
	u32 max_fn;

	// CPUID Check
	if (!has_cpuid()) {
		memzerou(amd, sizeof(CPUID_AuthenticAMD));
		return;
	}

	// Standard
	cpuid_exec_vendor((CPUIDVendor *)&amd->vendor, 0x00000000);
	max_fn = imin(amd->vendor.eax, 0x00000010);
	switch(max_fn) {
		case 0x00000010: {
			cpuid_exec(&amd->pqe_capabilities, 0x00000010, 0);
			cpuid_exec(&amd->l3_cache_enforcement, 0x00000010, 1);
		}
		case 0x0000000F: {
			cpuid_exec(&amd->pqm_capabilities, 0x0000000F, 0);
			cpuid_exec(&amd->l3_cache_monitoring, 0x0000000F, 1);
		}
		case 0x0000000D: {
			// NOTE: 0x0000000Dh was skipped
		}
		case 0x0000000C:   // Reserved
		case 0x0000000B: {
			cpuid_exec(&amd->topology_thread, 0x0000000B, 0);
			cpuid_exec(&amd->topology_core, 0x0000000B, 1);
		}
		case 0x0000000A:   // Reserved
		case 0x00000009:   // Reserved
		case 0x00000008:   // Reserved
		case 0x00000007: cpuid_exec(&amd->structured_features, 0x00000007, 0);
		case 0x00000006: cpuid_exec(&amd->power_management, 0x00000006, 0);
		case 0x00000005: cpuid_exec(&amd->monitor_mwait, 0x00000005, 0);
		case 0x00000004:   // Reserved
		case 0x00000003:   // Reserved
		case 0x00000002:   // Reserved
		case 0x00000001: cpuid_exec(&amd->identifier_and_features, 0x00000001, 0);
		case 0x00000000: break;
		default: assert(0, "INTERNAL: max_fn was somehow greater...");
	}

	// Extended
	cpuid_exec_vendor((CPUIDVendor *)&amd->vendor_ext, 0x80000000);
	max_fn = imin(amd->vendor_ext.eax, 0x8000001E);
	switch(max_fn) {
		case 0x8000001E: cpuid_exec(&amd->topology_cpu, 0x8000001E, 0);
		case 0x8000001D: {
			count = 0;
			do {
				cpuid_exec(&topology_cache, 0x8000001D, count);
				count++;
			} while (topology_cache.cache_type != 0);

			amd->topology_cache = mfreelist_alloc(&_sysfl,
			                                      count * sizeof(CPUIDRegisters),
			                                      16);

			if (amd->topology_cache) {
				count = -1;
				do {
					count++;
					cpuid_exec((CPUIDRegisters *)amd->topology_cache + count,
							   0x8000001D,
							   count);
				} while (amd->topology_cache[count].cache_type != 0);
			}
		}
		case 0x8000001C: cpuid_exec(&amd->lwp, 0x8000001C, 0);
		case 0x8000001B: cpuid_exec(&amd->sample_based_capabilities, 0x8000001B, 0);
		case 0x8000001A: cpuid_exec(&amd->instruction_optimizations, 0x8000001A, 0);
		case 0x80000019: cpuid_exec(&amd->tlb_1gb, 0x80000019, 0);
		case 0x80000018:   // Reserved
		case 0x80000017:   // Reserved
		case 0x80000016:   // Reserved
		case 0x80000015:   // Reserved
		case 0x80000014:   // Reserved
		case 0x80000013:   // Reserved
		case 0x80000012:   // Reserved
		case 0x80000011:   // Reserved
		case 0x80000010:   // Reserved
		case 0x8000000F:   // Reserved
		case 0x8000000E:   // Reserved
		case 0x8000000D:   // Reserved
		case 0x8000000C:   // Reserved
		case 0x8000000B:   // Reserved
		case 0x8000000A: cpuid_exec(&amd->svm_features, 0x8000000A, 0);
		case 0x80000009:
		case 0x80000008: cpuid_exec(&amd->cpu_capacity_and_features_ext, 0x80000008, 0);
		case 0x80000007: cpuid_exec(&amd->power_management_ext, 0x80000007, 0);
		case 0x80000006: cpuid_exec(&amd->l2_and_l3, 0x80000006, 0);
		case 0x80000005: cpuid_exec(&amd->l1, 0x80000005, 0);
		case 0x80000004: cpuid_exec(&amd->cpu_name_2_2,0x80000004, 0);
		case 0x80000003: cpuid_exec(&amd->cpu_name_1_2, 0x80000003, 0);
		case 0x80000002: cpuid_exec(&amd->cpu_name_0_2, 0x80000002, 0);
		case 0x80000001: cpuid_exec(&amd->identifier_and_features_ext, 0x80000001, 0);
		case 0x80000000: break;
		default: assert(0, "INTERNAL: max_fn was somehow not correct...");
	}
}

local void
cpuid_setup_amd(CPUID *out) {
	CPUID_AuthenticAMD_0x8000001Dh *topology_cache;

	if (_cpuid.amd->vendor.eax >= 0x00000001) {
		out->lcores = _cpuid.amd->identifier_and_features.lcpu_count;

		out->features.sse    = _cpuid.amd->identifier_and_features.sse;
		out->features.sse2   = _cpuid.amd->identifier_and_features.sse2;
		out->features.sse3   = _cpuid.amd->identifier_and_features.sse3;
		out->features.ssse3  = _cpuid.amd->identifier_and_features.ssse3;
		out->features.sse4_1 = _cpuid.amd->identifier_and_features.sse4_1;
		out->features.sse4_2 = _cpuid.amd->identifier_and_features.sse4_2;
		out->features.mmx    = _cpuid.amd->identifier_and_features.mmx;
		out->features.avx    = _cpuid.amd->identifier_and_features.avx;
		out->features.fma    = _cpuid.amd->identifier_and_features.fma;
		out->features.tsc    = _cpuid.amd->identifier_and_features.tsc;
	}

	if (_cpuid.amd->vendor.eax >= 0x00000007) {
		out->features.avx2 = _cpuid.amd->structured_features.avx2;
	}

	if (_cpuid.amd->vendor_ext.eax >= 0x80000001) {
		out->features.sse4a  = _cpuid.amd->identifier_and_features_ext.sse4a;
		out->features.fma4   = _cpuid.amd->identifier_and_features_ext.fma4;
		out->features.rdtscp = _cpuid.amd->identifier_and_features_ext.rdtscp;
		out->features.rdpmc  = _cpuid.amd->identifier_and_features_ext.perf_tsc;
		out->features.lwp    = _cpuid.amd->identifier_and_features_ext.lwp;
	}

	if (_cpuid.amd->vendor_ext.eax >= 0x80000008) {
		out->pcores = _cpuid.amd->cpu_capacity_and_features_ext.pcores + 1;

//		out->rdpru  = _cpuid.amd->cpu_capacity_and_features_ext.rdpru;
	}

	if (_cpuid.amd->vendor_ext.eax >= 0x8000001E)
		out->pcores /= (_cpuid.amd->topology_cpu.threads_per_compute_unit + 1);

	#define CPUID_TOTAL_BUFFER(tcache)                 \
		((usz)tcache->cache_line_size + 1) *           \
		((usz)tcache->cache_physical_partitions + 1) * \
		((usz)tcache->cache_number_of_ways + 1) *      \
		((usz)tcache->cache_number_ways_set_associative + 1)
	if (_cpuid.amd->vendor_ext.eax >= 0x8000001D) {
		topology_cache = &_cpuid.amd->topology_cache[0];
		while(topology_cache && topology_cache->cache_type != 0) {
			switch(topology_cache->cache_level) {
				case 0: break;
				case 1: {
					if (topology_cache->cache_type == 1)  { // Data
						out->cache_l1d = CPUID_TOTAL_BUFFER(topology_cache);
					}
					else if (topology_cache->cache_type == 2) { // Instruction
						out->cache_l1i = CPUID_TOTAL_BUFFER(topology_cache);
					}
				} break;
				case 2: {
					out->cache_l2 = CPUID_TOTAL_BUFFER(topology_cache);
					
				} break;
				case 3: {
					out->cache_l3 = CPUID_TOTAL_BUFFER(topology_cache);
				} break;
				default: break;
			}

			topology_cache++;
		}
	}
	else {
		if (_cpuid.amd->vendor_ext.eax >= 0x80000005) {
			out->cache_l1d = (usz)_cpuid.amd->l1.l1d_cache_size_in_kb << 10;
			out->cache_l1i = (usz)_cpuid.amd->l1.l1i_cache_size_in_kb << 10;
		}

		if (_cpuid.amd->vendor_ext.eax >= 0x80000006) {
			out->cache_l2 = (usz)_cpuid.amd->l2_and_l3.l2_cache_size_in_kb << 10;
			out->cache_l3 = (usz)_cpuid.amd->l2_and_l3.l3_cache_size_in_kb << 10;
			
		}
	}
	#undef CPUID_TOTAL_BUFFER

}

local void
cpuid_init() {
	CPUIDVendor v;

	_cpuid.has_cpuid = has_cpuid();
	if (!_cpuid.has_cpuid) {
		_cpuid.cpu = 0;
		_cpuid.id = CPUID_VENDOR_ERROR;
		_cpuid.has_cpuid = 0;
		return;
	}

	_cpuid.id = cpuid_get_vendor_id(&v);
	switch(_cpuid.id) {
		case CPUID_VENDOR_AuthenticAMD: {
			_cpuid.cpu = mfreelist_alloc(&_sysfl,
			                             sizeof(CPUID_AuthenticAMD),
			                             16);
			cpuid_fill_amd(_cpuid.amd);
		} break;

		case CPUID_VENDOR_GenuineIntel: {
			
		} break;

		case CPUID_VENDOR_ERROR:
		case CPUID_VENDOR_UNKNOWN:
		default:  {
			_cpuid.cpu = 0;
		}break;
	}
	
}

local void
cpuid_setup(CPUID *out) {
	CPUIDVendor v;
	CPUIDVendorID id;

	if (!has_cpuid())
		return;

	id = cpuid_get_vendor_id(&v);
	switch(id) {
		case CPUID_VENDOR_AuthenticAMD: {
			cpuid_setup_amd(out);
		} break;

		case CPUID_VENDOR_GenuineIntel: {
		} break;

		case CPUID_VENDOR_ERROR:
		case CPUID_VENDOR_UNKNOWN:
		default: break;
	}
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H

