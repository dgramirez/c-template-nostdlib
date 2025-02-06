#error This shouldn't be loaded! Content here is saved for future. \
references. Code here is potentially broken (though can be saved if you \
look into the normal memfn.h and follow that format for the SSE code.) \
This will be removed when I separate the CPU functions and features into \
its own dynamic library (Architecturally, I want to support both x86_64 \
and aarch64, and make it as smooth as possible.)

#if 0

#include <emmintrin.h>
#include <immintrin.h>

local void
memcpya_sse2(void *dst,
             void *src,
             usz size)
{
	__m128i d128[4];
	__m128i s128[4];
	pfn_si128_load si128_load_src;
	usz size_m128_4;

	size_m128_4 = sizeof(__m128i) << 2;
	if (size < (sizeof(usz) << 2)) {
		memcpy_u8(dst, src, size);
		return;
	}
	else if (size < size_m128_4) {
		memcpya_usz(dst, src, size);
		return;
	}

	si128_load_src = _mm_load_si128;
	if (align_pad(src, sizeof(__m128i)) > 0)
		si128_load_src = _mm_loadu_si128;

	while (size > size_m128_4) {
		d128[0] = _mm_load_si128((__m128i*)(dst));
		d128[1] = _mm_load_si128((__m128i*)(dst) + 1);
		d128[2] = _mm_load_si128((__m128i*)(dst) + 2);
		d128[3] = _mm_load_si128((__m128i*)(dst) + 3);
		s128[0] = si128_load_src((__m128i*)(src));
		s128[1] = si128_load_src((__m128i*)(src) + 1);
		s128[2] = si128_load_src((__m128i*)(src) + 2);
		s128[3] = si128_load_src((__m128i*)(src) + 3);

		_mm_store_si128(&d128[0], s128[0]);
		_mm_store_si128(&d128[1], s128[1]);
		_mm_store_si128(&d128[2], s128[2]);
		_mm_store_si128(&d128[3], s128[3]);

		dst = (__m128i*)dst + 4;
		src = (__m128i*)dst + 4;
		size -= size_m128_4;
	}

	memcpya_usz(dst, src, size);
}

local void
memcpyu_sse2(void *dst,
             void *src,
             usz size)
{
	__m128i d128;
	__m128i s128;
	pfn_si128_load si128_load_src;
	usz pad;

	if (size < sizeof(__m128i)) {
		memcpyu_usz(dst, src, size);
		return;
	}

	pad = align_pad(dst, sizeof(__m128i));
	if (pad) {
		d128 = _mm_loadu_si128((__m128i*)(dst));

		si128_load_src = _mm_load_si128;
		if (align_pad(src, sizeof(__m128i)) > 0)
			si128_load_src = _mm_loadu_si128;

		s128 = si128_load_src((__m128i*)(src));
		_mm_storeu_si128(&d128, s128);


		dst = (char *)dst + pad;
		src = (char *)src + pad;
		size -= pad;
	}

	memcpya_sse2(dst, src, size);
}

local void
memcpya_avx(void *dst,
            void *src,
            usz size)
{
	__m256i d256[4];
	__m256i s256[4];
	pfn_si256_load si256_load_src;
	usz size_m256_4;

	size_m256_4 = sizeof(__m256i) << 2;
	if (size < (sizeof(usz) << 2)) {
		memcpy_u8(dst, src, size);
		return;
	}
	else if (size < (sizeof(__m128i) << 2)) {
		memcpya_usz(dst, src, size);
		return;
	}
	else if (size < size_m256_4) {
		memcpya_sse2(dst, src, size);
		return;
	}

	si256_load_src = _mm256_load_si256;
	if (align_pad(src, sizeof(__m256i)) > 0)
		si256_load_src = _mm256_loadu_si256;

	while (size > size_m256_4) {
		d256[0] = _mm256_load_si256((__m256i*)(dst));
		d256[1] = _mm256_load_si256((__m256i*)(dst) + 1);
		d256[2] = _mm256_load_si256((__m256i*)(dst) + 2);
		d256[3] = _mm256_load_si256((__m256i*)(dst) + 3);
		s256[0] = si256_load_src((__m256i*)(src));
		s256[1] = si256_load_src((__m256i*)(src) + 1);
		s256[2] = si256_load_src((__m256i*)(src) + 2);
		s256[3] = si256_load_src((__m256i*)(src) + 3);

		_mm256_store_si256(&d256[0], s256[0]);
		_mm256_store_si256(&d256[1], s256[1]);
		_mm256_store_si256(&d256[2], s256[2]);
		_mm256_store_si256(&d256[3], s256[3]);

		dst = (__m256i*)dst + 4;
		src = (__m256i*)dst + 4;
		size -= size_m256_4;
	}

	memcpya_sse2(dst, src, size);
}

