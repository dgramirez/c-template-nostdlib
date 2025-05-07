#ifndef INCLUDE_NOSTD_COMMON_CPUID_AMD_H
#define INCLUDE_NOSTD_COMMON_CPUID_AMD_H

// MAJOR NOTE:
//     In order to truly understand this, you need to be side-by-side with
//     the AMD Manual. The AMD Manual that is in current use is:
//         - AMD64 Architecture Programmer's Manual Volumes 1-5
//         - Publication No. 40332
//         - Revision: 4.08
//         - Date: April 2024
//
//     Updated manuals should be fine as long as the appropriate pages
//     are found. Here is the starting page number for the CPUID function
//     with the AMD Manual provided above:
//         - Pdf  Pg 1875
//         - Phys Pg  599 (Appendix E)

// MAJOR NOTE:
//     Updating this will also require you to update this file:
//         - include/nostd/platform/common/cpuid.h
//
//     Without the update, the new CPUID items will not be detected.

// Notes: Reserved Functions:
//            - 0x00000002h
//            - 0x00000003h
//            - 0x00000004h
//            - 0x00000008h
//            - 0x00000009h
//            - 0x0000000Ah
//            - 0x0000000Eh
//            - 0x40000000h - 0x400000FFh [Hypervisor Use]
//            - 0x80000009h
//            - 0x8000000Bh - 0x80000018h [Reserved]
//            - 0x80000024h
//            - 0x80000025h

//////////////////////////////////
// AuthenticAMD Basic Functions //
// 0x00000001 - 0x00000010h     //
//////////////////////////////////
typedef union {
	char vendor[16];
	struct {
		u32 ebx, edx, ecx, eax;
	};
	struct {
		u32 reserved_0, reserved_1, reserved_2, max_fn;
	};
} CPUID_AuthenticAMD_0x00000000h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 stepping    : 4;
			u32 base_model  : 4;
			u32 base_family : 4;
			u32 reserved_0  : 4;
			u32 ext_model   : 4;
			u32 ext_family  : 8;
			u32 reserved_1  : 4;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 brand_id_8bit : 8;
			u32 clflush_size  : 8;
			u32 lcpu_count    : 8;
			u32 local_apic_id : 8;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 sse3                   : 1; u32 pclmulqdq              : 1;
			u32 reserved_2             : 1; u32 monitor                : 1;
			u32 reserved_3             : 1; u32 reserved_4             : 1;
			u32 reserved_5             : 1; u32 reserved_6             : 1;
			u32 reserved_7             : 1; u32 ssse3                  : 1;
			u32 reserved_8             : 1; u32 reserved_9             : 1;
			u32 fma                    : 1; u32 cmpxchg16b             : 1;
			u32 reserved_10            : 1; u32 reserved_11            : 1;
			u32 reserved_12            : 1; u32 reserved_13            : 1;
			u32 reserved_14            : 1; u32 sse4_1                 : 1;
			u32 sse4_2                 : 1; u32 x2apic                 : 1;
			u32 movbe                  : 1; u32 popcnt                 : 1;
			u32 resreved               : 1; u32 aes                    : 1;
			u32 xsave                  : 1; u32 osxsave                : 1;
			u32 avx                    : 1; u32 f16c                   : 1;
			u32 rdrand                 : 1; u32 raz                    : 1;
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
			u32 reserved_15            : 1; u32 sysenter_sysexit       : 1;
			u32 mtrr                   : 1; u32 pge                    : 1;
			u32 mca                    : 1; u32 cmov                   : 1;
			u32 pat                    : 1; u32 pse36                  : 1;
			u32 reserved_16            : 1; u32 clfsh                  : 1;
			u32 reserved_17            : 1; u32 reserved_18            : 1;
			u32 reserved_19            : 1; u32 mmx                    : 1;
			u32 fxsr                   : 1; u32 sse                    : 1;
			u32 sse2                   : 1; u32 reserved_20            : 1;
			u32 htt                    : 1; u32 reserved_21            : 1;
			u32 reserved_22            : 1; u32 reserved_23            : 1;
		};
	};

} CPUID_AuthenticAMD_0x00000001h;

// NOTE: 0x00000002h, 0x00000003h, 0x00000004h is RESERVED for AuthenticAMD

