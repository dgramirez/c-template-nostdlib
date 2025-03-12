#ifndef INCLUDE_MEM_ARENA_H
#define INCLUDE_MEM_ARENA_H

#include "define.h"
#include "platform.h"
#include "memfn.h"

typedef struct {
	u8 *start;
	u8 *end;
	u8 *current;
	u8 *previous;
} MArena;

typedef struct {
	MArena *arena;
	u8 *current;
	u8 *previous;
} MArenaTemp;

local void
marena_init(MArena *out,
            void *buf,
            usz len,
            int align)
{
	usz pad;

	if (!is_pow2(align))
		assert(0, "Align Variable is NOT power of 2!");
	pad = align_pad(buf, align);

	out->start = (u8 *)buf + pad;
	out->end = out->start + len - pad;
	out->current = out->start;
	out->previous = 0;
}

local void *
marena_alloc(MArena *arena,
             usz len,
			 usz align)
{
	usz pad;

	if (!is_pow2(align))
		assert(0, "Align Variable is NOT power of 2!");

	pad = align_pad(arena->current, align);
	if ((arena->current + pad + len) > arena->end)
		return 0;

	arena->previous = arena->current + pad;
	arena->current  = arena->current + pad + len;

	memzerou(arena->previous, len);
	return arena->previous;
}

local void *
marena_realloc(MArena *arena,
               void *oldbuf,
               usz len,
			   usz align)
{
	usz oldlen;
	usz pad;

	if (oldbuf && (oldbuf == arena->previous)) {
		pad = align_pad(oldbuf, align);
		if ((arena->previous + pad + len) > arena->end)
			return 0;

		if (!is_pow2(align))
			assert(0, "Align Variable is NOT power of 2!");

		oldlen = arena->current - arena->previous;
		if (pad) {
			memmoveu(arena->previous + pad,
			         arena->previous,
			         imin(oldlen, len));

			arena->previous += pad;
			if ((oldlen + pad) > len)
				memzerou(arena->current + pad, len - (oldlen + pad));

			arena->current = arena->previous + len;
			return oldbuf;
		}

		if (len > oldlen)
			memzerou(arena->current, len - oldlen);

		arena->current = arena->previous + len;
		return oldbuf;
	}

	return marena_alloc(arena, len, align);
}

local void
marena_reset(MArena *arena)
{
	arena->current = arena->start;
	arena->previous = 0;
}

local void
marena_save(MArenaTemp *out,
            MArena *arena)
{
	out->arena    = arena;
	out->current  = arena->current;
	out->previous = arena->previous;
}

local void
marena_load(MArenaTemp *temp)
{
	temp->arena->current  = temp->current;
	temp->arena->previous = temp->previous;
}

#endif // INCLUDE_MEM_ARENA_H

