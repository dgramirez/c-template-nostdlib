#include "platform/main-linux.c"

typedef enum {
	ENTRY_MEMORY_PROFILE_DETECT  = 0,
	ENTRY_MEMORY_PROFILE_MINIMUM,
	ENTRY_MEMORY_PROFILE_LOW,
	ENTRY_MEMORY_PROFILE_MEDIUM,
	ENTRY_MEMORY_PROFILE_HIGH,
	ENTRY_MEMORY_PROFILE_CUSTOM,
} EntryMemoryProfile;

int
entry(int    argc,
      char **argv);

local b8
setup_memory_profile(int profile);

