#ifndef INCLUDE_NOSTD_CORE_DEFINE_H
#define INCLUDE_NOSTD_CORE_DEFINE_H

//////////////////////////////
// Above All: No Reference! //
//////////////////////////////
#define unref(x) (void)sizeof(x)

////////////////////
// Static Contexts//
////////////////////
#define local  static
#define global static
#define sticky static
#define external

/////////////////////////
// Byte Groups & Flags //
/////////////////////////
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((usz)(x) << 30)
#define TB(x) ((usz)(x) << 40)

#define flag_set(x, flag) ((x) |= (flag))
#define flag_rem(x, flag) ((x) &= ~(flag))
#define flag_tog(x, flag) ((x) ^= (flag))
#define flag_has(x, flag) ((x) & (flag))
#define flag_all(x, flag) ((x) & (flag) == (flag))

#define page_size         KB(4)
#define word_size        (sizeof(void *))
#define two_word_size    (sizeof(void *) << 1)
#define sse_word_size    16
#define avx_word_size    32
#define avx512_word_size 64
#define amx_word_size    128

////////////////////////////
// Architecture Detection //
////////////////////////////
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

//////////////
// Bit Math //
//////////////
#define minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
#define imin(x, y) ((y) ^ minmax_bits((x), (y)))
#define imax(x, y) ((x) ^ minmax_bits((x), (y)))
#define iclamp(l, x, h) imin(imax(x, l), h)

#define isinf(x) (((x) & 0x7FF0000000000000) == 0x7FF0000000000000)
#define isnan(x) (((x) & 0x7FFFFFFFFFFFFFFF)  > 0x7FF0000000000000)

#define is_pow2(x) ((x) && !((x) & ((x) - 1)))
#define align_addr(p, a) ((usz)(p) & -(a))
#define align_next(p, a) ((a) + align_addr(p, a))
#define align_over(p, a) ((usz)(p) & ((a) - 1))
#define align_pad(p, a)  (((a) - align_over(p, a)) & ((a) - 1))

#define swap(x, y)  \
		(x) ^= (y); \
		(y) ^= (x); \
		(x) ^= (y)
#define safe_swap(x, y) \
	if ((x) ^ (y)) {    \
		swap(x, y)      \
	}

#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#define ceilto_pow2(x) \
	x--; \
	x |= x >> 1;  \
	x |= x >> 2;  \
	x |= x >> 4;  \
	x |= x >> 8;  \
	x |= x >> 16; \
	x++;
#else
	#define ceilto_pow2(x) \
	x--; \
	x |= x >> 1;  \
	x |= x >> 2;  \
	x |= x >> 4;  \
	x |= x >> 8;  \
	x |= x >> 16; \
	x |= x >> 32; \
	x++;
#endif


