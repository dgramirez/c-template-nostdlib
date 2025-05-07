#ifndef INCLUDE_NOSTD_COMMON_CPUID_INTEL_H
#define INCLUDE_NOSTD_COMMON_CPUID_INTEL_H

// MAJOR NOTE:
//     In order to truly understand this, you need to be side-by-side with
//     the Intel Manual. The Intel Manual that is in current use is:
//         - Intel 64 and IA-32 Architectures Software Developer's Manual
//         - Combined Volumes: 1, 2A, 2B, 2C, 2D, 3A, 3B, 3C, 3D and 4
//         - Order Number: 253665-086US
//         - Combined Order Numbers: 253665; 325383; 325384; 335592.
//         - Date: December 2024
//
//     Updated manuals should be fine as long as the appropriate pages
//     are found. Here is the starting page number for the CPUID function
//     with the Intel Manual provided above:
//         - Pdf  Pg 814
//         - Phys Pg  3-222 (CPUID - CPU Identification)

// MAJOR NOTE:
//     Updating this will also require you to update this file:
//         - include/nostd/platform/common/cpuid.h
//
//     Without the update, the new CPUID items will not be detected.

// Notes: Reserved Functions:
//            - 0x40000000h - 0x400000FFh [Hypervisor Use]
//            - 0x80000005h

//////////////////////////////////
// GenuineIntel Basic Functions //
// 0x00000001 - 0x00000024h     //
//////////////////////////////////
typedef union {
	char vendor[16];
	struct {
		u32 ebx, edx, ecx, eax;
	};
	struct {
		u32 reserved_0, reserved_1, reserved_2, max_fn;
	};
} CPUID_GenuineIntel_0x00000000h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 stepping    : 4;
			u32 base_model  : 4;
			u32 base_family : 4;
			u32 cpu_type    : 2;
			u32 reserved_0  : 2;
			u32 ext_model   : 4;
			u32 ext_family  : 8;
			u32 reserved_1  : 4;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 brand_id_8bit   : 8;
			u32 clflush_size    : 8;
			u32 lcpu_count      : 8;
			u32 initial_apic_id : 8;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 sse3                   : 1; u32 pclmulqdq              : 1;
			u32 dtes64                 : 1; u32 monitor                : 1;
			u32 ds_cpl                 : 1; u32 vmx                    : 1;
			u32 smx                    : 1; u32 eist                   : 1;
			u32 tm2                    : 1; u32 ssse3                  : 1;
			u32 cnxt_id                : 1; u32 sdbg                   : 1;
			u32 fma                    : 1; u32 cmpxchg16b             : 1;
			u32 xtpr_update_ctrl       : 1; u32 pdcm                   : 1;
			u32 reserved_2             : 1; u32 pcid                   : 1;
			u32 dca                    : 1; u32 sse4_1                 : 1;
			u32 sse4_2                 : 1; u32 x2apic                 : 1;
			u32 movbe                  : 1; u32 popcnt                 : 1;
			u32 tsc_deadline           : 1; u32 aes                    : 1;
			u32 xsave                  : 1; u32 osxsave                : 1;
			u32 avx                    : 1; u32 f16c                   : 1;
			u32 rdrand                 : 1; u32 not_used               : 1;
		};
	};

	union {
		u32 edx;
		struct {
			u32 fpu                    : 1; u32 vme                    : 1;
			u32 de                     : 1; u32 pse                    : 1;
			u32 tsc                    : 1; u32 msr                    : 1;
			u32 pae                    : 1; u32 mce                    : 1;
			u32 cmpxchg8b              : 1; u32 apic                   : 1;
			u32 reserved_3             : 1; u32 sysenter_sysexit       : 1;
			u32 mtrr                   : 1; u32 pge                    : 1;
			u32 mca                    : 1; u32 cmov                   : 1;
			u32 pat                    : 1; u32 pse36                  : 1;
			u32 psn                    : 1; u32 clfsh                  : 1;
			u32 reserved_4             : 1; u32 ds                     : 1;
			u32 acpi                   : 1; u32 mmx                    : 1;
			u32 fxsr                   : 1; u32 sse                    : 1;
			u32 sse2                   : 1; u32 ss                     : 1;
			u32 htt                    : 1; u32 tm                     : 1;
			u32 reserved_5             : 1; u32 pbe                    : 1;
		};
	};

} CPUID_GenuineIntel_0x00000001h;

