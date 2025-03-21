#include "mem/s8.h"
#include "mem/memfn.h"
#include "main-win32.c"

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

