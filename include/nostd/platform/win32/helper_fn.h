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

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
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
#else
	// NOTE: Stubbed
	local long
	Win32AddressWait(u32 *addr, u32 expected)
	{
		unref(addr);
		unref(expected);
		return 0;
	}
	
	local long
	Win32WakeByAddressSingle(u32 *addr) {
		unref(addr);
		return 0;
	}
	
	local long
	Win32WakeByAddressAll(u32 *addr) {
		unref(addr);
		return 0;
	}
	
	local void *
	Win32ThreadCreate(void *addr,
	                  void *args,
	                  usz stack_size)
	{
		unref(addr);
		unref(args);
		unref(stack_size);
		return 0;
	}

#endif
local usz
Win32GetFreq(void)
{
	LARGE_INTEGER f = {0};
	QueryPerformanceFrequency(&f);

#if ARCH_EXE == 64
	return f.QuadPart;
#else
	return f.LowPart;
#endif
}

local usz
Win32GetCounter(void)
{
	LARGE_INTEGER c = {0};
	QueryPerformanceCounter(&c);

#if ARCH_EXE == 64
	return c.QuadPart;
#else
	return c.LowPart;
#endif
}

#endif // INCLUDE_PLATFORM_WIN32_HELPER_FN_H