typedef struct {
	u32 eax, ebx, ecx, edx;
} CPUID_GenuineIntel_0x00000002h;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		u32 reserved_1;
	};

	union {
		u32 ecx;
		u32 serial_1;
	};

	union {
		u32 edx;
		u32 serial_2;
	};
} CPUID_GenuineIntel_0x00000003h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 cache_type             :  5;
			u32 cache_level            :  3;
			u32 self_initialization    :  1;
			u32 fully_associative      :  1;
			u32 reserved_0             :  4;
			u32 lcpu_cache_share_count : 12;
			u32 pcpu_max_ids           :  6;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 cache_line_size           : 12;
			u32 cache_physical_partitions : 10;
			u32 cache_number_of_ways      : 10;
		};
	};

	union {
		u32 ecx;
		u32 cache_number_ways_set_associative;
	};

	union {
		u32 edx;
		struct { 
			u32 wbinvd                 :  1;
			u32 cache_inclusive        :  1;
			u32 complex_cache_indexing :  1;
			u32 reserved_2             : 29;
		};
	};

} CPUID_GenuineIntel_0x00000004h;

typedef struct {
	union {
		u32 eax;
		u32 max_sub_fn;
		// MAJOR NOTE: Sub-leaf 1 & 2 not implemented
	};

	union {
		u32 ebx;
		struct {
			u32 fsgsbase               : 1; u32 ia32_tsc_adjust_msr    : 1;
			u32 sgx                    : 1; u32 bmi_1                  : 1;
			u32 hle                    : 1; u32 avx2                   : 1;
			u32 fdp_excptn_only        : 1; u32 smep                   : 1;
			u32 bmi_2                  : 1; u32 enhanced_rep           : 1;
			u32 invpcid                : 1; u32 rtm                    : 1;
			u32 rdt_m                  : 1; u32 fpu_cs_ds_deprecates   : 1;
			u32 mpx                    : 1; u32 rdt_a                  : 1;
			u32 avx512f                : 1; u32 avx512dq               : 1;
			u32 rseed                  : 1; u32 adx                    : 1;
			u32 smap                   : 1; u32 avx512_ifma            : 1;
			u32 reserved_0             : 1; u32 clflushopt             : 1;
			u32 clwb                   : 1; u32 intel_cpu_trace        : 1;
			u32 avx512pf               : 1; u32 avx512er               : 1;
			u32 avx512cd               : 1; u32 sha                    : 1;
			u32 avx512bw               : 1; u32 avx512vl               : 1;
		};
		// MAJOR NOTE: Sub-leaf 1 & 2 not implemented
	};

	union {
		u32 ecx;
		struct {
			u32 prefetchwt1            : 1; u32 avx512_vmbi            : 1;
			u32 umip                   : 1; u32 pku                    : 1;
			u32 ospke                  : 1; u32 waitpkg                : 1;
			u32 avx512_vmbi2           : 1; u32 cet_ss                 : 1;
			u32 gfni                   : 1; u32 vaes                   : 1;
			u32 vpclmulqdq             : 1; u32 avx512_vnni            : 1;
			u32 avx512_bitalg          : 1; u32 tme_en                 : 1;
			u32 avx512_vpopcntdq       : 1; u32 reserved_1             : 1;
			u32 la57                   : 1; u32 mawau_val              : 5;
			u32 rdpid                  : 1; u32 kl                     : 1;
			u32 buslocktrap            : 1; u32 cldemote               : 1;
			u32 reserved_2             : 1; u32 movdiri                : 1;
			u32 movdir64b              : 1; u32 enqcmd                 : 1;
			u32 sgx_lc                 : 1; u32 pks                    : 1;
		};
		// MAJOR NOTE: Sub-leaf 1 & 2 not implemented
	};

	union {
		u32 edx;
		struct {
			u32 reserved_3             : 1; u32 sgx_keys               : 1;
			u32 avx512_4vnnw           : 1; u32 avx512_4fmaps          : 1;
			u32 fast_short_rep_mov     : 1; u32 uintr                  : 1;
			u32 reserved_4             : 1; u32 reserved_5             : 1;
			u32 avx512_vp2intersect    : 1; u32 srbds_ctrl             : 1;
			u32 md_clear               : 1; u32 rtm_always_abort       : 1;
			u32 reserved_6             : 1; u32 rtm_force_abort        : 1;
			u32 serialized             : 1; u32 hybrid                 : 1;
			u32 tsxldtrk               : 1; u32 reserved_7             : 1;
			u32 pconfig                : 1; u32 architectural_lbrs     : 1;
			u32 cet_ibt                : 1; u32 reserved_8             : 1;
			u32 amx_bf16               : 1; u32 avx512_fp16            : 1;
			u32 amx_tile               : 1; u32 amx_int8               : 1;
			u32 enum_indirect_ibrs     : 1; u32 enum_stibp             : 1;
			u32 enum_l1d_flush         : 1; u32 enum_ia32_arch_cap_msr : 1;
			u32 enum_ia32_core_cap_msr : 1; u32 enum_ssbd              : 1;
		};
		// MAJOR NOTE: Sub-leaf 1 & 2 not implemented
	};
} CPUID_GenuineIntel_0x00000007h;


