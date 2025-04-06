#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H

typedef struct {
	union {
		u32 eax;
		struct {
			u32 stepping_id : 4;
			u32 model       : 4;
			u32 family      : 4;
			u32 cpu_type    : 2;
			u32 reserved_1  : 2;
			u32 model_ext   : 4;
			u32 family_ext  : 8;
			u32 reserved_2  : 4;
		};
	};

	union {
		u32 ebx;
		struct {
			u8 brand_id;
			u8 clflush_size;
		    u8 logical_cpu_count;
			u8 local_apic;
		};
	};

	union {
		u32 ecx;
		struct {
			u32 sse3                : 1; u32 pclmulqdq           : 1;
			u32 dtes64              : 1; u32 monitor             : 1;
			u32 ds_cpl              : 1; u32 vmx                 : 1;
			u32 smx                 : 1; u32 eist                : 1;
			u32 tm2                 : 1; u32 ssse3               : 1;
			u32 cnxtid              : 1; u32 sdbg                : 1;
			u32 fma                 : 1; u32 cmpxchg16b          : 1;
			u32 xtpr_update_control : 1; u32 pdcm                : 1;
			u32 reserved            : 1; u32 pcid                : 1;
			u32 dca                 : 1; u32 sse4_1              : 1;
			u32 sse4_2              : 1; u32 x2apic              : 1;
			u32 movbe               : 1; u32 popcnt              : 1;
			u32 tsc_deadline        : 1; u32 aes                 : 1;
			u32 xsave               : 1; u32 osxsave             : 1;
			u32 avx                 : 1; u32 f16c                : 1;
			u32 rdrand              : 1; u32 raz                 : 1;
		};
	};

	union {
		u32 edx;
		struct {
			u32 fpu              : 1; u32 vme              : 1;
			u32 de               : 1; u32 pse              : 1;
			u32 tsc              : 1; u32 msr              : 1;
			u32 pae              : 1; u32 mce              : 1;
			u32 cmpxchg8b        : 1; u32 apic             : 1;
			u32 reserved_3       : 1; u32 sysenter_sysexit : 1;
			u32 mtrr             : 1; u32 pge              : 1;
			u32 mca              : 1; u32 cmov             : 1;
			u32 pat              : 1; u32 pse36            : 1;
			u32 psn              : 1; u32 clfsh            : 1;
			u32 reserved_4       : 1; u32 ds               : 1;
			u32 acpi             : 1; u32 mmx              : 1;
			u32 fxsr             : 1; u32 sse              : 1;
			u32 sse2             : 1; u32 ss               : 1;
			u32 htt              : 1; u32 tm               : 1;
			u32 reserved_5       : 1; u32 pbe              : 1;
		};
	};
} CpuidBasicInfoAndFeatures;

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_CPUID_H

