/**************************************************************************
 * Name:   Memory Freelist (MFreelist, mfreelist.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_MFREELIST__
 *     - Use this define above for this file only.
 *
 * License:
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org/>
 */

////////////////////////
// Standalone Defines //
////////////////////////
#ifndef __nostd_u8_t
	#define __nostd_u8_t unsigned char
#endif

#ifndef __nostd_usz_t
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define __nostd_usz_t unsigned int
	#else
		#define __nostd_usz_t unsigned long long
	#endif
#endif

#ifndef __nostd_api
	#define __nostd_api
#endif

#ifndef __nostd_assert
	#define __nostd_assert(x, msg)
#endif

#ifndef __nostd_is_pow2
	#define __nostd_is_pow2(x) ((x) && !((x) & ((x) - 1)))
#endif

#ifndef __nostd_align_pad
	#ifndef __nostd_align_over
		#define __nostd_align_over(p, a) ((__nostd_usz_t)(p) & ((a) - 1))
	#endif
	#define __nostd_align_pad(p, a) \
		(((a) - __nostd_align_over(p, a)) & ((a) - 1))
#endif

#ifndef __nostd_memzero
	#define __nostd_memzero(dst, size) memset(dst, 0, size)
#endif

#define __mfreelist_header_size sizeof(MFLNode)
#define __mfreelist_footer_size sizeof(__nostd_usz_t)
#define __mfreelist_block_size(size) (__mfreelist_header_size + \
                                        size + \
                                      __mfreelist_footer_size)
/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef enum {
	MFREELIST_BLACK = 0,
	MFREELIST_RED   = 1
} MFreelistColor;

typedef struct __mfl_node {
	struct __mfl_node *left;
	struct __mfl_node *right;
	struct __mfl_node *parent;
	__nostd_usz_t      len;
} MFLNode;

typedef struct {
	MFLNode       *root;
	__nostd_u8_t  *buf;
	__nostd_usz_t  len;
} MFreelist;

__nostd_api void
mfreelist_init(MFreelist     *fl,
               void          *buf,
               __nostd_usz_t  len,
               int            align);

__nostd_api void *
mfreelist_alloc(MFreelist     *fl,
                __nostd_usz_t  len,
                __nostd_usz_t  align);

__nostd_api void
mfreelist_free(MFreelist *fl,
               void * buf);

__nostd_api void
mfreelist_reset(MFreelist *fl);

__nostd_api __nostd_usz_t
mfreelist_get_ncolor(MFLNode *node);

__nostd_api MFLNode *
mfreelist_get_parent(MFLNode *node);

__nostd_api void
mfreelist_set_ncolor(MFLNode *node,
                     MFreelistColor color);

__nostd_api void
mfreelist_set_parent(MFLNode *node,
                     MFLNode *parent);

__nostd_api void
mfreelist_set_footer_block(MFLNode *block);

__nostd_api MFLNode *
mfreelist_get_next_block(MFLNode *block);

__nostd_api MFLNode *
mfreelist_get_prev_block(MFLNode *block);

__nostd_api void
mfreelist_rotate_left(MFreelist *fl,
                      MFLNode *pivot);


__nostd_api void
mfreelist_rotate_right(MFreelist *fl,
                       MFLNode *pivot);

__nostd_api void
mfreelist_insert(MFreelist *fl,
                 MFLNode   *node);

__nostd_api MFLNode *
mfreelist_node_min(MFLNode *node);

__nostd_api void
mfreelist_transplant(MFreelist *fl,
                     MFLNode   *node_replace,
                     MFLNode   *node);

__nostd_api void
mfreelist_remove(MFreelist *fl,
                 MFLNode *node);

//////////////////////////
// Function Definitions //
//////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MFREELIST__)

__nostd_api void
mfreelist_init(MFreelist     *fl,
               void          *buf,
               __nostd_usz_t  len,
               int            align)
{
	__nostd_usz_t pad;

	__nostd_assert(__nostd_is_pow2(align) > 0,
	               "Arg3 (align) Variable is NOT power of 2!");
	pad = __nostd_align_pad(buf, align);

	fl->len  = len - pad;
	fl->buf  = (__nostd_u8_t *)buf + pad;
	mfreelist_reset(fl);
}

