#ifndef INCLUDE_PLATFORM_LINUX_THREAD_H
#define INCLUDE_PLATFORM_LINUX_THREAD_H

/* Note: This is a sample of a thread stack when cloning a new thread.
   typedef struct _tp_stack {
   	void (*entry)(struct _tp_stack *);

	// Thread-Based Variables Below:
	u32 futex_join;
   } TpStack;
*/

local long
futex_wait(u32 *futex, u32 expected)
{
	return sys_futex((u32 *)futex, FUTEX_WAIT, expected, 0, 0, 0);
}

local long
futex_wake(u32 *futex, u32 wake_count)
{
	return sys_futex((u32 *)futex, FUTEX_WAKE, wake_count, 0, 0, 0);
}

#endif // INCLUDE_PLATFORM_LINUX_THREAD_H

