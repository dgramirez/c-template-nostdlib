#ifndef MODULE_APP_INCLUDE_CROSS_APP_H
#define MODULE_APP_INCLUDE_CROSS_APP_H

typedef void *FdStdOut;

typedef struct {
	CPUID             cpuid;
	b8                bufapp;
	PFN_logsz         logsz;
	PFN_logs8         logs8;
	PFN_os_write      os_write;
	PFN_mlock_init    mlock_init;
	PFN_mlock_acquire mlock_acquire;
	PFN_mlock_release mlock_release;
	PFN_mlock_free    mlock_free;
	PFN_tp_post       tp_post;
	PFN_tp_wait       tp_wait;
	PFN_tp_quit       tp_quit;
	TLock             tlock_terminal;
	FdStdOut          std_out;
	i32               run_app;
	TPData           *tp_data;
} PlatformData;

#endif // MODULE_APP_INCLUDE_CROSS_APP_H