typedef struct {
	union {
		u32 eax;
		struct {
			u32 version               : 8;
			u32 count_per_lcpu        : 8;
			u32 bit_width             : 8;
			u32 ebx_bit_vector_length : 8;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 event_core_cycle                 :  1;
			u32 event_instruction_retired        :  1;
			u32 event_reference_cycles           :  1;
			u32 event_last_level_cache_ref       :  1;
			u32 event_last_level_cache_miss      :  1;
			u32 event_branch_instruction_retired :  1;
			u32 event_branch_mispredict_retired  :  1;
			u32 event_top_down_slots             :  1;
			u32 reserved_0                       : 24;
		};
	};

	union {
		u32 ecx;
		u32 fixed_counter_bit_mask;
	};

	union {
		u32 edx;
		struct {
			u32 contiguous_fixed_function_perf_counter :  5;
			u32 bit_width_fixed_function_perf_counter  :  8;
			u32 reserved_1                             :  2;
			u32 any_thread_deprecation                 :  1;
			u32 reserved_2                             : 16;
		};
	};
} CPUID_GenuineIntel_0x0000000Ah;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 x2apic_shift :  5;
			u32 reserved_0   : 27;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 lcpu_count : 16;
			u32 reserved_1 : 16;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 ecx_input   :  8;
			u32 domain_type :  8;
			u32 reserved_2  : 16;
		};
	};

	union {
		u32 edx;
		u32 x2apic_id_lcpu;
	};
} CPUID_GenuineIntel_0x0000000Bh;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 x2apic_shift :  5;
			u32 reserved_0   : 27;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 lcpu_count : 16;
			u32 reserved_1 : 16;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 ecx_input   :  8;
			u32 domain_type :  8;
			u32 reserved_2  : 16;
		};
	};

	union {
		u32 edx;
		u32 x2apic_id_lcpu;
	};
} CPUID_GenuineIntel_0x0000001Fh;

/////////////////////////////////////
// GenuineIntel Extended Functions //
// 0x80000000 - 0x80000008h        //
/////////////////////////////////////
typedef struct {
	union {
		u32 eax;
		u32 max_fn;
	};

	union {
		u32 ebx;
		u32 reserved_0;
	};

	union {
		u32 ecx;
		u32 reserved_1;
	};

	union {
		u32 edx;
		u32 reserved_2;
	};
} CPUID_GenuineIntel_0x80000000h;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		u32 reserved_1;
	};

	union {
		u32 ecx;
		struct {
			u32 lafh_sahf              : 1; u32 reserved_2             : 1;
			u32 reserved_3             : 1; u32 reserved_4             : 1;
			u32 reserved_5             : 1; u32 lzcnt                  : 1;
			u32 reserved_6             : 1; u32 reserved_7             : 1;
			u32 prefetchw              : 1; u32 reserved_8             : 1;
			u32 reserved_9             : 1; u32 reserved_10            : 1;
			u32 reserved_11            : 1; u32 reserved_12            : 1;
			u32 reserved_13            : 1; u32 reserved_14            : 1;
			u32 reserved_15            : 1; u32 reserved_16            : 1;
			u32 reserved_17            : 1; u32 reserved_18            : 1;
			u32 reserved_19            : 1; u32 reserved_20            : 1;
			u32 reserved_21            : 1; u32 reserved_22            : 1;
			u32 reserved_23            : 1; u32 reserved_24            : 1;
			u32 reserved_25            : 1; u32 reserved_26            : 1;
			u32 reserved_27            : 1; u32 reserved_28            : 1;
			u32 reserved_29            : 1; u32 reserved_30            : 1;
		};
	};

	union {
		u32 edx;
		struct {
			u32 reserved_31            : 1; u32 reserved_32            : 1;
			u32 reserved_33            : 1; u32 reserved_34            : 1;
			u32 reserved_35            : 1; u32 resreved_36            : 1;
			u32 reserved_37            : 1; u32 reserved_38            : 1;
			u32 reserved_39            : 1; u32 reserved_40            : 1;
			u32 reserved_41            : 1; u32 syscall_sysret         : 1;
			u32 reserved_42            : 1; u32 reserved_43            : 1;
			u32 reserved_44            : 1; u32 reserved_45            : 1;
			u32 reserved_46            : 1; u32 reserved_47            : 1;
			u32 reserved_48            : 1; u32 reserved_49            : 1;
			u32 execute_disable_bit    : 1; u32 reserved_50            : 1;
			u32 reserved_51            : 1; u32 reserved_52            : 1;
			u32 reserved_53            : 1; u32 reserved_54            : 1;
			u32 page_1gb               : 1; u32 rdtscp                 : 1;
			u32 reserved_55            : 1; u32 intel_64_arch          : 1;
			u32 reserved_56            : 1; u32 reserved_57            : 1;
		};
	};

} CPUID_GenuineIntel_0x80000001h;

