#include "mem/s8.h"
#include "mem/memfn.h"
#include "main-win32.c"

#ifdef _ZIG
	int _tls_index = 0;
#endif
	
#ifdef _MSC_VER
	int _fltused;
#endif

local void
Win32ParseCmdLine(int    *argc,
                  s8     *argv,
                  int   argmax,
                  char *buffer,
                  usz   buflen);

void
_start() {
	char buffer[page_size];
	s8  argv[10];
	int argc;
	int exit;
	int i;
	int isdebug;

	Win32ParseCmdLine(&argc, &argv[0], 10, buffer, page_size);

	i = argc;
	isdebug = 0;
	while(i--) {
		if (argv[i].len == 5 && memequ(argv[i].data, "debug", 5))
			isdebug = 1;
	}

	if (isdebug) AllocConsole();

	exit = Win32Main(argc, argv);
	ExitProcess(exit);
}

local void
Win32ParseCmdLine(int    *argc,
                  s8     *argv,
                  int   argmax,
                  char *buffer,
                  usz   buflen)
{
	s8 cmdline;
	u8 *delimiter;
	int i;

	cmdline.data = (u8*)GetCommandLineA();
	cmdline.len  = c_strlen((char*)cmdline.data);
	delimiter = s8_chr(cmdline, ' ', cmdline.len);
	if (!delimiter) {
		*argc = 1;
		argv[0].data = cmdline.data;
		argv[0].len  = cmdline.len; 
		return;
	}

	memzerou(buffer, buflen);
	buflen = imin(cmdline.len, buflen-1);
	memcpyu(buffer, cmdline.data, buflen);

	i = 0;
	cmdline.data = (u8 *)buffer;
	cmdline.len  = c_strlen((char*)cmdline.data);
	while (argmax && cmdline.len) {
		if (cmdline.data[0] < ' ') {
			cmdline.data += 1;
			cmdline.len  -= 1;
			continue;
		}

		switch(cmdline.data[0]) {
			case ' ': {
				
			} break;

			case '\'': {
				cmdline.data += 1;
				cmdline.len  -= 1;

				delimiter = s8_chr(cmdline, '\'', cmdline.len);
				assert(delimiter, "Failed to parse arguments with: '\n");

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;

				*delimiter     = 0;
				*(delimiter+1) = 0;

				cmdline.len -= argv[i].len + 2;
				cmdline.data = delimiter + 2;
			} break;

			case '"': {
				cmdline.data += 1;
				cmdline.len  -= 1;

				delimiter = s8_chr(cmdline, '"', cmdline.len);
				assert(delimiter, "Failed to parse arguments with: \"\n");

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;

				*delimiter     = 0;
				*(delimiter+1) = 0;

				cmdline.len -= argv[i].len + 2;
				cmdline.data = delimiter + 2;
			} break;
			
			default: {
				delimiter = s8_chr(cmdline, ' ', cmdline.len);
				if (!delimiter)
					delimiter = cmdline.data + cmdline.len;

				argv[i].data = cmdline.data;
				argv[i].len  = delimiter - cmdline.data;
				*delimiter = 0;

				cmdline.len -= argv[i].len + 1;
				cmdline.data = delimiter + 1;
			}
		}

		i++;
		argmax--;
	}

	*argc = i+1;
}


#if defined(_MSC_VER)
	#pragma function(memset)
	void *memset(void *dst, int c, size_t count)
	{
		#if EXE_ARCH == 32
			usz _c = (usz)c;
		#else
			usz _c = (usz)c | ((usz)c << 32);
		#endif
		memsetu(dst, _c, count);
		return dst;
	}
	
	#pragma function(memcpy)
	void *memcpy(void *dst, const void *src, size_t count)
	{
		memcpyu(dst, (void*)src, count);
		return dst;
	}

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

