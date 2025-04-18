#ifndef SRC_APP_LIBAPP_H
#define SRC_APP_LIBAPP_H

#include "nostd.h"

global PlatformData *g_platform;
global AppLock       tlock_terminal;
global MArena        appmem;

#if EXE_ARCH == 32
	#define APP_USZ_VALUE 1234567890
	#define APP_ISZ_VALUE -1876543290
#else
	#define APP_USZ_VALUE 12345678901234567
	#define APP_ISZ_VALUE -9876543210987654
#endif

local void
show_log_debug(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_debug("Debug: I am debugging the log defines.");
}

local void
show_log_info(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_info("We're inside libapp.c. "
	          "This is the application portion of this project!");
}

local void
show_log_pass(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_pass("And so far, log_debug and log_info has passed!");
}

local void
show_log_odd(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_odd("However, its kinda weird that I'm logging at different "
	         "levels. Of course its a test but you don't do this normally.");
}

local void
show_log_warn(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_warn("So a warning: We will see two errors occur. "
	          "Only here, would the errors not truly matter!");
}

local void
show_log_error(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_error("I'm an error, but not the one that would force a close on "
	           "your application.");
}

local void
show_log_fatal(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_fatal("I'm the fatal error. By myself, I am like the regular "
	           "logc_error. However, Devs should terminate the program once "
	           "I have been used! In the end, its totally up to them.");
}

local void
show_log_egg(void *arg, TPData *tp_data)
{
	unref(arg);
	unref(tp_data);
	logc_egg("And I'm an Easter Egg! or Dev Notes... Or whatever they want "
	"me to be. If I am supposed to be useful, I should be in "
	"logc_info. This is just meant for cool or fun info the devs "
	"want to show out.");
}

#endif // SRC_APP_LIBAPP_H

