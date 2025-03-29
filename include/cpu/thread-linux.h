#ifndef INCLUDE_CPU_THREAD_H
#define INCLUDE_CPU_THREAD_H

typedef void (*FnEntry)(void *);
typedef struct __attribute((aligned(16))) stack_head {
	void (*entry)(struct stack_head *);

	// Thread Local Data
	FnEntry fn_entry;
	void *args;
	u32 join_futex;
} StackHead;

local long
futex_wait(u32 *futex)
{
	return sys_futex((u32 *)futex, FUTEX_WAIT, 0, 0, 0, 0);
}

local long
futex_wake(u32 *futex)
{
	return sys_futex((u32 *)futex, FUTEX_WAKE, 0, 0, 0, 0);
}

local void
thread_entry(struct stack_head *h)
{
	h->fn_entry(h->args);
	__atomic_store_n(&h->join_futex, 1, __ATOMIC_SEQ_CST);
	futex_wake(&h->join_futex);
	sys_exit(0);
}

local long
create_thread(StackHead *s,
              FnEntry entry,
			  void *args)
{
	s->entry = thread_entry;
	s->fn_entry = entry;
	s->args = args;
	s->join_futex = 0;

	return sys_clone(0x50f00, s, 0, 0, 0);
}

//typedef void (*pfn_entry)(struct stack_head *);
//
//local void
//thread_entry(StackHead *stack)
//{
//	lock(A)
//	while (!check_value(V)) {
//		unlock(A);
//		block_on(B);
//		lock(A);
//	};
//	unlock(A);
//}
//
//local long
//new_thread(pfn_entry entry,
//           usz stack_size)
//{
//	usz pad;
//
//	if (stack_size < KB(4))
//		stack_size = KB(4);
//
//	pad = align_pad(stack_size, KB(4));
//	stack_size += pad;
//
//	StackHead *stack = (StackHead *)mmap_anon(stack_size);
//	if ((isz)stack > -4096)
//		return -0;
//
//	stack += (stack_size / sizeof(StackHead)) - 1;
//	stack->entry = entry;
//	stack->futex = 0;
//
//	return sys_clone(0x50f00, 0, 0, 0, 0);
//}

#endif // INCLUDE_CPU_THREAD_H