/////////////////////////////////////////
// Function Types & Function Variables //
/////////////////////////////////////////
#define declfn(rtype, name, rcode, ...) \
	typedef rtype (*PFN_##name)(__VA_ARGS__); \
	local rtype stub_##name(__VA_ARGS__) {rcode} \
	PFN_##name name = stub_##name

#define declfn_type(rtype, name, ...) \
	typedef rtype (*PFN_##name)(__VA_ARGS__);

#define deffn_wgl(name) \
	name = (PFN_##name)wglGetProcAddress(#name)

#define deffn_dlsym(solib, name) \
	name = (PFN_##name)dlsym(solib, #name)

#define deffn_dll(dll, name) \
	name = (PFN_##name)GetProcAddress(dll, #name)

/////////////////////////////////////
// Counted Strings, Logs & Asserts //
/////////////////////////////////////
#define LOG_LEVEL_GOOFY    0x01
#define LOG_LEVEL_DEBUG    0x02
#define LOG_LEVEL_INFO     0x04
#define LOG_LEVEL_SUCCESS  0x08
#define LOG_LEVEL_ANOMALLY 0x10
#define LOG_LEVEL_WARNING  0x20
#define LOG_LEVEL_ERROR    0x40
#define LOG_LEVEL_FATAL    0x80
#define LOG_LEVEL_CRASH    0x4000
#define LOG_LEVEL_ASSERT   0x8000

#define LOG_FORMAT_FILE_7Z 0x10000

#define log(level, msg) logs8(level, msg, __FILE__, __LINE__, __func__)
#define log_egg(msg)    log(LOG_LEVEL_GOOFY, msg)
#define log_debug(msg)  log(LOG_LEVEL_DEBUG, msg)
#define log_info(msg)   log(LOG_LEVEL_INFO, msg)
#define log_pass(msg)   log(LOG_LEVEL_SUCCESS, msg)
#define log_odd(msg)    log(LOG_LEVEL_ANOMALLY, msg)
#define log_warn(msg)   log(LOG_LEVEL_WARNING, msg)
#define log_error(msg)  log(LOG_LEVEL_ERROR, msg)
#define log_fatal(msg)  log(LOG_LEVEL_FATAL, msg)
#define log_assert(msg) log(LOG_LEVEL_FATAL | LOG_LEVEL_ASSERT, msg)

#define logc(level, msg) logsz(level, msg, __FILE__, __LINE__, __func__)
#define logc_egg(msg)    logc(LOG_LEVEL_GOOFY, msg)
#define logc_debug(msg)  logc(LOG_LEVEL_DEBUG, msg)
#define logc_info(msg)   logc(LOG_LEVEL_INFO, msg)
#define logc_pass(msg)   logc(LOG_LEVEL_SUCCESS, msg)
#define logc_odd(msg)    logc(LOG_LEVEL_ANOMALLY, msg)
#define logc_warn(msg)   logc(LOG_LEVEL_WARNING, msg)
#define logc_error(msg)  logc(LOG_LEVEL_ERROR, msg)
#define logc_fatal(msg)  logc(LOG_LEVEL_FATAL, msg)

#if defined(_DEBUG)
	#define logc_assert(msg) logc(LOG_LEVEL_FATAL | LOG_LEVEL_ASSERT, msg)
	#define assert(x, s) if (!(x)) { \
		logc_assert(s);              \
		usz *___killme = 0;          \
		*___killme = 0xDEADA55E;     \
	}
#elif defined(_DEBUG_RELEASE)
	#define logc_assert(msg) logc(LOG_LEVEL_ERROR | LOG_LEVEL_ASSERT, msg)
	#define assert(x, s) if (!(x)) { logc_assert(s); }
#else
	#define logc_assert(msg)  logc(LOG_LEVEL_ERROR, msg)
	#define assert(x, s) ;
#endif

////////////////////////
// Standalone Defines //
////////////////////////
#define __GET_NOSTD_STANDALONE_IMPLEMENTATION__

#define __nostd_api                     local
#define __nostd_extern                  local
#define __nostd_marena_assert(x, msg)   assert(x, msg)
#define __nostd_is_pow2(x)              is_pow2(x)
#define __nostd_align_over(p, a)        align_over(p, a)
#define __nostd_imin(x, y)              imin(x, y)
#define __nostd_memcpy(dst, src, size)  memcpyu(dst, src, size)
#define __nostd_memmove(dst, src, size) memmoveu(dst, src, size)
#define __nostd_memcmp(dst, src, size)  memcmpu(dst, src, size)
#define __nostd_memeq(dst, src, size)   memequ(dst, src, size)
#define __nostd_memzero(dst, size)      memzerou(dst, size)
#define __nostd_memset(dst, val, size)  memsetu(dst, val, size)

//////////////////////////
// Other Define Helpers //
//////////////////////////
#define _def_cat(x, y) x##y
#define  def_cat(x, y) _def_cat(x, y)

#endif // INCLUDE_NOSTD_CORE_DEFINE_H

