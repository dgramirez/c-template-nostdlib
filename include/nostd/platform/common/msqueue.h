#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

typedef struct {
	void     *nodes;
	i32       max;
	i32       current;
} MSQueueRetiredNodes;

typedef struct {
	MArena              tmp;
	MCSMutex            mtx;
	MSQueueRetiredNodes qr;
} ThreadAppJobData;

typedef void *HAppJob;
typedef void (*AppJobFn)(void *, ThreadAppJobData *);

typedef enum {
	TP_POST_EXIT_IF_FAILED    = 0x1,
	TP_POST_IN_FRONT_OF_QUEUE = 0x2
} MSQueueFlags;

struct _app_job_node;
typedef struct _app_job_ptr {
	struct _app_job_node *ptr;
	usz                   count;
} MSQueuePtr;

#define MSQueuePtr(x) \
	(MSQueuePtr){ (x).ptr, (x).count }

#define MSQueuePtr_Neq(x, y) \
	(!(x.ptr == y.ptr || x.count == y.count))

#define MSQueuePtr_Swap(p, o, n) \
	_afn_atcasD(p, o, n)

typedef struct _app_job_node {
	AppJobFn   fn;
	void      *args;
	MSQueuePtr  next;
} MSQueueNode;

typedef struct {
	MSQueuePtr  head;
	MSQueuePtr  tail;
	MPool     *pool;
	u32       *addr_jobavail;
	u32       *addr_jobposted;
} MSQueue;

local void
msqueue_init(MSQueue       *q,
             MPool         *p,
			 u32           *addr_jobavail,
             u32           *addr_jobposted)
{
	MSQueueNode *node;

	q->addr_jobavail  = addr_jobavail;
	q->addr_jobposted = addr_jobposted;

	q->pool = p;
	node = (MSQueueNode *)mpool_alloc(p);

	q->head.ptr   = node;
	q->head.count = 0;

	q->tail.ptr   = node;
	q->tail.count = 0;
}

local HAppJob
msqueue_enqueue(MSQueue       *q,
                AppJobFn      fn,
                void         *args,
                MSQueueFlags  flags)
{
	MSQueueNode *node;
	MSQueuePtr   tail;
	MSQueuePtr   next;
	MSQueuePtr   new_next;
	MSQueuePtr   new_tail;

	node = (MSQueueNode *)mpool_alloc(q->pool);
	while (!node) {
		if (flag_has(flags, TP_POST_EXIT_IF_FAILED))
			return (HAppJob)0;

		_afn_atstoreI(q->addr_jobavail, 0);
		thread_wait(q->addr_jobavail, 0);
	}

	node->fn         = fn;
	node->args       = args;
	node->next.ptr   = 0;
	node->next.count = 0;

	do {
		tail = MSQueuePtr(q->tail);
		next = MSQueuePtr(q->tail.ptr->next);

		if (MSQueuePtr_Neq(tail, q->tail))
			continue;

		if (!next.ptr) {
			new_next.ptr   = node;
			new_next.count = next.count + 1;
			if (MSQueuePtr_Swap(&tail.ptr->next, &next, &new_next))
				break;
		}
		else {
			new_tail.ptr = next.ptr;
			new_tail.count = tail.count + 1;
			MSQueuePtr_Swap(&q->tail, &tail, &new_tail);
		}
	} while(1);

	new_tail.ptr = node;
	new_tail.count = tail.count + 1;
	MSQueuePtr_Swap(&q->tail, &tail, &new_tail);

	if (!_afn_atloadI(q->addr_jobposted)) {
		_afn_atstoreI(q->addr_jobposted, 1);
		thread_wake_one(q->addr_jobposted);
	}

	return node;
}

local i32
msqueue_dequeue(MSQueue      *q,
                MSQueueNode **job)
{
	MSQueuePtr   head;
	MSQueuePtr   tail;
	MSQueuePtr   next;
	MSQueuePtr   new_head;
	MSQueuePtr   new_tail;
	MSQueueNode *current_job;

	do {
		head = MSQueuePtr(q->head);
		tail = MSQueuePtr(q->tail);
		next = MSQueuePtr(q->head.ptr->next);

		if (MSQueuePtr_Neq(head, q->head))
			continue;

		if (head.ptr == tail.ptr) {
			if (!next.ptr) {
				*job = 0;
				return 0;
			}

			new_tail.ptr = next.ptr;
			new_tail.count = tail.count + 1;
			MSQueuePtr_Swap(&q->tail, &tail, &new_tail);
		}
		else {
			current_job = next.ptr;

			new_head.ptr = next.ptr;
			new_head.count = head.count + 1;
			if (MSQueuePtr_Swap(&q->head, &head, &new_head)) {
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
msqueue_retire(MPool               *qpool,
               MSQueueNode         *node,
               MSQueueRetiredNodes *qr)
{
	int i;
	MSQueueNode **retired_nodes;

	retired_nodes = (MSQueueNode **)qr->nodes;
	if (qr->current == qr->max) {
		for (i = 0; i < qr->max; ++i)
			mpool_free(qpool, retired_nodes[i]);

		qr->current = 0;
	}

	retired_nodes[qr->current++] = node;
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_QUEUE_APPJOB_H