typedef struct {
	union {
		u32 eax;
		struct {
			u32 mon_line_size_min : 16;
			u32 reserved_0        : 16;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 mon_line_size_max : 16;
			u32 reserved_1        : 16;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 emx        :  1;
			u32 ibe        :  1;
			u32 reserved_2 : 30;
		};
	};

	union {
		u32 edx;
		u32 reserved_3;
	};
} CPUID_AuthenticAMD_0x00000005h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 reserved_0        : 2;
			u32 ARAT              : 1;
			u32 reserved_1        : 29;
		};
	};

	union {
		u32 ebx;
		u32 reserved_2;
	};

	union {
		u32 ecx;
		struct {
			u32 eff_freq   :  1;
			u32 reserved_3 : 31;
		};
	};

	union {
		u32 edx;
		u32 reserved_4;
	};
} CPUID_AuthenticAMD_0x00000006h;

typedef struct {
	union {
		u32 eax;
		u32 max_sub_fn;
	};

	union {
		u32 ebx;
		struct {
			u32 fsgsbase               : 1; u32 reserved_0             : 1;
			u32 reserved_1             : 1; u32 bmi_1                  : 1;
			u32 reserved_2             : 1; u32 avx2                   : 1;
			u32 reserved_3             : 1; u32 smep                   : 1;
			u32 bmi_2                  : 1; u32 reserved_4             : 1;
			u32 invpcid                : 1; u32 reserved_5             : 1;
			u32 pqm                    : 1; u32 reserved_6             : 1;
			u32 reserved_7             : 1; u32 pqe                    : 1;
			u32 reserved_8             : 1; u32 reserved_9             : 1;
			u32 rseed                  : 1; u32 adx                    : 1;
			u32 smap                   : 1; u32 reserved_10            : 1;
			u32 reserved_11            : 1; u32 clflushopt             : 1;
			u32 clwb                   : 1; u32 reserved_12            : 1;
			u32 reserved_13            : 1; u32 reserved_14            : 1;
			u32 reserved_15            : 1; u32 sha                    : 1;
			u32 reserved_16            : 1; u32 reserved_17            : 1;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 reserved_18            : 1; u32 reserved_19            : 1;
			u32 umip                   : 1; u32 pku                    : 1;
			u32 ospke                  : 1; u32 reserved_20            : 1;
			u32 reserved_21            : 1; u32 cet_ss                 : 1;
			u32 reserved_22            : 1; u32 vaes                   : 1;
			u32 vpcmulqdq              : 1; u32 reserved_23            : 1;
			u32 reserved_24            : 1; u32 reserved_25            : 1;
			u32 reserved_26            : 1; u32 reserved_27            : 1;
			u32 la57                   : 1; u32 reserved_28            : 1;
			u32 reserved_29            : 1; u32 reserved_30            : 1;
			u32 reserved_31            : 1; u32 reserved_32            : 1;
			u32 rdpid                  : 1; u32 reserved_33            : 1;
			u32 buslocktrap            : 1; u32 reserved_34            : 1;
			u32 reserved_35            : 1; u32 reserved_36            : 1;
			u32 reserved_37            : 1; u32 reserved_38            : 1;
			u32 reserved_39            : 1; u32 reserved_40            : 1;
		};
	};

	union {
		u32 edx;
		u32 reserved_41;
	};
} CPUID_AuthenticAMD_0x00000007h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 sfn0_thread_mask_width :  5;
			u32 reserved_0        : 27;
		};

		struct {
			u32 sfn1_core_mask_width :  5;
			u32 reserved_1      : 27;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 numlogproc : 16;
			u32 reserved_2 : 16;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 input_ecx       :  8;
			u32 hierarchy_level :  8;
			u32 reserved_3      : 16;
		};
	};

	union {
		u32 edx;
		u32 x2apic_id;
	};
} CPUID_AuthenticAMD_0x0000000Bh;