typedef union {
	char cpu_name_0_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_GenuineIntel_0x80000002h;

typedef union {
	char cpu_name_1_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_GenuineIntel_0x80000003h;

typedef union {
	char cpu_name_2_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_GenuineIntel_0x80000004h;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		u32 reserved_1;
	};

	union {
		u32 ecx;
		struct {
			u32 l2_cache_line_size_in_bytes :  8;
			u32 reserved_2                  :  4;
			u32 l2_associativity_field      :  4;
			u32 l2_cache_size_in_1k_units   : 16;
		};
	};

	union {
		u32 edx;
		u32 reserved_3;
	};
} CPUID_GenuineIntel_0x80000006h;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		u32 reserved_1;
	};

	union {
		u32 ecx;
		u32 reserved_2;
	};

	union {
		u32 edx;
		struct {
			u32 reserved_3    :  8;
			u32 invariant_tsc :  1;
			u32 reserved_4    : 23;
		};
	};
} CPUID_GenuineIntel_0x80000007h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 phys_addr_bits   : 8;
			u32 linear_addr_bits : 8;
			u32 guest_addr_bits  : 8;
			u32 reserved_0       : 8;
		};
	};

	union {
		u32 ebx;
		u32 reserved_1;
	};

	union {
		u32 ecx;
		u32 reserved_2;
	};

	union {
		u32 edx;
		struct {
			u32 reserved_3 :  9;
			u32 wbnoinvd   :  1;
			u32 reserved_4 : 22;
		};
	};

} CPUID_GenuineIntel_0x80000008h;

typedef struct {
	MArena mem_enumerations;

	// Standard Features
	CPUID_GenuineIntel_0x00000000h   vendor;
	CPUID_GenuineIntel_0x00000001h   identifier_and_features;
	CPUID_GenuineIntel_0x00000002h   cache_tlb;
	CPUID_GenuineIntel_0x00000003h   psn;
	CPUID_GenuineIntel_0x00000004h  *topology_cache;
	CPUID_GenuineIntel_0x00000007h   structured_features;
	CPUID_GenuineIntel_0x0000000Ah   performance_monitoring;
	CPUID_GenuineIntel_0x0000000Bh  *topology_cpu;
	CPUID_GenuineIntel_0x0000001Fh  *topology_cpu2;

	// Extended
	CPUID_GenuineIntel_0x80000000h ext_fn;
	CPUID_GenuineIntel_0x80000001h identifier_and_features_ext;
	CPUID_GenuineIntel_0x80000002h cpu_name_0_2;
	CPUID_GenuineIntel_0x80000003h cpu_name_1_2;
	CPUID_GenuineIntel_0x80000004h cpu_name_2_2;
	CPUID_GenuineIntel_0x80000006h l2;
	CPUID_GenuineIntel_0x80000007h invtsc;
	CPUID_GenuineIntel_0x80000008h addr_bits_and_wbnoinvd;
} CPUID_GenuineIntel;

#endif // INCLUDE_NOSTD_COMMON_CPUID_INTEL_H

