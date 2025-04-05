#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H

#ifdef _WIN32
	#define __thread_wait(addr, val)  Win32AddressWait(addr, val)
#else
	#define __thread_wait(addr, val) futex_wait(x, val)
#endif

#ifdef _WIN32
	#define __thread_wake_one(addr) WakeByAddressSingle(addr)
#else
	#define __thread_wake_one(x) futex_wake(addr, 1)
#endif

#ifdef _WIN32
	#define __thread_wake_all(addr) WakeByAddressAll(addr)
#else
	#define __thread_wake_all(x) futex_wake(addr, 0x7FFFFFFF)
#endif

#ifdef _WIN32
	#define __thread_create(start_addr, arg, stack_size) \
		CreateThread(0, stack_size, start_addr, arg, 0, 0);
#else
	#define __thread_create(start_addr, arg, stack_size) \
		sys_clone(0x50f00, arg, 0, 0, 0); 
#endif

#ifdef _WIN32
	#define __thread_exit() ExitThread(0);
#else
	#define __thread_exit() sys_exit(0); 
#endif

#ifdef _WIN64
	#define __thread_return DWORD
#elif defined(_WIN32)
	#define __thread_return DWORD
#else
	#define __thread_return void
#endif

#if defined(_WIN32)
	#ifndef _ZIG
		#define __atomic_asm_prefix __fastcall
	#else
		#define __atomic_asm_prefix
	#endif
#else
	#define __atomic_asm_prefix
#endif

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_DEFINE_H