typedef struct {
	union {
		u32 eax;
	};

	union {
		u32 ebx;
	};

	union {
		u32 ecx;
	};

	union {
		u32 edx;
	};
} CPUID_AuthenticAMD_0x0000000Dh;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 sfn1_counter_size :  8;
			u32 sfn1_overflow_bit :  1;
			u32 reserved_0        : 23;
		};
	};

	union {
		u32 ebx;
		u32 sfn0_max_rmid_any;
		u32 sfn1_scale_factor;
	};

	union {
		u32 ecx;
		u32 sfn1_max_rmid_l3;
	};

	union {
		u32 edx;
		struct {
			u32 reserved_1        :  1;
			u32 sfn0_l3_cache_mon :  1;
			u32 reserved_2        : 30;
		};
		struct {
			u32 l3_cache_occ_mon      : 1;
			u32 l3_cache_bw_mon_evt_0 : 1;
			u32 l3_cache_bw_mon_evt_1 : 1;
			u32 reserved_3        : 29;
		};
	};
} CPUID_AuthenticAMD_0x0000000Fh;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		u32 sfn1_l3_share_alloc_mask;
	};

	union {
		u32 ecx;
		struct {
			u32 reserved_1  :  2;
			u32 sfn1_cdp    :  1;
			u32 reserved_2  : 29;
		};
	};

	union {
		u32 edx;
		struct {
			u32 reserved_3        :  1;
			u32 sfn0_l3_alloc     :  1;
			u32 reserved_4        : 30;
		};
		struct {
			u32 sfn1_cos_max : 16;
			u32 reserved_5   : 16;
		};
	};
} CPUID_AuthenticAMD_0x00000010h;

/////////////////////////////////////
// AuthenticAMD Extended Functions //
// 0x80000000 - 0x80000026h        //
/////////////////////////////////////
typedef union {
	char vendor[16];
	struct {
		u32 ebx, edx, ecx, eax;
	};
	struct {
		u32 reserved_0, reserved_1, reserved_2, max_fn;
	};
} CPUID_AuthenticAMD_0x80000000h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 stepping    : 4;
			u32 base_model  : 4;
			u32 base_family : 4;
			u32 reserved_0  : 4;
			u32 ext_model   : 4;
			u32 ext_family  : 8;
			u32 reserved_1  : 4;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 brand_id   : 16;
			u32 reserved_2 : 12;
			u32 pkg_type   :  4;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 lafh_sahf              : 1; u32 cmp_legacy             : 1;
			u32 svm                    : 1; u32 extended_apic_space    : 1;
			u32 alt_mov_cr8            : 1; u32 abm                    : 1;
			u32 sse4a                  : 1; u32 misaligned_sse_mode    : 1;
			u32 _3dnow_prefetch        : 1; u32 osvw                   : 1;
			u32 ibs                    : 1; u32 xop                    : 1;
			u32 skinit                 : 1; u32 wdt                    : 1;
			u32 reserved_3             : 1; u32 lwp                    : 1;
			u32 fma4                   : 1; u32 tce                    : 1;
			u32 reserved_4             : 1; u32 reserved_5             : 1;
			u32 reserved_6             : 1; u32 tbm                    : 1;
			u32 topology_extensions    : 1; u32 perf_ctr_ext_core      : 1;
			u32 perf_ctr_ext_nb        : 1; u32 reserved               : 1;
			u32 data_breakpoint_ext    : 1; u32 perf_tsc               : 1;
			u32 perf_ctr_ext_llc       : 1; u32 monitorx               : 1;
			u32 addr_mask_ext          : 1; u32 reserved_7             : 1;
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
			u32 reserved_8             : 1; u32 syscall_sysret         : 1;
			u32 mtrr                   : 1; u32 pge                    : 1;
			u32 mca                    : 1; u32 cmov                   : 1;
			u32 pat                    : 1; u32 pse36                  : 1;
			u32 reserved_9             : 1; u32 reserved_10            : 1;
			u32 nxu                    : 1; u32 reserved_11            : 1;
			u32 mmx_ext_amd            : 1; u32 mmx                    : 1;
			u32 fxsr                   : 1; u32 ffxsr                  : 1;
			u32 page_1gb               : 1; u32 rdtscp                 : 1;
			u32 reserved_12            : 1; u32 lm                     : 1;
			u32 _3dnow_ext             : 1; u32 _3dnow                 : 1;
		};
	};

} CPUID_AuthenticAMD_0x80000001h;

