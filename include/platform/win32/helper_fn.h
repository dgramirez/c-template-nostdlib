#ifndef INCLUDE_PLATFORM_WIN32_HELPER_FN_H
#define INCLUDE_PLATFORM_WIN32_HELPER_FN_H

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

local i32
Win32WriteFile(fb8 *b)
{
	DWORD written;
	return WriteFile(b->fd, b->data, (DWORD)b->len, &written, 0);
}

local b8
Win32CpuidGetVendor(u8 *buffer,
                    usz len)
{
	b8 b;
	u32 *bptr;

	if (len < 12)
		return s8("");
	b.data = buffer;
	b.len  = 12;

	unsigned int eax, ebx, ecx, edx;
	eax = 0;
	cpuid_native(&eax, &ebx, &ecx, &edx);

	bptr  = (u32 *)&b.data[0];
	*bptr = ebx;

	bptr  = (u32 *)&b.data[4];
	*bptr = edx;

	bptr  = (u32 *)&b.data[8];
	*bptr = ecx;
	if (len >= 16) {
		bptr  = (u32 *)&b.data[12];
		*bptr = 0;
	}

	return b;
}

// Note: THIS ISNT A SYSCALL, ITS AN ASM CALL!
//       I will find a place for this in a later date!
extern void
cpuid_native(unsigned int *eax,
             unsigned int *ebx,
             unsigned int *ecx,
             unsigned int *edx);

#endif // INCLUDE_PLATFORM_WIN32_HELPER_FN_H

