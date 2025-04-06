#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

typedef struct {
	MArena   tmp;
	MCSMutex mtx;
} ThreadAppJobData;

typedef void *HAppJob;
typedef void (*AppJobFn)(void *, ThreadAppJobData *);

typedef enum {
	TP_POST_EXIT_IF_FAILED    = 0x1,
	TP_POST_IN_FRONT_OF_QUEUE = 0x2
} AppJobFlags;

struct _app_job_node;
typedef struct _app_job_ptr {
	struct _app_job_node *ptr;
	usz                   count;
} AppJobPtr;

#define AppJobPtr(x) \
	(AppJobPtr){ (x).ptr, (x).count }

#define AppJobPtr_Neq(x, y) \
	(!(x.ptr == y.ptr || x.count == y.count))

#define AppJobPtr_SwapNode(p, o, n) \
	atomic_cmpxchg(p.ptr, (isz)o.ptr, (isz)n)

#define AppJobPtr_SwapPtr(p, o, n) \
	atomic_cmpxchg(p.ptr, (isz)o.ptr, (isz)n.ptr)

typedef struct _app_job_node {
	AppJobFn   fn;
	void      *args;
	AppJobPtr  next;
} AppJobNode;

typedef struct {
	AppJobPtr  head;
	AppJobPtr  tail;
	MPool     *pool;
	u32       *addr_jobavail;
} AppJobQueue;

local void
queue_appjob_init(AppJobQueue   *q,
                  MPool         *p,
                  u32           *addr_jobavail)
{
	AppJobNode *node;

	q->addr_jobavail = addr_jobavail;
	q->pool = p;
	node = (AppJobNode *)mpool_alloc(p);

	q->head.ptr   = node;
	q->head.count = 0;

	q->tail.ptr   = node;
	q->tail.count = 0;
}

local HAppJob
queue_appjob_enqueue(AppJobQueue *q,
                     AppJobFn     fn,
                     void        *args,
                     AppJobFlags flags)
{
	AppJobNode *node;
	AppJobPtr   tail;
	AppJobPtr   next;
	isz         swap;

	node = (AppJobNode *)mpool_alloc(q->pool);
	while (!node) {
		if (flag_has(flags, TP_POST_EXIT_IF_FAILED))
			return (HAppJob)0;

		atomic_store(q->addr_jobavail, 0);
		__thread_wait(q->addr_jobavail, 1);
	}

	node->fn         = fn;
	node->args       = args;
	node->next.ptr   = 0;
	node->next.count = 0;

	do {
		tail = AppJobPtr(q->tail);
		next = AppJobPtr(q->tail.ptr->next);

		if (AppJobPtr_Neq(tail, q->tail))
			continue;

		if (!next.ptr) {
			swap = AppJobPtr_SwapNode(&tail.ptr->next, next, node);
			if (swap == (isz)next.ptr) {
				atomic_inc(&next.count);
				break;
			}
		}
		else {
			swap = AppJobPtr_SwapPtr(&q->tail, tail, next);
			if (swap == (isz)tail.ptr)
				atomic_inc(&tail.count);
		}
	} while(1);

	swap = AppJobPtr_SwapNode(&q->tail, tail, node);
	if (swap == (isz)tail.ptr)
		atomic_inc(&tail.count);

	return node;
}

local i32
queue_appjob_dequeue(AppJobQueue *q,
                     AppJobNode **job)
{
	AppJobPtr head;
	AppJobPtr tail;
	AppJobPtr next;
	isz swap;

	do {
		head = AppJobPtr(q->head);
		tail = AppJobPtr(q->tail);
		next = AppJobPtr(q->head.ptr->next);

		if (AppJobPtr_Neq(head, q->head))
			continue;

		if (head.ptr == tail.ptr) {
			if (!next.ptr) {
				*job = 0;
				return 0;
			}

			swap = AppJobPtr_SwapPtr(&q->tail, tail, next);
			if (swap == (isz)tail.ptr)
				atomic_inc(&tail.count);
		}
		else {
			*job = next.ptr;

			swap = AppJobPtr_SwapPtr(&q->head, head, next);
			if (swap == (isz)head.ptr) {
				atomic_inc(&head.count);
				break;
			}
		}
	} while (1);

	return 1;
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

