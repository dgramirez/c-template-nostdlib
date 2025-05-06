#ifndef INCLUDE_PLATFORM_WIN32_HELPER_FN_H
#define INCLUDE_PLATFORM_WIN32_HELPER_FN_H

// Note: THIS IS AN ASM CALL!
//       I will find a place for this in a later date!
extern void
cpuid_native(unsigned int *eax,
             unsigned int *ebx,
             unsigned int *ecx,
             unsigned int *edx);

local i32
Win32WriteFile(fb8 *b)
{
	DWORD written;
	return WriteFile(b->fd, b->data, (DWORD)b->len, &written, 0);
}

local i32
Win32WriteToConsole(void *b, usz len) {
	HANDLE out;
	DWORD written;

	out = GetStdHandle(STD_OUTPUT_HANDLE);
	return WriteFile(out, b, (DWORD)len, &written, 0);
}

local long
Win32AddressWait(u32 *addr, u32 expected)
{
	u32 val;

	val = expected;
	while (val == expected) {
		WaitOnAddress(addr, &val, sizeof(u32), INFINITE);
		val = *addr;
	}

	return 0;
}

local long
Win32WakeByAddressSingle(u32 *addr) {
	WakeByAddressSingle(addr);
	return 0;
}

local long
Win32WakeByAddressAll(u32 *addr) {
	WakeByAddressAll(addr);
	return 0;
}

local void *
Win32ThreadCreate(void *addr,
                  void *args,
                  usz stack_size)
{
	LPTHREAD_START_ROUTINE a = (LPTHREAD_START_ROUTINE)addr;
	return CreateThread(0, stack_size, a, args, 0, 0);
}

#endif // INCLUDE_PLATFORM_WIN32_HELPER_FN_H