typedef union {
	char cpu_name_0_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_AuthenticAMD_0x80000002h;

typedef union {
	char cpu_name_1_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_AuthenticAMD_0x80000003h;

typedef union {
	char cpu_name_2_2[16];
	struct {
		u32 eax, ebx, ecx, edx;
	};
} CPUID_AuthenticAMD_0x80000004h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 l1i_tlb_24mb_size  : 8;
			u32 l1i_tlb_24mb_assoc : 8;
			u32 l1d_tlb_24mb_size  : 8;
			u32 l1d_tlb_24mb_assoc : 8;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 l1i_tlb_4kb_size  : 8;
			u32 l1i_tlb_4kb_assoc : 8;
			u32 l1d_tlb_4kb_size  : 8;
			u32 l1d_tlb_4kb_assoc : 8;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 reserved_1  :  2;
			struct {
				u32 l1d_cache_line_size     : 8;
				u32 l1d_cache_lines_per_tag : 8;
				u32 l1d_cache_assoc         : 8;
				u32 l1d_cache_size_in_kb    : 8;
			};
		};
	};

	union {
		u32 edx;
		struct {
			u32 l1i_cache_line_size     : 8;
			u32 l1i_cache_lines_per_tag : 8;
			u32 l1i_cache_assoc         : 8;
			u32 l1i_cache_size_in_kb    : 8;
		};
	};
} CPUID_AuthenticAMD_0x80000005h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 l2i_tlb_24mb_size  : 8;
			u32 l2i_tlb_24mb_assoc : 8;
			u32 l2d_tlb_24mb_size  : 8;
			u32 l2d_tlb_24mb_assoc : 8;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 l2i_tlb_4kb_size  : 8;
			u32 l2i_tlb_4kb_assoc : 8;
			u32 l2d_tlb_4kb_size  : 8;
			u32 l2d_tlb_4kb_assoc : 8;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 reserved_1  :  2;
			struct {
				u32 l2_cache_line_size     : 8;
				u32 l2_cache_lines_per_tag : 8;
				u32 l2_cache_assoc         : 8;
				u32 l2_cache_size_in_kb    : 8;
			};
		};
	};

	union {
		u32 edx;
		struct {
			u32 l3_cache_line_size     :  8;
			u32 l3_cache_lines_per_tag :  4;
			u32 l3_cache_assoc         :  4;
			u32 reserved               :  2;
			u32 l3_cache_size_in_kb    : 14;
		};
	};
} CPUID_AuthenticAMD_0x80000006h;

