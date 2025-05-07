#ifndef INCLUDE_NOSTD_COMMON_CPUID_H
#define INCLUDE_NOSTD_COMMON_CPUID_H

typedef enum {
	CPUID_VENDOR_ERROR         = -1,
	CPUID_VENDOR_UNKNOWN       =  0,
	CPUID_VENDOR_GenuineIntel,
	CPUID_VENDOR_AuthenticAMD,
	CPUID_VENDOR_COUNT
} CPUIDVendorID;

typedef struct {
	u32 eax, ebx, ecx, edx;
} CPUIDRegisters;

typedef union {
	char data[16];
	struct { u32 ebx, edx, ecx, eax; };
} CPUIDVendor;

typedef union {
	char data[48];
	i32  regs[12];
	struct {
		u32 eax0, ebx0, ecx0, edx0;
		u32 eax1, ebx1, ecx1, edx1;
		u32 eax2, ebx2, ecx2, edx2;
	};
} CPUIDProcessorName;

typedef struct {
	union {
		u32 feature_bits_1;
		struct {
			u32 sse                    : 1; u32 sse2                   : 1;
			u32 sse3                   : 1; u32 ssse3                  : 1;
			u32 sse4_1                 : 1; u32 sse4_2                 : 1;
			u32 sse4a                  : 1; u32 mmx                    : 1;
			u32 avx                    : 1; u32 avx2                   : 1;
			u32 avx512f                : 1; u32 reserved_0             : 1;
			u32 fma                    : 1; u32 fma4                   : 1;
			u32 tsc                    : 1; u32 rdtscp                 : 1;
			u32 rdpmc                  : 1; u32 rdpru                  : 1;
			u32 lwp                    : 1; u32 reserved_1             : 1;
			u32 reserved_2             : 12;
		};
	};

	union {
		u32 reserved_padding;
	};
} CPUIDFeatures;

typedef struct {
	union {
		CPUID_AuthenticAMD *amd;
		CPUID_GenuineIntel *intel;
		void               *cpu;
	};
	CPUIDVendorID id;
	u32           has_cpuid;
} CPUIDFull;

typedef struct {
	CPUIDFeatures features;
	usz           cache_l1i;
	usz           cache_l1d;
	usz           cache_l2;
	usz           cache_l3;
	usz           pcores;
	usz           lcores;
} CPUID;

#endif // INCLUDE_NOSTD_COMMON_CPUID_H