__nostd_api void *
mfreelist_alloc(MFreelist     *fl,
                __nostd_usz_t  len,
                __nostd_usz_t  align)
{
	MFLNode *best;
	MFLNode *cur;
	MFLNode *new_mem;
	void    *buf;
	__nostd_usz_t remaining;

	__nostd_assert(__nostd_is_pow2(align) > 0,
	               "Arg2 (align) Variable is NOT power of 2!");
	if (!len) return 0;

	len += __nostd_align_pad(len, align);

	best = 0;
	cur  = fl->root;
	while (cur) {
		if (cur->len == len) {
			best = cur;
			break;
		}

		if (cur->len > len) {
			best = cur;
			cur = cur->left;
		}
		else
			cur = cur->right;
	}

	if (!best) return 0;
	mfreelist_remove(fl, best);

	new_mem = 0;
	remaining = best->len - len;
	if (remaining > __mfreelist_block_size(sizeof(void*))) {
		new_mem        = (MFLNode*)((char*)best +
		                                   __mfreelist_block_size(len));

		new_mem->len   = remaining -
		                 __mfreelist_header_size -
		                 __mfreelist_footer_size;

		new_mem->left  = 0;
		new_mem->right = 0;

		mfreelist_set_parent(new_mem, 0);
		mfreelist_set_ncolor(new_mem, MFREELIST_RED);
		mfreelist_set_footer_block(new_mem);

		mfreelist_insert(fl, new_mem);
		best->len = len;
	}

	buf = (void*)((char*)best + __mfreelist_header_size);
	__nostd_memzero(buf, best->len);
	return buf;
}

__nostd_api void
mfreelist_free(MFreelist *fl,
               void * buf)
{
	MFLNode       *block;
	MFLNode       *prev;
	MFLNode       *next;
	__nostd_usz_t  size;
	int            merged;

	if (!buf) return;
	block = (MFLNode *)((char*)buf - __mfreelist_header_size);
	size  = block->len;

	prev   = 0;
	next   = 0;
	merged = 0;

	if (((char*)block) > ((char*)fl->buf + __mfreelist_header_size)) {
		prev = mfreelist_get_prev_block(block);
		if (prev->parent || fl->root == prev) {
			mfreelist_remove(fl, prev);
			prev->len += __mfreelist_block_size(size);
			mfreelist_set_footer_block(prev);
			block = prev;
			merged = 1;
		}
	}

	next = mfreelist_get_next_block(block);
	if (((char*)block) < ((char*)fl->buf + fl->len)) {
		if (next->parent || fl->root == next) {
			mfreelist_remove(fl, next);
			block->len += __mfreelist_block_size(next->len);
			merged = 1;
		}
	}

	if (!merged) {
		block->left  = 0;
		block->right = 0;
		mfreelist_set_parent(block, 0);
		mfreelist_set_ncolor(block, MFREELIST_RED);
	}

	mfreelist_set_footer_block(block);
	mfreelist_insert(fl, block);
}

__nostd_api void
mfreelist_reset(MFreelist *fl)
{
	fl->root = (MFLNode *)fl->buf;

	fl->root->len   = fl->len - __mfreelist_block_size(0); 
	fl->root->left  = 0;
	fl->root->right = 0;
	mfreelist_set_parent(fl->root, 0);
	mfreelist_set_ncolor(fl->root, MFREELIST_BLACK);
}

__nostd_api __nostd_usz_t
mfreelist_get_ncolor(MFLNode *n)
{
	return (__nostd_usz_t)((__nostd_usz_t)n->parent & 1);
}

__nostd_api MFLNode *
mfreelist_get_parent(MFLNode *n)
{
	return (MFLNode *)((__nostd_usz_t)n->parent & ~((__nostd_usz_t)(1)));
}

__nostd_api void
mfreelist_set_ncolor(MFLNode *n,
                     MFreelistColor c)
{
	n->parent = (MFLNode *)(((__nostd_usz_t)mfreelist_get_parent(n)) | (c & 1));
}

__nostd_api void
mfreelist_set_parent(MFLNode *n,
                     MFLNode *p)
{
	n->parent = (MFLNode *)(((__nostd_usz_t)p) | mfreelist_get_ncolor(n));
}

__nostd_api void
mfreelist_rotate_left(MFreelist *fl,
                      MFLNode *pivot)
{
	MFLNode *child_r;
	MFLNode *pivot_p;

	child_r = pivot->right;
	pivot->right = child_r->left;
	if (child_r->left)
		mfreelist_set_parent(child_r->left, pivot);

	pivot_p = mfreelist_get_parent(pivot);
	mfreelist_set_parent(child_r, pivot_p);
	if (!pivot_p)
		fl->root = child_r;
	else if (pivot == pivot_p->left)
		pivot_p->left = child_r;
	else
		pivot_p->right = child_r;

	child_r->left = pivot;
	mfreelist_set_parent(pivot, child_r);
}

