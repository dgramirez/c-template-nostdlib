#ifndef INCLUDE_DEFINE_H
#define INCLUDE_DEFINE_H

#define local  static
#define global static
#define sticky static
#define external

#define unref(x) (void)sizeof(x)

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#define flag_set(x, flag) ((x) |= (flag))
#define flag_rem(x, flag) ((x) &= ~(flag))
#define flag_tog(x, flag) ((x) ^= (flag))
#define flag_has(x, flag) ((x) & (flag))
#define flag_all(x, flag) ((x) & (flag) == (flag))

#define minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
#define imin(x, y) ((y) ^ minmax_bits((x), (y)))
#define imax(x, y) ((x) ^ minmax_bits((x), (y)))
#define iclamp(l, x, h) imin(imax(x, l), h)

#define isinf(x) (((x) & 0x7FF0000000000000) == 0x7FF0000000000000)
#define isnan(x) (((x) & 0x7FFFFFFFFFFFFFFF)  > 0x7FF0000000000000)

#define swap(x, y)  \
		(x) ^= (y); \
		(y) ^= (x); \
		(x) ^= (y)
#define safe_swap(x, y) \
	if ((x) ^ (y)) {    \
		swap(x, y)      \
	}

#define cntof(x) (unsigned int)(sizeof(x) / sizeof(*(x))
#define lenof(x) (cntof(x) - 1)

#define is_pow2(x) ((x) && !((x) & ((x) - 1)))
#define align_addr(p, a) ((usz)(p) & -(a))
#define align_next(p, a) ((a) + align_addr(p, a))
#define align_over(p, a) ((usz)(p) & ((a) - 1))
#define align_pad(p, a)  (((a) - align_over(p, a)) & ((a) - 1))

#define page_size KB(4)

#if defined(_DEBUG)
	#define assert(x, s) if (!(x)) *((volatile int*)0) = 0;
#else
	#define assert(x, s) ;
#endif

#define declfn(rtype, name, rcode, ...) \
	typedef rtype (*PFN_##name)(__VA_ARGS__); \
	rtype stub_##name(__VA_ARGS__) {rcode} \
	PFN_##name name = stub_##name

#define deffn_wgl(name) \
	name = (PFN_##name)wglGetProcAddress(#name)

#define deffn_dlsym(solib, name) \
	name = (PFN_##name)dlsym(solib, #name)

#define deffn_dll(dll, name) \
	name = (PFN_##name)GetProcAddress(dll, #name)

#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#define EXE_ARCH 32
#endif
#if defined(__x86_64__) || defined(_M_X64)
	#define EXE_ARCH 64
#endif

#if defined(_WIN32)
	#define export __declspec(dllexport)
#else
	#define export
#endif

#endif // INCLUDE_DEFINE_H

