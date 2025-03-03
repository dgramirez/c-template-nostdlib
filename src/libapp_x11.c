#include "define.h"
#include "platform.h"
#include "mem/s8.h"
#include "asm/linux-syscall.h"
#include <X11/Xlib.h>

local     int g_ivar = 0;
external long g_lvar = 0;

int
increment_ivar()
{
	return ++g_ivar;
}

long
increment_lvar()
{
	return ++g_lvar;
}

local void
init(fb8 *b, pfn_os_write fn_os_write)
{
	s8 str;

	os_write = fn_os_write;

	str = s8("libtest: init");
	fb8_append(b, str);
	fb8_append_lf(b);
}

void
close(fb8 *b)
{
	s8 str;

	str = s8("libtest: close");
	fb8_append(b, str);
	fb8_append_lf(b);
}

void
open_window()
{
	Display *d;
	Window w;
	XEvent e;
	const char *msg = "Hello, World!";
	int s;

	sys_write(0, (char*)msg, c_strlen(msg));

	d = XOpenDisplay(NULL);
	if (d == NULL) {
		return;
	}

	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
	                        BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, ExposureMask | KeyPressMask);
	XMapWindow(d, w);

	while (1) {
		XNextEvent(d, &e);
		if (e.type == Expose) {
			XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
			XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, c_strlen(msg));
		}
		if (e.type == KeyPress)
			break;
	}

	XCloseDisplay(d);
}

