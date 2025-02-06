#ifndef INCLUDE_CPU_THREAD_H
#define INCLUDE_CPU_THREAD_H

#include "define.h"
#include "platform.h"
#include "asm/linux-syscall.h"
#include <linux/futex.h>

typedef struct __attribute((aligned(16))) stack_head {
	void (*entry)(struct stack_head *);
	int futex;
} StackHead;

typedef void (*pfn_entry)(struct stack_head *);

local void
thread_entry(StackHead *stack)
{
	lock(A)
	while (!check_value(V)) {
		unlock(A);
		block_on(B);
		lock(A);
	};
	unlock(A);
}

local long
new_thread(pfn_entry entry,
           usz stack_size)
{
	usz pad;

	if (stack_size < KB(4))
		stack_size = KB(4);

	pad = align_pad(stack_size, KB(4));
	stack_size += pad;

	StackHead *stack = (StackHead *)mmap_anon(stack_size);
	if ((isz)stack > -4096)
		return -0;

	stack += (stack_size / sizeof(StackHead)) - 1;
	stack->entry = entry;
	stack->futex = 0;

	return sys_clone(0x50f00, 0, 0, 0, 0);
}

#endif // INCLUDE_CPU_THREAD_H