local void
memcpyu_avx(void *dst,
            void *src,
            usz size)
{
	__m256i d256;
	__m256i s256;
	pfn_si256_load si256_load_src;
	usz  pad;

	if (size < sizeof(__m256i)) {
		memcpyu_sse2(dst, src, size);
		return;
	}

	pad = align_pad(dst, sizeof(__m256i));
	if (pad) {
		d256 = _mm256_loadu_si256((__m256i*)(dst));

		si256_load_src = _mm256_load_si256;
		if (align_pad(src, sizeof(__m256i)) > 0)
			si256_load_src = _mm256_loadu_si256;
	
		s256 = si256_load_src((__m256i*)(src));
		_mm256_storeu_si256(&d256, s256);

		dst = (char *)dst + pad;
		src = (char *)src + pad;
		size -= pad;
	}

	memcpya_sse2(dst, src, size);
}

local int
memcmpa_sse2(void *left,
             void *right,
             usz size)
{
	__m128i l128[4];
	__m128i r128[4];
	pfn_si128_load si128_load_right;
	usz size_m128_4;

	size_m128_4 = sizeof(__m128i) << 2;
	if (size < (sizeof(usz) << 2))
		return memcmp_u8(left, right, size);
	else if (size < size_m128_4)
		return memcmpa_usz(left, right, size);

	si128_load_right = _mm_load_si128;
	if (align_pad(right, sizeof(__m128i)) > 0)
		si128_load_right = _mm_loadu_si128;

	while (size > size_m128_4) {
		l128[0] = _mm_load_si128((__m128i*)(left));
		l128[1] = _mm_load_si128((__m128i*)(left) + 1);
		l128[2] = _mm_load_si128((__m128i*)(left) + 2);
		l128[3] = _mm_load_si128((__m128i*)(left) + 3);
		r128[0] = si128_load_right((__m128i*)(right));
		r128[1] = si128_load_right((__m128i*)(right) + 1);
		r128[2] = si128_load_right((__m128i*)(right) + 2);
		r128[3] = si128_load_right((__m128i*)(right) + 3);

		_mm_store_si128(&l128[0], r128[0]);
		_mm_store_si128(&l128[1], r128[1]);
		_mm_store_si128(&l128[2], r128[2]);
		_mm_store_si128(&l128[3], r128[3]);

		left = (__m128i*)left + 4;
		right = (__m128i*)left + 4;
		size -= size_m128_4;
	}

	return memcmpa_usz(left, right, size);
}

local int
memcmpu_sse2(void *left,
             void *right,
             usz size)
{
	__m128i d128;
	__m128i s128;
	pfn_si128_load si128_load_right;
	usz pad;

	if (size < sizeof(__m128i)) {
		memcmpu_usz(left, right, size);
		return;
	}

	pad = align_pad(left, sizeof(__m128i));
	if (pad) {
		d128 = _mm_loadu_si128((__m128i*)(left));

		si128_load_right = _mm_load_si128;
		if (align_pad(right, sizeof(__m128i)) > 0)
			si128_load_right = _mm_loadu_si128;

		s128 = si128_load_right((__m128i*)(right));
		_mm_storeu_si128(&d128, s128);


		left = (char *)left + pad;
		right = (char *)right + pad;
		size -= pad;
	}

	memcmpa_sse2(left, right, size);
}

local int
memcmpa_avx(void *left,
            void *right,
            usz size)
{
	__m256i d256[4];
	__m256i s256[4];
	pfn_si256_load si256_load_right;
	usz size_m256_4;

	size_m256_4 = sizeof(__m256i) << 2;
	if (size < (sizeof(usz) << 2)) {
		memcmp_u8(left, right, size);
		return;
	}
	else if (size < (sizeof(__m128i) << 2)) {
		memcmpa_usz(left, right, size);
		return;
	}
	else if (size < size_m256_4) {
		memcmpa_sse2(left, right, size);
		return;
	}

	si256_load_right = _mm256_load_si256;
	if (align_pad(right, sizeof(__m256i)) > 0)
		si256_load_right = _mm256_loadu_si256;

	while (size > size_m256_4) {
		d256[0] = _mm256_load_si256((__m256i*)(left));
		d256[1] = _mm256_load_si256((__m256i*)(left) + 1);
		d256[2] = _mm256_load_si256((__m256i*)(left) + 2);
		d256[3] = _mm256_load_si256((__m256i*)(left) + 3);
		s256[0] = si256_load_right((__m256i*)(right));
		s256[1] = si256_load_right((__m256i*)(right) + 1);
		s256[2] = si256_load_right((__m256i*)(right) + 2);
		s256[3] = si256_load_right((__m256i*)(right) + 3);

		_mm256_store_si256(&d256[0], s256[0]);
		_mm256_store_si256(&d256[1], s256[1]);
		_mm256_store_si256(&d256[2], s256[2]);
		_mm256_store_si256(&d256[3], s256[3]);

		left = (__m256i*)left + 4;
		right = (__m256i*)left + 4;
		size -= size_m256_4;
	}

	memcmpa_sse2(left, right, size);
}