__nostd_api void
mfreelist_rotate_right(MFreelist *fl,
                       MFLNode *pivot)
{
	MFLNode *child_l;
	MFLNode *pivot_p;

	child_l = pivot->left;
	pivot->left = child_l->right;
	if (child_l->right)
		mfreelist_set_parent(child_l->right, pivot);

	pivot_p = mfreelist_get_parent(pivot);
	mfreelist_set_parent(child_l, pivot_p);
	if (!pivot_p)
		fl->root = child_l;
	else if (pivot == pivot_p->right)
		pivot_p->right = child_l;
	else
		pivot_p->left = child_l;

	child_l->right = pivot;
	mfreelist_set_parent(pivot, child_l);
}

__nostd_api void
mfreelist_insert(MFreelist *fl,
                 MFLNode   *node)
{
	MFLNode *cur;
	MFLNode *parent;
	MFLNode *gparent;

	parent = 0;
	cur  = fl->root;
	while(cur) {
		parent = cur;
		cur = (node->len < cur->len) ? cur->left : cur->right;
	}
	mfreelist_set_parent(node, parent);

	if (!parent)
		fl->root = node;
	else if (node->len < parent->len)
		parent->left = node;
	else
		parent->right = node;

	node->left  = 0;
	node->right = 0;
	mfreelist_set_ncolor(node, MFREELIST_RED);

	while (node != fl->root &&
	       mfreelist_get_ncolor(mfreelist_get_parent(node)) == MFREELIST_RED)
	{
		parent  = mfreelist_get_parent(node);
		gparent = mfreelist_get_parent(parent);

		if (parent == gparent->left) {
			cur = gparent->right;
			if (cur && mfreelist_get_ncolor(cur) == MFREELIST_RED) {
				mfreelist_set_ncolor(parent, MFREELIST_BLACK);
				mfreelist_set_ncolor(cur, MFREELIST_BLACK);
				mfreelist_set_ncolor(gparent, MFREELIST_RED);

				node = gparent;
			}
			else {
				if (node == parent->right) {
					node = parent;
					mfreelist_rotate_left(fl, node);
				}

				mfreelist_set_ncolor(mfreelist_get_parent(node), MFREELIST_BLACK);
				mfreelist_set_ncolor(mfreelist_get_parent(mfreelist_get_parent(node)),
				                     MFREELIST_RED);
				mfreelist_rotate_right(fl,
				                       mfreelist_get_parent(mfreelist_get_parent(node)));
			}
		}
		else {
			cur = gparent->left;
			if (cur && mfreelist_get_ncolor(cur) == MFREELIST_RED) {
				mfreelist_set_ncolor(parent, MFREELIST_BLACK);
				mfreelist_set_ncolor(cur, MFREELIST_BLACK);
				mfreelist_set_ncolor(gparent, MFREELIST_RED);

				node = gparent;
			}
			else {
				if (node == parent->left) {
					node = parent;
					mfreelist_rotate_right(fl, node);
				}

				mfreelist_set_ncolor(mfreelist_get_parent(node), MFREELIST_BLACK);
				mfreelist_set_ncolor(mfreelist_get_parent(mfreelist_get_parent(node)),
				                     MFREELIST_RED);
				mfreelist_rotate_left(fl,
				                      mfreelist_get_parent(mfreelist_get_parent(node)));
			}
		}
	}

	mfreelist_set_ncolor(fl->root, MFREELIST_BLACK);
}

__nostd_api MFLNode *
mfreelist_node_min(MFLNode *node) {
	while (node->left) {
		node = node->left;
	}

	return node;
}

__nostd_api void
mfreelist_transplant(MFreelist *fl,
                     MFLNode   *node_replace,
                     MFLNode   *node)
{
	MFLNode *node_rep_p;

	node_rep_p = mfreelist_get_parent(node_replace);
	if (!node_rep_p)
		fl->root = node;
	else if (node_replace == node_rep_p->left)
		node_rep_p->left = node;
	else
		node_rep_p->right = node;

	if (node)
		mfreelist_set_parent(node, node_rep_p);
}

