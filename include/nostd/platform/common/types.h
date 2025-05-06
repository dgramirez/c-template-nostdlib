#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_TYPES_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_TYPES_H

typedef enum {
	ENTRY_MEMORY_PROFILE_DETECT  = 0,
	ENTRY_MEMORY_PROFILE_MINIMUM,
	ENTRY_MEMORY_PROFILE_LOW,
	ENTRY_MEMORY_PROFILE_MEDIUM,
	ENTRY_MEMORY_PROFILE_HIGH,
	ENTRY_MEMORY_PROFILE_CUSTOM,
} EntryMemoryProfile;

typedef enum {
	MCSLOCK_FREE_NONE  = 0x0,
	MCSLOCK_FREE_LOCK  = 0x1,
	MCSLOCK_FREE_MUTEX = 0x2,
	MCSLOCK_FREE_ALL   = 0x3,
} MCSLockFreeFlags;

typedef struct {
	fb8    fb_terminal;
	fb8    fb_file;
	fb8    fb_network;
	usz    flags_terminal;
	usz    flags_file;
	usz    flags_network;
} Logger;

typedef struct _mcs_lock {
	struct _mcs_lock *next;
	u32 locked;
#if ARCH_EXE == 64
	u32 reserved;
#endif
} MCSLock;

typedef struct {
	MCSLock me;
	MCSLock *lock;
} MCSMutex;

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_TYPES_H

