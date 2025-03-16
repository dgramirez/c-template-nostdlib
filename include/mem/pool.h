#ifndef INCLUDE_MEM_POOL_H
#define INCLUDE_MEM_POOL_H

#include "define.h"
#include "platform.h"

typedef struct __mpool_node {
	struct __mpool_node *next;
} MPoolNode;

typedef struct {
	u8 *start;
	u8 *end;
	MPoolNode *head;
	usz chunk_len;
} MPool;

local void
mpool_reset(MPool *pool)
{
	MPoolNode *c;
	usz chunks;

	pool->head = 0;
	chunks = ((pool->end - pool->start) / pool->chunk_len) - 1;
	do {
		c = (MPoolNode *)(pool->start + (chunks * pool->chunk_len));
		c->next = pool->head;
		pool->head = c;
	} while (chunks--);
}

local void
mpool_init(MPool *out,
           void *buf,
           usz len,
           usz chunk_len,
           usz align)
{
	usz pad;

	if (!is_pow2(align))
		assert(0, "Align variable is NOT power of 2!");

	pad = align_pad(buf, align);
	out->start = (u8 *)buf + pad;
	out->end = out->start + len - pad;
	assert((out->end - out->start) >= sizeof(MPoolNode),
	       "Buffer size is too small to fit one node in this pool!");

	pad = align_pad(chunk_len, align);
	out->chunk_len = chunk_len + pad;
	assert((usz)(out->end - out->start) >= out->chunk_len,
	       "Buffer size is too small to fit one chunk in this pool!");

	mpool_reset(out);
}

local void *
mpool_alloc(MPool *pool)
{
	MPoolNode *n;

	n = pool->head;
	if (!n)
		return 0;

	pool->head = n->next;
	memzerou(n, pool->chunk_len);
	return n;
}

local void
mpool_free(MPool *pool,
           void *addr)
{
	MPoolNode *n;

	n = (MPoolNode *)addr;
	if (!n)
		return;

	if ((u8 *)n < pool->start || (u8 *)n >= pool->end) {
		assert(0, "Address is outside of this pool!");
		return;
	}

	n->next = pool->head;
	pool->head = n;
}

#endif // INCLUDE_MEM_POOL_H

