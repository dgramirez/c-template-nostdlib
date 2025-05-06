#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_HELPER_FN_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_HELPER_FN_H

local void
main_setup_memory(b8 mem,
                  MBuddy    *mbuddy)
{
	b8 b = {0};

	usz mlen = mem.len >> 1;
	ceilto_pow2(mlen);
	mlen = mbuddy_get_bitmap_len(mlen);
	mbuddy_init(mbuddy, mem.data + mlen, mem.len - mlen, mem.data);

	b.len  = MB(4);
	b.data = mbuddy_alloc(mbuddy, b.len);
	mfreelist_init(&_sysfl, b.data, b.len, 8);
}



#endif // INCLUDE_NOSTD_PLATFORM_COMMON_HELPER_FN_H

