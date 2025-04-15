#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

typedef struct {
	MArena    tmp;
	MCSMutex  mtx;
	void     *q_retired_nodes;
	i32       q_retired_max;
	i32       q_retired_current;
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

#define AppJobPtr_Swap(p, o, n) \
	atomic_cas128(p, o, n)

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
	u32       *addr_jobposted;
} AppJobQueue;

local void
queue_appjob_init(AppJobQueue   *q,
                  MPool         *p,
				  u32           *addr_jobavail,
                  u32           *addr_jobposted)
{
	AppJobNode *node;

	q->addr_jobavail  = addr_jobavail;
	q->addr_jobposted = addr_jobposted;

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
	AppJobPtr   new_next;
	AppJobPtr   new_tail;

	node = (AppJobNode *)mpool_alloc(q->pool);
	while (!node) {
		if (flag_has(flags, TP_POST_EXIT_IF_FAILED))
			return (HAppJob)0;

		atomic_store32(q->addr_jobavail, 0);
		__thread_wait(q->addr_jobavail, 0);
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
			new_next.ptr   = node;
			new_next.count = next.count + 1;
			if (AppJobPtr_Swap(&tail.ptr->next, &next, &new_next))
				break;
		}
		else {
			new_tail.ptr = next.ptr;
			new_tail.count = tail.count + 1;
			AppJobPtr_Swap(&q->tail, &tail, &new_tail);
		}
	} while(1);

	new_tail.ptr = node;
	new_tail.count = tail.count + 1;
	AppJobPtr_Swap(&q->tail, &tail, &new_tail);

	if (!atomic_load32(q->addr_jobposted)) {
		atomic_store32(q->addr_jobposted, 1);
		__thread_wake_one(q->addr_jobposted);
	}

	return node;
}

local i32
queue_appjob_dequeue(AppJobQueue *q,
                     AppJobNode  **job)
{
	AppJobPtr   head;
	AppJobPtr   tail;
	AppJobPtr   next;
	AppJobPtr   new_head;
	AppJobPtr   new_tail;
	AppJobNode *current_job;

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

			new_tail.ptr = next.ptr;
			new_tail.count = tail.count + 1;
			AppJobPtr_Swap(&q->tail, &tail, &new_tail);
		}
		else {
			current_job = next.ptr;

			new_head.ptr = next.ptr;
			new_head.count = head.count + 1;
			if (AppJobPtr_Swap(&q->head, &head, &new_head)) {
				*job = current_job;
				break;
			}
		}
	} while (1);

	// Note: We will free the node outside of here, inside the threadpool.
	//       This is due to each thread having their own retirement buffers.
	return 1;
}

local void
queue_appjob_retire(MPool            *qpool,
                    AppJobNode       *node,
                    ThreadAppJobData *tdata)
{
	int i;
	AppJobNode **retired_nodes;

	retired_nodes = (AppJobNode **)tdata->q_retired_nodes;
	if (tdata->q_retired_current == tdata->q_retired_max) {
		for (i = 0; i < tdata->q_retired_max; ++i)
			mpool_free(qpool, retired_nodes[i]);

		tdata->q_retired_current = 0;
	}

	retired_nodes[tdata->q_retired_current++] = node;
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