local int
memcmpu_avx(void *left,
            void *right,
            usz size)
{
	__m256i d256;
	__m256i s256;
	pfn_si256_load si256_load_right;
	usz  pad;

	if (size < sizeof(__m256i)) {
		memcmpu_sse2(left, right, size);
		return;
	}

	pad = align_pad(left, sizeof(__m256i));
	if (pad) {
		d256 = _mm256_loadu_si256((__m256i*)(left));

		si256_load_right = _mm256_load_si256;
		if (align_pad(right, sizeof(__m256i)) > 0)
			si256_load_right = _mm256_loadu_si256;
	
		s256 = si256_load_right((__m256i*)(right));
		_mm256_storeu_si256(&d256, s256);

		left = (char *)left + pad;
		right = (char *)right + pad;
		size -= pad;
	}

	memcmpa_sse2(left, right, size);
}

local void
memzeroa_sse2(void *dst,
              usz size)
{
	__m128i d128[4];
	__m128i zero;
	usz size_m128_4;

	size_m128_4 = sizeof(__m128i) << 2;
	if (size < (sizeof(usz) << 2)) {
		memzero_u8(dst, size);
		return;
	}
	else if (size < size_m128_4) {
		memzeroa_usz(dst, size);
		return;
	}

	zero = _mm_setzero_si128();
	while (size > size_m128_4) {
		d128[0] = _mm_load_si128((__m128i*)(dst));
		d128[1] = _mm_load_si128((__m128i*)(dst) + 1);
		d128[2] = _mm_load_si128((__m128i*)(dst) + 2);
		d128[3] = _mm_load_si128((__m128i*)(dst) + 3);

		_mm_store_si128(&d128[0], zero);
		_mm_store_si128(&d128[1], zero);
		_mm_store_si128(&d128[2], zero);
		_mm_store_si128(&d128[3], zero);

		dst = (__m128i*)dst + 4;
		size -= size_m128_4;
	}

	memzeroa_usz(dst, size);
}

local void
memzerou_sse2(void *dst,
              usz size)
{
	__m128i d128;
	__m128i zero;
	usz pad;

	if (size < sizeof(__m128i)) {
		memzerou_usz(dst, size);
		return;
	}

	pad = align_pad(dst, sizeof(__m128i));
	if (pad) {
		d128 = _mm_loadu_si128((__m128i*)(dst));
		zero = _mm_setzero_si128();
		_mm_storeu_si128(&d128, zero);

		dst = (char *)dst + pad;
		size -= pad;
	}

	memzeroa_sse2(dst, size);
}

local void
memzeroa_avx(void *dst,
             usz size)
{
	__m256i d256[4];
	__m256i zero;
	usz size_m256_4;

	size_m256_4 = sizeof(__m256i) << 2;
	if (size < (sizeof(usz) << 2)) {
		memzero_u8(dst, size);
		return;
	}
	else if (size < (sizeof(__m128i) << 2)) {
		memzeroa_usz(dst, size);
		return;
	}
	else if (size < size_m256_4) {
		memzeroa_sse2(dst, size);
		return;
	}

	zero = _mm256_setzero_si256();
	while (size > size_m256_4) {
		d256[0] = _mm256_load_si256((__m256i*)(dst));
		d256[1] = _mm256_load_si256((__m256i*)(dst) + 1);
		d256[2] = _mm256_load_si256((__m256i*)(dst) + 2);
		d256[3] = _mm256_load_si256((__m256i*)(dst) + 3);

		_mm256_store_si256(&d256[0], zero);
		_mm256_store_si256(&d256[1], zero);
		_mm256_store_si256(&d256[2], zero);
		_mm256_store_si256(&d256[3], zero);

		dst = (__m256i*)dst + 4;
		size -= size_m256_4;
	}

	memzeroa_sse2(dst, size);
}

local void
memzerou_avx(void *dst,
             usz size)
{
	__m256i d256;
	__m256i zero;
	usz  pad;

	if (size < sizeof(__m256i)) {
		memzerou_sse2(dst, size);
		return;
	}

	pad = align_pad(dst, sizeof(__m256i));
	if (pad) {
		d256 = _mm256_loadu_si256((__m256i*)(dst));
		zero = _mm256_setzero_si256();
		_mm256_storeu_si256(&d256, zero);

		dst = (char *)dst + pad;
		size -= pad;
	}

	memzeroa_sse2(dst, size);
}

#endif // 0