typedef struct {
	union {
		u32 eax;
		u32 reserved_0;
	};

	union {
		u32 ebx;
		struct {
			u32 mca_overflow_recovery :  1;
			u32 succor                :  1;
			u32 hwa                   :  1;
			u32 scalable_mca          :  1;
			u32 reserved_1            : 28;
		};
	};

	union {
		u32 ecx;
		u32 cpu_power_sample_time_ratio;
	};

	union {
		u32 edx;
		struct {
			u32 ts                     :  1;
			u32 fid                    :  1;
			u32 vid                    :  1;
			u32 ttp                    :  1;
			u32 tm                     :  1;
			u32 reserved_2             :  1;
			u32 _100_mhz_steps         :  1;
			u32 hw_pstate              :  1;
			u32 tsc_invariant          :  1;
			u32 cpb                    :  1;
			u32 effective_frequency_ro :  1;
			u32 cpu_feedback_interface :  1;
			u32 cpu_power_reporting    :  1;
			u32 reserved_3             : 19;
		};
	};
} CPUID_AuthenticAMD_0x80000007h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 reserved_0       : 8;
			u32 guest_paddr_size : 8;
			u32 linaddr_size     : 8;
			u32 paddr_size       : 8;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 clzero                 : 1; u32 inst_ret_cnt_msr     : 1;
			u32 rstr_fp_err_ptrs       : 1; u32 invlpgb              : 1;
			u32 rdpru                  : 1; u32 reserved_1           : 1;
			u32 be                     : 1; u32 reserved_2           : 1;
			u32 mcommit                : 1; u32 wbnoinvd             : 1;
			u32 reserved_3             : 1; u32 reserved_4           : 1;
			u32 ibpb                   : 1; u32 int_wbinvd           : 1; 
			u32 ibrs                   : 1; u32 stibp                : 1; 
			u32 ibrs_always_on         : 1; u32 stibp_always_on      : 1;
			u32 ibrs_preferred         : 1; u32 ibrs_same_mode       : 1;
			u32 efer_lmsle_unsupported : 1; u32 invlpgb_nested_pages : 1;
			u32 reserved_5             : 1; u32 reserved_6           : 1;
			u32 ssbd                   : 1; u32 ssbd_virt_spec_ctrl  : 1;
			u32 ssbd_not_required      : 1; u32 cppc                 : 1;
			u32 psfd                   : 1; u32 btc_no               : 1;
			u32 ibpb_ret               : 1; u32 reserved_7           : 1;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 pcores        :  8;
			u32 reserved_x    :  4;
			u32 apicid_size   :  4;
			u32 perf_tsc_size :  2;
			u32 reserved_y    : 14;
		};
	};

	union {
		u32 edx;
		struct {
			u32 max_page_count_invlpgb : 16;
			u32 max_ecx_value_rdpru    : 16;
		};
	};

} CPUID_AuthenticAMD_0x80000008h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 svm_revision :  8;
			u32 reserved_0   : 24;
		};
	};

	union {
		u32 ebx;
		u32 num_of_available_space_ids;
	};

	union {
		u32 ecx;
		u32 reserved_1;
	};

	union {
		u32 edx;
		struct {
			u32 np                        : 1; u32 lbr_virt                : 1;
			u32 svml                      : 1; u32 nrips                   : 1;
			u32 tsc_rate_msr              : 1; u32 vmcb_clean              : 1;
			u32 flush_by_asid             : 1; u32 decode_assists          : 1;
			u32 pmc_virt                  : 1; u32 reserved_2              : 1;
			u32 pause_filter              : 1; u32 reserved_3              : 1;
			u32 pause_filter_threshold    : 1; u32 avic                    : 1;
			u32 reserved_4                : 1; u32 vmsave_virt             : 1;
			u32 vgif                      : 1; u32 gmet                    : 1;
			u32 x2avic                    : 1; u32 sss_check               : 1;
			u32 spec_ctrl                 : 1; u32 rogpt                   : 1;
			u32 reserved_5                : 1; u32 host_mce_override       : 1;
			u32 tlb_ictl                  : 1; u32 vnmi                    : 1;
			u32 ibs_virt                  : 1; u32 ext_lvl_avic_access_chg : 1;
			u32 nested_virt_vmcb_addr_chk : 1; u32 bus_lock_threshold      : 1;
			u32 idle_hlt_intercept        : 1; u32 reserved_6              : 1;
		};
	};
} CPUID_AuthenticAMD_0x8000000Ah;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 l1i_tlb_1g_size  : 12;
			u32 l1i_tlb_1g_assoc :  4;
			u32 l1d_tlb_1g_size  : 12;
			u32 l1d_tlb_1g_assoc :  4;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 l2i_tlb_1g_size  : 12;
			u32 l2i_tlb_1g_assoc :  4;
			u32 l2d_tlb_1g_size  : 12;
			u32 l2d_tlb_1g_assoc :  4;
		};
	};

	union {
		u32 ecx;
		u32 reserved_0;
	};

	union {
		u32 edx;
		u32 reserved_1;
	};
} CPUID_AuthenticAMD_0x80000019h;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 fp128      : 1;
			u32 movu       : 1;
			u32 fp256      : 1;
			u32 reserved_0 : 29;
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
		u32 reserved_3;
	};
} CPUID_AuthenticAMD_0x8000001Ah;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 ibsffv                :  1;
			u32 fetch_sam             :  1;
			u32 op_sam                :  1;
			u32 read_write_op_counter :  1;
			u32 op_counting_support   :  1;
			u32 branch_target         :  1;
			u32 opcnt_ext             :  1;
			u32 rip_invalid_chk       :  1;
			u32 op_branch_fuse        :  1;
			u32 reserved_0            :  1;
			u32 reserved_1            :  1;
			u32 ibs_l3_miss_filtering :  1;
			u32 reserved_2            : 20;
		};
	};

	union {
		u32 ebx;
		u32 reserved_3;
	};

	union {
		u32 ecx;
		u32 reserved_4;
	};

	union {
		u32 edx;
		u32 reserved_5;
	};
} CPUID_AuthenticAMD_0x8000001Bh;