__nostd_api void
mfreelist_remove(MFreelist *fl,
                 MFLNode *node)
{
	MFLNode       *replacement;
	MFLNode       *fixup_node;
	MFLNode       *fixup_p;
	MFLNode       *sibling;
	__nostd_usz_t  orig_color;

	replacement = node;
	orig_color = mfreelist_get_ncolor(replacement);
	fixup_node = 0;

	if (!node->left) {
		fixup_node = node->right;
		mfreelist_transplant(fl, node, node->right);
	}
	else if (!node->right) {
		fixup_node = node->left;
		mfreelist_transplant(fl, node, node->left);
	}
	else {
		replacement = mfreelist_node_min(node->right);
		orig_color = mfreelist_get_ncolor(replacement);
		fixup_node = replacement->right;

		if (mfreelist_get_parent(replacement) == node) {
			if (fixup_node)
				mfreelist_set_parent(fixup_node, replacement);
		}
		else {
			mfreelist_transplant(fl, replacement, replacement->right);
			replacement->right = node->right;
			mfreelist_set_parent(replacement->right, replacement);
		}

		mfreelist_transplant(fl, node, replacement);
		replacement->left = node->left;
		mfreelist_set_parent(replacement->left, replacement);
		mfreelist_set_ncolor(replacement, mfreelist_get_ncolor(node));
	}

	if (orig_color == MFREELIST_BLACK && fixup_node) {
		while (fixup_node != fl->root && mfreelist_get_ncolor(fixup_node) == MFREELIST_BLACK) {
			fixup_p = mfreelist_get_parent(fixup_node);
			if (fixup_node == fixup_p->left) {
				sibling = fixup_p->right;

				if (mfreelist_get_ncolor(sibling) == MFREELIST_RED) {
					mfreelist_set_ncolor(sibling, MFREELIST_BLACK);
					mfreelist_set_ncolor(fixup_p, MFREELIST_RED);
					mfreelist_rotate_left(fl, fixup_p);
					sibling = fixup_p->right;
				}

				if ((!sibling->left  || mfreelist_get_ncolor(sibling->left)  == MFREELIST_BLACK) &&
					(!sibling->right || mfreelist_get_ncolor(sibling->right) == MFREELIST_BLACK))
				{
					mfreelist_set_ncolor(sibling, MFREELIST_RED);
					fixup_node = fixup_p;
				}
				else {
					if ((!sibling->right) || mfreelist_get_ncolor(sibling->right) == MFREELIST_BLACK) {
						if (sibling->left)
							mfreelist_set_ncolor(sibling->left, MFREELIST_BLACK);

						mfreelist_set_ncolor(sibling, MFREELIST_RED);
						mfreelist_rotate_right(fl, sibling);
						sibling = fixup_p->right;
					}

					mfreelist_set_ncolor(sibling, mfreelist_get_ncolor(fixup_p));
					mfreelist_set_ncolor(fixup_p, MFREELIST_BLACK);
					if (sibling->right)
						mfreelist_set_ncolor(sibling->right, MFREELIST_BLACK);

					mfreelist_rotate_left(fl, fixup_p);
					fixup_node = fl->root;
				}
			}
			else {
				sibling = fixup_p->left;

				if (mfreelist_get_ncolor(sibling) == MFREELIST_RED) {
					mfreelist_set_ncolor(sibling, MFREELIST_BLACK);
					mfreelist_set_ncolor(fixup_p, MFREELIST_RED);
					mfreelist_rotate_right(fl, fixup_p);
					sibling = fixup_p->left;
				}

				if ((!sibling->left  || mfreelist_get_ncolor(sibling->left)  == MFREELIST_BLACK) &&
					(!sibling->right || mfreelist_get_ncolor(sibling->right) == MFREELIST_BLACK))
				{
					mfreelist_set_ncolor(sibling, MFREELIST_RED);
					fixup_node = fixup_p;
				}
				else {
					if ((!sibling->left) || mfreelist_get_ncolor(sibling->left) == MFREELIST_BLACK) {
						if (sibling->right)
							mfreelist_set_ncolor(sibling->right, MFREELIST_BLACK);
						
						mfreelist_set_ncolor(sibling, MFREELIST_RED);
						mfreelist_set_ncolor(fixup_p, MFREELIST_BLACK);
						if (sibling->left)
							mfreelist_set_ncolor(sibling->left, MFREELIST_BLACK);

						mfreelist_rotate_right(fl, fixup_p);
						fixup_node = fl->root;
					}

					mfreelist_set_ncolor(sibling, mfreelist_get_ncolor(fixup_p));
				}
			}
		}

		mfreelist_set_ncolor(fixup_node, MFREELIST_BLACK);
	}
}

__nostd_api void
mfreelist_set_footer_block(MFLNode *block)
{
	__nostd_usz_t *f = (__nostd_usz_t *)((char*)block +
	                                            __mfreelist_header_size +
	                                            block->len);
	*f = block->len;
}

__nostd_api MFLNode *
mfreelist_get_next_block(MFLNode *block)
{
	return (MFLNode *)((char*)block + __mfreelist_block_size(block->len));
}

__nostd_api MFLNode *
mfreelist_get_prev_block(MFLNode *block)
{
	__nostd_usz_t *pf = (__nostd_usz_t *)((char*)block - 
	                                             __mfreelist_footer_size);
	__nostd_usz_t plen = *pf;
	return (MFLNode *)((char *)block - __mfreelist_block_size(plen));
}

#endif // defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_MFREELIST__)

