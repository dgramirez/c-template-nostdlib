#ifndef INCLUDE_PLATFORM_LINUX_THREADPOOL_QUEUE_H
#define INCLUDE_PLATFORM_LINUX_THREADPOOL_QUEUE_H

// Note: This uses the Michael-Scott Non-Blocking Queue Algorithm.
//       If used correctly, this works very well for multiprocessors with
//       and without contention. Below are two resources on this algorithm
//
// 1: https://www.cs.rochester.edu/research/synchronization/pseudocode/queues.html
// 2: https://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf
//
// Note 2: If any of the above links don't work, try a web archiver. It
//         should find at least the 1st one (html).

typedef struct _thread_items {
	MArena   tmp;
	MCSMutex mtx;
} ThreadItems;

typedef void *TpJobHandle;
typedef void (*TpJobFn)(void *arg, ThreadItems *);

typedef enum {
	TP_POST_EXIT_IF_FAILED    = 0x1,
	TP_POST_IN_FRONT_OF_QUEUE = 0x2
} TpFlags;

struct _job_node;
typedef struct _job_ptr {
	struct _job_node *ptr;
	usz               count;
} TpJobPtr;

#define TpJobPtr(x) \
	(TpJobPtr){ (x).ptr, (x).count }
#define TpJobPtr_Neq(x, y) \
	(!(x.ptr == y.ptr || x.count == y.count))
#define TpJobPtr_SwapNode(p, o, n) \
	atomic_cmpxchg(p.ptr, (isz)o.ptr, (isz)n)
#define TpJobPtr_SwapPtr(p, o, n) \
	atomic_cmpxchg(p.ptr, (isz)o.ptr, (isz)n.ptr)

typedef struct _job_node {
	TpJobFn   fn;
	void     *args;
	TpJobPtr  next;
} TpJobNode;

typedef struct {
	TpJobPtr  head;
	TpJobPtr  tail;
	MPool    *pool;
	u32      *futex_jobavail;
} TpJobQueue;

local void
queue_tpjob_init(TpJobQueue *q,
                 MPool *p,
                 u32 *futex_jobavail)
{
	TpJobNode *node;

	q->futex_jobavail = futex_jobavail;
	q->pool = p;
	node = (TpJobNode *)mpool_alloc(p);

	q->head.ptr   = node;
	q->head.count = 0;

	q->tail.ptr   = node;
	q->tail.count = 0;
}

local TpJobHandle
queue_tpjob_enqueue(TpJobQueue *q,
                    TpJobFn fn,
                    void *args,
                    TpFlags flags)
{
	TpJobNode *node;
	TpJobPtr   tail;
	TpJobPtr   next;
	isz swap;

	node = (TpJobNode *)mpool_alloc(q->pool);
	while (!node) {
		if (flag_has(flags, TP_POST_EXIT_IF_FAILED))
			return (TpJobHandle)0;

		atomic_store(q->futex_jobavail, 0);
		futex_wait(q->futex_jobavail, 1);
	}

	node->fn         = fn;
	node->args       = args;
	node->next.ptr   = 0;
	node->next.count = 0;

	do {
		tail = TpJobPtr(q->tail);
		next = TpJobPtr(q->tail.ptr->next);

		if (TpJobPtr_Neq(tail, q->tail))
			continue;

		if (!next.ptr) {
			swap = TpJobPtr_SwapNode(&tail.ptr->next, next, node);
			if (swap == (isz)next.ptr) {
				atomic_inc(&next.count);
				break;
			}
		}
		else {
			swap = TpJobPtr_SwapPtr(&q->tail, tail, next);
			if (swap == (isz)tail.ptr)
				atomic_inc(&tail.count);
		}
	} while(1);

	swap = TpJobPtr_SwapNode(&q->tail, tail, node);
	if (swap == (isz)tail.ptr)
		atomic_inc(&tail.count);

	return node;
}

local i32
queue_tpjob_dequeue(TpJobQueue *q,
                    TpJobNode **job)
{
	TpJobPtr head;
	TpJobPtr tail;
	TpJobPtr next;
	isz swap;

	do {
		head = TpJobPtr(q->head);
		tail = TpJobPtr(q->tail);
		next = TpJobPtr(q->head.ptr->next);

		if (TpJobPtr_Neq(head, q->head))
			continue;

		if (head.ptr == tail.ptr) {
			if (!next.ptr) {
				*job = 0;
				return 0;
			}

			swap = TpJobPtr_SwapPtr(&q->tail, tail, next);
			if (swap == (isz)tail.ptr)
				atomic_inc(&tail.count);
		}
		else {
			*job = next.ptr;

			swap = TpJobPtr_SwapPtr(&q->head, head, next);
			if (swap == (isz)head.ptr) {
				atomic_inc(&head.count);
				break;
			}
		}
	} while (1);

	return 1;
}

#endif // INCLUDE_PLATFORM_LINUX_THREADPOOL_QUEUE_H