typedef struct {
	union {
		u32 eax;
		struct {
			u32 lwp_avail  :  1;
			u32 lwp_val    :  1;
			u32 lwp_ire    :  1;
			u32 lwp_bre    :  1;
			u32 lwp_dme    :  1;
			u32 lwp_cnh    :  1;
			u32 lwp_rnh    :  1;
			u32 reserved_0 : 22;
			u32 lwp_cont   :  1;
			u32 lwp_ptsc   :  1;
			u32 lwp_int    :  1;
		};
	};

	union {
		u32 ebx;
		struct {
			u32 lwp_cb_size      : 8;
			u32 lwp_event_size   : 8;
			u32 lwp_max_events   : 8;
			u32 lwp_event_offset : 8;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 lwp_latency_max       : 5;
			u32 lwp_data_address      : 1;
			u32 lwp_latency_rnd       : 3;
			u32 lwp_version           : 7;
			u32 lwp_min_buffer_size   : 8;
			u32 reserved_1            : 4;
			u32 lwp_branch_prediction : 1;
			u32 lwp_ip_filtering      : 1;
			u32 lwp_cache_levels      : 1;
			u32 lwp_cache_latency     : 1;
		};
	};

	union {
		u32 edx;
		struct {
			u32 edx_lwp_avail  :  1;
			u32 edx_lwp_val    :  1;
			u32 edx_lwp_ire    :  1;
			u32 edx_lwp_bre    :  1;
			u32 edx_lwp_dme    :  1;
			u32 edx_lwp_cnh    :  1;
			u32 edx_lwp_rnh    :  1;
			u32 edx_reserved_0 : 22;
			u32 edx_lwp_cont   :  1;
			u32 edx_lwp_ptsc   :  1;
			u32 edx_lwp_int    :  1;
		};
	};
} CPUID_AuthenticAMD_0x8000001Ch;

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
			u32 reserved_1             :  6;
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
			u32 wbinvd          :  1;
			u32 cache_inclusive :  1;
			u32 reserved_2      : 30;
		};
	};

} CPUID_AuthenticAMD_0x8000001Dh;

typedef struct {
	union {
		u32 eax;
		u32 extended_apic_id;
	};

	union {
		u32 ebx;
		struct {
			u32 compute_unit_id          :  8;
			u32 threads_per_compute_unit :  8;
			u32 reserved_0               : 16;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 node_id             :  8;
			u32 nodes_per_processor :  8;
			u32 reserved_1          : 16;
		};
	};

	union {
		u32 edx;
		u32 reserved_2;
	};

} CPUID_AuthenticAMD_0x8000001Eh;

typedef struct {
	// Standard Functions
	CPUID_AuthenticAMD_0x00000000h vendor;
	CPUID_AuthenticAMD_0x00000001h identifier_and_features;
	CPUID_AuthenticAMD_0x00000005h monitor_mwait;
	CPUID_AuthenticAMD_0x00000006h power_management;
	CPUID_AuthenticAMD_0x00000007h structured_features;
	CPUID_AuthenticAMD_0x0000000Bh topology_thread;
	CPUID_AuthenticAMD_0x0000000Bh topology_core;
	// NOTE: 0x0000000Dh was skipped
	CPUID_AuthenticAMD_0x0000000Fh pqm_capabilities;
	CPUID_AuthenticAMD_0x0000000Fh l3_cache_monitoring;
	CPUID_AuthenticAMD_0x00000010h pqe_capabilities;
	CPUID_AuthenticAMD_0x00000010h l3_cache_enforcement;

	// Extended Functions
	CPUID_AuthenticAMD_0x80000000h  vendor_ext;
	CPUID_AuthenticAMD_0x80000001h  identifier_and_features_ext;
	CPUID_AuthenticAMD_0x80000002h  cpu_name_0_2;
	CPUID_AuthenticAMD_0x80000003h  cpu_name_1_2;
	CPUID_AuthenticAMD_0x80000004h  cpu_name_2_2;
	CPUID_AuthenticAMD_0x80000005h  l1;
	CPUID_AuthenticAMD_0x80000006h  l2_and_l3;
	CPUID_AuthenticAMD_0x80000007h  power_management_ext;
	CPUID_AuthenticAMD_0x80000008h  cpu_capacity_and_features_ext;
	CPUID_AuthenticAMD_0x8000000Ah  svm_features;
	CPUID_AuthenticAMD_0x80000019h  tlb_1gb;
	CPUID_AuthenticAMD_0x8000001Ah  instruction_optimizations;
	CPUID_AuthenticAMD_0x8000001Bh  sample_based_capabilities;
	CPUID_AuthenticAMD_0x8000001Ch  lwp;
	CPUID_AuthenticAMD_0x8000001Dh *topology_cache;
	CPUID_AuthenticAMD_0x8000001Eh  topology_cpu;
	// NOTE: The remaining have been skipped.
} CPUID_AuthenticAMD;

#endif // INCLUDE_NOSTD_COMMON_CPUID_AMD_H

