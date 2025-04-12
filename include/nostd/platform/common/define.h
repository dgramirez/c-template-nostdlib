#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H

#ifdef _WIN32
	#define __thread_return DWORD

	#define __thread_wait(addr, val)  Win32AddressWait(addr, val)
	#define __thread_wake_one(addr) WakeByAddressSingle(addr)
	#define __thread_wake_all(addr) WakeByAddressAll(addr)
	#define __thread_create(start_addr, arg, stack_size) \
		CreateThread(0, stack_size, start_addr, arg, 0, 0);
	#define __thread_exit() ExitThread(0);

	#ifndef _ZIG
		#define __atomic_asm_prefix __fastcall
	#else
		#define __atomic_asm_prefix
	#endif

#elif __linux__
	#define __thread_return void

	#define __thread_wait(addr, val) futex_wait(addr, val)
	#define __thread_wake_one(addr) futex_wake(addr, 1)
	#define __thread_wake_all(addr) futex_wake(addr, 0x7FFFFFFF)
	#define __thread_create(start_addr, arg, stack_size) \
		sys_clone(0x50f00, arg, 0, 0, 0); 
	#define __thread_exit() sys_exit(0); 
	#define __atomic_asm_prefix
	#define __stdcall
#endif

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H

