#ifndef INCLUDE_PLATFORM_APP_RELATIONSHIP_H
#define INCLUDE_PLATFORM_APP_RELATIONSHIP_H

// Note: To future self, this is just an example. This **should** be
//       expanded to solve "Platform-To-App" problems.
//
//       For Instance: A "very simple chat application" can use the buffer
//       provided by the platform layer to setup a send network buffer.
//       Then the app can use a "Platform Function" to give its buffer's
//       pointer to the platform. The platform will check to see if the
//       buffer "len" property is above 0, and if so it will attempt to
//       send that many bytes through the network.
//
//       On that note, the same setup can be used for a recv network buffer.
//       Follows the same procedure up to the platform function. This could
//       be a bit more complex, but the premise remains the same: The app
//       can check to see both how many messages sent from previous check
//       and parse through them (either all, bulk, or single) to update the
//       client.
typedef struct {
	b8                bufapp;
	PFN_logsz         logsz;
	PFN_logs8         logs8;
	PFN_os_write      os_write;
	PFN_cpuid_vendor  cpuid_vendor;
	PFN_mlock_init    mlock_init;
	PFN_mlock_acquire mlock_acquire;
	PFN_mlock_release mlock_release;
	AppLock           tlock_terminal;
	FdStdOut          std_out;
	i32               run_app;
} PlatformData;

#ifdef _MSC_VER
	int _fltused;

	#if EXE_ARCH == 32
		__declspec(naked) void _ftol2()
		{
			__asm {
				fistp qword ptr [esp-8]
				mov edx, [esp-4]
				mov eax, [esp-8]
				ret
			}
		}

		__declspec(naked) void _ftol2_sse()
		{
			__asm {
				fistp dword ptr [esp-4]
				mov eax, [esp-4]
				ret
			}
		}
	#endif
#endif

#ifdef _ZIG
	int _tls_index = 0;
#endif

#endif // INCLUDE_PLATFORM_APP_RELATIONSHIP_H

