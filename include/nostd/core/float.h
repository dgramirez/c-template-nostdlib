/**************************************************************************
 * Name:   Float Functions (floats.h)
 * Author: Derrick G. Ramirez
 *
 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION__
 *     - Use this define above for all nostd standalone files.
 *
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION_FLOATS__
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

 /* A few future TODOs */
 /*
	 - Couple of functions:
		 - exp
		 - pow
		 - log, log2, log10
 */

 ////////////////////////
 // Standalone Defines //
 ////////////////////////
#ifndef __STANDALONE_FLOAT_FUNCTIONS_H
#define __STANDALONE_FLOAT_FUNCTIONS_H

#ifndef __nostd_api
	#define __nostd_api
#endif

#ifndef __nostd_iminf
	#ifndef __nostd_minmax_bits
		#define __nostd_minmaxf_bits(ix, iy, fx, fy) \
			(((ix) ^ (iy)) & -((fx) < (fy)))
	#endif
	#define __nostd_iminf(ix, iy, fx, fy) \
		((iy) ^ __nostd_minmaxf_bits((ix), (iy), (fx), (fy)))
#endif

#ifndef __nostd_extern 
	#define __nostd_extern extern
#endif

#ifndef __nostd_f32iunion
	typedef union {
		float        f;
		unsigned int u;
		int          i;
	} F32i;
	#define __nostd_f32iunion
#endif

#ifndef __nostd_f64iunion
	typedef union {
		double d;
		struct { unsigned int ulow, uhigh; };
		struct {          int ilow, ihigh; };
		#if defined(__x86_64__) || defined(_M_X64)
			unsigned long long ull;
			long long           ll;
		#endif
	} F64i;
	#define __nostd_f64iunion
#endif


// F32 Defines
#ifndef F32_DIGITS
#define F32_DIGITS           6
#endif

#ifndef F32_EPSILON
#define F32_EPSILON          0.000001f
#endif

#ifndef F32_EPSILON_MACHINE
#define F32_EPSILON_MACHINE  1.19209e-07f
#endif 

#ifndef F32_MIN
#define F32_MIN              1.1754943508e+38f
#endif

#ifndef F32_MIN_BITS
#define F32_MIN_BITS         0x00800000
#endif

#ifndef F32_MAX
#define F32_MAX              3.4028234664e+38f
#endif

#ifndef F32_MAX_BITS
#define F32_MAX_BITS         0x7F7FFFFF
#endif

#ifndef F32_2PI
#define F32_2PI              6.28318530717958647692528676655900f
#endif

#ifndef F32_PI
#define F32_PI               3.14159265358979323846264338327950f
#endif

#ifndef F32_PI2
#define F32_PI2              1.57079632679489661923132169163975f
#endif

#ifndef F32_PI4
#define F32_PI4              0.78539816339744830961566084581988f
#endif

#ifndef F32_SQRT2_2
#define F32_SQRT2_2          0.70710678118654752440084436210485f
#endif

// F64
#ifndef F64_DIGITS
#define F64_DIGITS           15
#endif

#ifndef F64_EPSILON
#define F64_EPSILON          0.000000000000001f
#endif

#ifndef F64_EPSILON_MACHINE
#define F64_EPSILON_MACHINE  2.22045e-16f
#endif 

#ifndef F64_MIN
#define F64_MIN              2.2250738585072014e10-308
#endif

#ifndef F64_MIN_BITS
#define F64_MIN_BITS         0x0010000000000000
#endif

#ifndef F64_MAX
#define F64_MAX              1.7976931348623157e+308
#endif

#ifndef F64_MAX_BITS
#define F64_MAX_BITS         0x7FEFFFFFFFFFFFFF
#endif

#ifndef F64_2PI
#define F64_2PI              6.28318530717958647692528676655900
#endif

#ifndef F64_PI
#define F64_PI               3.14159265358979323846264338327950
#endif

#ifndef F64_PI2
#define F64_PI2              1.57079632679489661923132169163975
#endif

#ifndef F64_PI4
#define F64_PI4              0.78539816339744830961566084581988
#endif

#ifndef F64_SQRT2_2
#define F64_SQRT2_2          0.70710678118654752440084436210485
#endif

#ifndef F32_DEG_TO_RAD
#define F32_DEG_TO_RAD(deg)  0.01745329251994329576923690768489f * deg
#endif

#ifndef F32_RAD_TO_DEG
#define F32_RAD_TO_DEG(rad)  57.2957795130823208767981548141051f * rad
#endif

/////////////////
//  Prototypes //
/////////////////
typedef double (*f64_trigfn)(double x);
__nostd_extern f64_trigfn f64_sin;
__nostd_extern f64_trigfn f64_cos;
__nostd_extern f64_trigfn f64_tan;
__nostd_extern f64_trigfn f64_csc;
__nostd_extern f64_trigfn f64_sec;
__nostd_extern f64_trigfn f64_cot;
__nostd_extern f64_trigfn f64_asin;
__nostd_extern f64_trigfn f64_acos;
__nostd_extern f64_trigfn f64_atan;

typedef float (*f32_trigfn)(float x);
__nostd_extern  f32_trigfn f32_sin;
__nostd_extern  f32_trigfn f32_cos;
__nostd_extern  f32_trigfn f32_tan;
__nostd_extern  f32_trigfn f32_csc;
__nostd_extern  f32_trigfn f32_sec;
__nostd_extern  f32_trigfn f32_cot;
__nostd_extern  f32_trigfn f32_asin;
__nostd_extern  f32_trigfn f32_acos;
__nostd_extern  f32_trigfn f32_atan;

// F64
__nostd_api int
f64_iszero(double x);

__nostd_api int
f64_iseq(double x,
	double y);

__nostd_api double
f64_ceil(double x);

__nostd_api double
f64_floor(double x);

__nostd_api double
f64_abs(double x);

__nostd_api double
f64_mod(double x, double mod);

/* Unimplemented
__nostd_api double
f64_pow(double x, double e);

__nostd_api double
f64_exp(double x);

__nostd_api double
f64_log(double x);

__nostd_api double
f64_log2(double x);

__nostd_api double
f64_log10(double x);
*/

__nostd_api double
f64_sqrt(double x);

__nostd_api double
f64_isqrt(double x);

// F32
__nostd_api int
f32_iszero(float x);

__nostd_api int
f32_iseq(float x,
	float y);

__nostd_api float
f32_ceil(float x);

__nostd_api float
f32_floor(float x);

__nostd_api float
f32_abs(float x);

__nostd_api float
f32_mod(float x, float mod);

/* Unimplemented
__nostd_api float
f32_pow(float x, float e);

__nostd_api float
f32_exp(float x);

__nostd_api float
f32_log(float x);

__nostd_api float
f32_log2(float x);

__nostd_api float
f32_log10(float x);
*/

__nostd_api float
f32_sqrt(float x);

__nostd_api float
f32_isqrt(float x);

// Trig Functions (F64, F32)
__nostd_api double
f64_sin_mm2(double x);
__nostd_api double
f64_sin_mm3(double x);
__nostd_api double
f64_sin_mm4(double x);
__nostd_api double
f64_sin_mm5(double x);
__nostd_api double
f64_sin_mm6(double x);
__nostd_api double
f64_sin_mm7(double x);

__nostd_api float
f32_sin_mm2(float x);
__nostd_api float
f32_sin_mm3(float x);
__nostd_api float
f32_sin_mm4(float x);
__nostd_api float
f32_sin_mm5(float x);
__nostd_api float
f32_sin_mm6(float x);
__nostd_api float
f32_sin_mm7(float x);

__nostd_api double
f64_cos_mm2(double x);
__nostd_api double
f64_cos_mm3(double x);
__nostd_api double
f64_cos_mm4(double x);
__nostd_api double
f64_cos_mm5(double x);
__nostd_api double
f64_cos_mm6(double x);
__nostd_api double
f64_cos_mm7(double x);

__nostd_api float
f32_cos_mm2(float x);
__nostd_api float
f32_cos_mm3(float x);
__nostd_api float
f32_cos_mm4(float x);
__nostd_api float
f32_cos_mm5(float x);
__nostd_api float
f32_cos_mm6(float x);
__nostd_api float
f32_cos_mm7(float x);

__nostd_api double
f64_tan_mm2(double x);
__nostd_api double
f64_tan_mm3(double x);
__nostd_api double
f64_tan_mm4(double x);
__nostd_api double
f64_tan_mm5(double x);
__nostd_api double
f64_tan_mm6(double x);
__nostd_api double
f64_tan_mm7(double x);

__nostd_api float
f32_tan_mm2(float x);
__nostd_api float
f32_tan_mm3(float x);
__nostd_api float
f32_tan_mm4(float x);
__nostd_api float
f32_tan_mm5(float x);
__nostd_api float
f32_tan_mm6(float x);
__nostd_api float
f32_tan_mm7(float x);

__nostd_api double
f64_csc_mm2(double x);
__nostd_api double
f64_csc_mm3(double x);
__nostd_api double
f64_csc_mm4(double x);
__nostd_api double
f64_csc_mm5(double x);
__nostd_api double
f64_csc_mm6(double x);
__nostd_api double
f64_csc_mm7(double x);

__nostd_api float
f32_csc_mm2(float x);
__nostd_api float
f32_csc_mm3(float x);
__nostd_api float
f32_csc_mm4(float x);
__nostd_api float
f32_csc_mm5(float x);
__nostd_api float
f32_csc_mm6(float x);
__nostd_api float
f32_csc_mm7(float x);

__nostd_api double
f64_sec_mm2(double x);
__nostd_api double
f64_sec_mm3(double x);
__nostd_api double
f64_sec_mm4(double x);
__nostd_api double
f64_sec_mm5(double x);
__nostd_api double
f64_sec_mm6(double x);
__nostd_api double
f64_sec_mm7(double x);

__nostd_api float
f32_sec_mm2(float x);
__nostd_api float
f32_sec_mm3(float x);
__nostd_api float
f32_sec_mm4(float x);
__nostd_api float
f32_sec_mm5(float x);
__nostd_api float
f32_sec_mm6(float x);
__nostd_api float
f32_sec_mm7(float x);

__nostd_api double
f64_cot_mm2(double x);
__nostd_api double
f64_cot_mm3(double x);
__nostd_api double
f64_cot_mm4(double x);
__nostd_api double
f64_cot_mm5(double x);
__nostd_api double
f64_cot_mm6(double x);
__nostd_api double
f64_cot_mm7(double x);

__nostd_api float
f32_cot_mm2(float x);
__nostd_api float
f32_cot_mm3(float x);
__nostd_api float
f32_cot_mm4(float x);
__nostd_api float
f32_cot_mm5(float x);
__nostd_api float
f32_cot_mm6(float x);
__nostd_api float
f32_cot_mm7(float x);

__nostd_api double
f64_asin_mm2(double x);
__nostd_api double
f64_asin_mm3(double x);
__nostd_api double
f64_asin_mm4(double x);
__nostd_api double
f64_asin_mm5(double x);
__nostd_api double
f64_asin_mm6(double x);
__nostd_api double
f64_asin_mm7(double x);
__nostd_api double
f64_asin_mm8(double x);
__nostd_api double
f64_asin_mm9(double x);
__nostd_api double
f64_asin_mm10(double x);
__nostd_api double
f64_asin_mm11(double x);
__nostd_api double
f64_asin_mm12(double x);
__nostd_api double
f64_asin_mm13(double x);
__nostd_api double
f64_asin_mm14(double x);
__nostd_api double
f64_asin_mm15(double x);

__nostd_api float
f32_asin_mm2(float x);
__nostd_api float
f32_asin_mm3(float x);
__nostd_api float
f32_asin_mm4(float x);
__nostd_api float
f32_asin_mm5(float x);
__nostd_api float
f32_asin_mm6(float x);
__nostd_api float
f32_asin_mm7(float x);
__nostd_api float
f32_asin_mm8(float x);
__nostd_api float
f32_asin_mm9(float x);
__nostd_api float
f32_asin_mm10(float x);
__nostd_api float
f32_asin_mm11(float x);
__nostd_api float
f32_asin_mm12(float x);
__nostd_api float
f32_asin_mm13(float x);
__nostd_api float
f32_asin_mm14(float x);
__nostd_api float
f32_asin_mm15(float x);

__nostd_api double
f64_acos_mm2(double x);
__nostd_api double
f64_acos_mm3(double x);
__nostd_api double
f64_acos_mm4(double x);
__nostd_api double
f64_acos_mm5(double x);
__nostd_api double
f64_acos_mm6(double x);
__nostd_api double
f64_acos_mm7(double x);
__nostd_api double
f64_acos_mm8(double x);
__nostd_api double
f64_acos_mm9(double x);
__nostd_api double
f64_acos_mm10(double x);
__nostd_api double
f64_acos_mm11(double x);
__nostd_api double
f64_acos_mm12(double x);
__nostd_api double
f64_acos_mm13(double x);
__nostd_api double
f64_acos_mm14(double x);
__nostd_api double
f64_acos_mm15(double x);

__nostd_api float
f32_acos_mm2(float x);
__nostd_api float
f32_acos_mm3(float x);
__nostd_api float
f32_acos_mm4(float x);
__nostd_api float
f32_acos_mm5(float x);
__nostd_api float
f32_acos_mm6(float x);
__nostd_api float
f32_acos_mm7(float x);
__nostd_api float
f32_acos_mm8(float x);
__nostd_api float
f32_acos_mm9(float x);
__nostd_api float
f32_acos_mm10(float x);
__nostd_api float
f32_acos_mm11(float x);
__nostd_api float
f32_acos_mm12(float x);
__nostd_api float
f32_acos_mm13(float x);
__nostd_api float
f32_acos_mm14(float x);
__nostd_api float
f32_acos_mm15(float x);

__nostd_api double
f64_atan_mm2(double x);
__nostd_api double
f64_atan_mm3(double x);
__nostd_api double
f64_atan_mm4(double x);
__nostd_api double
f64_atan_mm5(double x);
__nostd_api double
f64_atan_mm6(double x);
__nostd_api double
f64_atan_mm7(double x);
__nostd_api double
f64_atan_mm8(double x);
__nostd_api double
f64_atan_mm9(double x);
__nostd_api double
f64_atan_mm10(double x);
__nostd_api double
f64_atan_mm11(double x);
__nostd_api double
f64_atan_mm12(double x);
__nostd_api double
f64_atan_mm13(double x);
__nostd_api double
f64_atan_mm14(double x);
__nostd_api double
f64_atan_mm15(double x);

__nostd_api float
f32_atan_mm2(float x);
__nostd_api float
f32_atan_mm3(float x);
__nostd_api float
f32_atan_mm4(float x);
__nostd_api float
f32_atan_mm5(float x);
__nostd_api float
f32_atan_mm6(float x);
__nostd_api float
f32_atan_mm7(float x);
__nostd_api float
f32_atan_mm8(float x);
__nostd_api float
f32_atan_mm9(float x);
__nostd_api float
f32_atan_mm10(float x);
__nostd_api float
f32_atan_mm11(float x);
__nostd_api float
f32_atan_mm12(float x);
__nostd_api float
f32_atan_mm13(float x);
__nostd_api float
f32_atan_mm14(float x);
__nostd_api float
f32_atan_mm15(float x);

//////////////////////////
// Function Definitions //
//////////////////////////
#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_FLOATS__)

// F64
__nostd_api int
f64_iszero(double x)
{
	return x < F64_EPSILON;
}

__nostd_api int
f64_iseq(double x,
	double y)
{
	double f1;
	double f2;
	double fdiff;

	// Exact Equality
	if (x == y)
		return 1;

	// Near Zero
	f1 = f64_abs(x);
	f2 = f64_abs(y);
	fdiff = f64_abs(f2 - f1);
	if (fdiff < F64_EPSILON)
		return 1;

	// Relative Equality
	#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		double rdiv = f1 + f2;
		rdiv = rdiv < F64_MAX ? rdiv : F64_MAX;
		if ((fdiff / rdiv) < F64_EPSILON)
			return 1;
	#else
		F64i rdiv = { .d = f1+f2 };
		rdiv.ull = __nostd_iminf(rdiv.ull, F64_MAX_BITS, rdiv.d, F64_MAX);
		if ((fdiff / rdiv.d) < F64_EPSILON)
			return 1;
	#endif

	return 0;
}

__nostd_api double
f64_ceil(double x)
{
	int val = (int)x;
	double isneg = x < 0 ? 1.0f : 0.0f;
	return (double)(val + 1 - isneg);
}

__nostd_api double
f64_floor(double x)
{
	int val = (int)x;
	double isneg = x < 0 ? 1.0f : 0.0f;
	return (double)(val - isneg);
}

__nostd_api double
f64_abs(double x)
{
	F64i val = { .d = x };
	val.uhigh &= 0x7FFFFFFF;
	return val.d;
}

__nostd_api double
f64_mod(double x, double mod)
{
	int q;
	double ans;

	if (f64_iszero(mod))
		return 0;


	// NOTE: Why include errors if we don't have to?
	q = (int)(x / mod);
	ans = x - ((double)q * mod);
	if (f64_iseq(x, ans))
		return x;

	return ans;
}

__nostd_api double
f64_sqrt(double x)
{
	return 1.0 / f64_isqrt(x);
}

__nostd_api double
f64_isqrt(double x)
{
#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	return (double)f32_isqrt((float)x);
#else
	double xhalf;
	F64i  val;

	val.d = x;
	val.ll = 0x5FE6EB50C7B537A9 - (val.ll >> 1);

	xhalf = x * 0.5;
	val.d *= 1.5f - xhalf * val.d * val.d;
	val.d *= 1.5f - xhalf * val.d * val.d;
	val.d *= 1.5f - xhalf * val.d * val.d;

	return val.d;
#endif
}

// F32
__nostd_api int
f32_iszero(float x)
{
	return f32_abs(x) < F32_EPSILON;
}

__nostd_api int
f32_iseq(float x,
	float y)
{
	float f1;
	float f2;
	float fdiff;
	F32i  rdiv;

	// Exact Equality
	if (x == y)
		return 1;

	// Near Zero
	f1 = f32_abs(x);
	f2 = f32_abs(y);
	fdiff = f32_abs(f2 - f1);
	if (fdiff < F32_EPSILON)
		return 1;

	// Relative Equality
	rdiv.f = f1 + f2;
	rdiv.i = __nostd_iminf(rdiv.u, F32_MAX_BITS, rdiv.f, F32_MAX);
	if ((fdiff / rdiv.f) < F32_EPSILON)
		return 1;

	return 0;
}

__nostd_api float
f32_ceil(float x)
{
	int val;
	
	val = (int)x;
	val = val + 1 - (((unsigned int)val & 0x80000000) >> 31);
	return (float)(val);
}

__nostd_api float
f32_floor(float x)
{
	int val;

	val = (int)x;
	val = val - (((unsigned int)val & 0x80000000) >> 31);
	return (float)(val);
}

__nostd_api float
f32_abs(float x)
{
	F32i val = { .f = x };
	val.i &= 0x7FFFFFFF;
	return val.f;
}

__nostd_api float
f32_mod(float x, float mod)
{
	return (float)f64_mod((double)x, (double)mod);
}

__nostd_api float
f32_sqrt(float x)
{
	return 1.0f / f32_isqrt(x);
}

__nostd_api float
f32_isqrt(float x)
{
	float xhalf;
	F32i  val;

	val.f = x;
	val.i = 0x5F375A86 - (val.i >> 1);

	xhalf = x * 0.5f;
	val.f *= 1.5f - xhalf * val.f * val.f;
	val.f *= 1.5f - xhalf * val.f * val.f;
	val.f *= 1.5f - xhalf * val.f * val.f;

	return val.f;
}

////////////////////////
// sin(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_sin_mm2(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = -0x1.2b704cf682899p-3;

	ans_sin = xd2 * ans_sin + 0x1.fc4eac57b4a27p-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm2(float x)
{
	return (float)f64_sin_mm2((double)x);
}

////////////////////////
// sin(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_sin_mm3(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = 0x1.f2438d36d9dbbp-8;

	ans_sin = xd2 * ans_sin - 0x1.53e2e5c7dd831p-3;
	ans_sin = xd2 * ans_sin + 0x1.fff1d21fa9dedp-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm3(float x)
{
	return (float)f64_sin_mm3((double)x);
}

////////////////////////
// sin(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_sin_mm4(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = -0x1.83b9725dff6e8p-13;

	ans_sin = xd2 * ans_sin + 0x1.105d44e6222ap-7;
	ans_sin = xd2 * ans_sin - 0x1.554f800fc5ea1p-3;
	ans_sin = xd2 * ans_sin + 0x1.ffffe07d31fe8p-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm4(float x)
{
	return (float)f64_sin_mm4((double)x);
}

////////////////////////
// sin(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_sin_mm5(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = 0x1.5d38b56aee7f1p-19;

	ans_sin = xd2 * ans_sin - 0x1.9f6445023f795p-13;
	ans_sin = xd2 * ans_sin + 0x1.110e7b396c557p-7;
	ans_sin = xd2 * ans_sin - 0x1.555547ef5150bp-3;
	ans_sin = xd2 * ans_sin + 0x1.ffffffd25a681p-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm5(float x)
{
	return (float)f64_sin_mm5((double)x);
}

////////////////////////
// sin(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_sin_mm6(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = -0x1.9a0e192a4e2cbp-26;

	ans_sin = xd2 * ans_sin + 0x1.716ba4fe56f6ep-19;
	ans_sin = xd2 * ans_sin - 0x1.a017a8fe15033p-13;
	ans_sin = xd2 * ans_sin + 0x1.11110b74adb14p-7;
	ans_sin = xd2 * ans_sin - 0x1.55555541759fap-3;
	ans_sin = xd2 * ans_sin + 0x1.ffffffffd17d1p-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm6(float x)
{
	return (float)f64_sin_mm6((double)x);
}

////////////////////////
// sin(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_sin_mm7(double x)
{
	double xd;
	double xd2;
	double ans_sin;

	// Range Reduction
	xd = (double)f64_abs(x);
	if (xd > F64_PI2)
		xd = F64_PI - xd;

	// Calculation
	xd2 = xd * xd;
	ans_sin = 0x1.52ace959bd023p-33;

	ans_sin = xd2 * ans_sin - 0x1.ae00fd733fe8dp-26;
	ans_sin = xd2 * ans_sin + 0x1.71dce5ace58d2p-19;
	ans_sin = xd2 * ans_sin - 0x1.a019fce979937p-13;
	ans_sin = xd2 * ans_sin + 0x1.111111090f0bcp-7;
	ans_sin = xd2 * ans_sin - 0x1.5555555540b9bp-3;
	ans_sin = xd2 * ans_sin + 0x1.ffffffffffdcep-1;
	ans_sin *= xd;

	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api float
f32_sin_mm7(float x)
{
	return (float)f64_sin_mm7((double)x);
}

////////////////////////
// cos(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_cos_mm2(double x)
{
	return f64_sin_mm2(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm2(float x)
{
	return (float)f64_sin_mm2(F64_PI2 - (double)(x));
}

////////////////////////
// cos(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_cos_mm3(double x)
{
	return f64_sin_mm3(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm3(float x)
{
	return (float)f64_sin_mm3(F64_PI2 - (double)(x));
}

////////////////////////
// cos(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_cos_mm4(double x)
{
	return f64_sin_mm4(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm4(float x)
{
	return (float)f64_sin_mm4(F64_PI2 - (double)(x));
}

////////////////////////
// cos(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_cos_mm5(double x)
{
	return f64_sin_mm5(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm5(float x)
{
	return (float)f64_sin_mm5(F64_PI2 - (double)(x));
}

////////////////////////
// cos(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_cos_mm6(double x)
{
	return f64_sin_mm6(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm6(float x)
{
	return (float)f64_sin_mm6(F64_PI2 - (double)(x));
}

////////////////////////
// cos(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_cos_mm7(double x)
{
	return f64_sin_mm7(F64_PI2 - x);
}

__nostd_api float
f32_cos_mm7(float x)
{
	return (float)f64_sin_mm7(F64_PI2 - (double)(x));
}

////////////////////////
// tan(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_tan_mm2(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm2(xd);
	ans_cos = f64_sin_mm2(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm2(float x)
{
	return (float)f64_tan_mm2((double)x);
}

////////////////////////
// tan(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_tan_mm3(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm3(xd);
	ans_cos = f64_sin_mm3(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm3(float x)
{
	return (float)f64_tan_mm3((double)x);
}

////////////////////////
// tan(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_tan_mm4(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm4(xd);
	ans_cos = f64_sin_mm4(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm4(float x)
{
	return (float)f64_tan_mm4((double)x);
}

////////////////////////
// tan(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_tan_mm5(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm5(xd);
	ans_cos = f64_sin_mm5(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm5(float x)
{
	return (float)f64_tan_mm5((double)x);
}

////////////////////////
// tan(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_tan_mm6(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm6(xd);
	ans_cos = f64_sin_mm6(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm6(float x)
{
	return (float)f64_tan_mm6((double)x);
}

////////////////////////
// tan(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_tan_mm7(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_tan;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm7(xd);
	ans_cos = f64_sin_mm7(F64_PI2 - xd);
	ans_tan = ans_sin / ans_cos;

	if (xt > F32_PI2)
		return (x < 0) ? ans_tan : -ans_tan;

	return (x < 0) ? -ans_tan : ans_tan;
}

__nostd_api float
f32_tan_mm7(float x)
{
	return (float)f64_tan_mm7((double)x);
}

////////////////////////
// csc(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_csc_mm2(double x)
{
	return 1.0 / f64_sin_mm2(x);
}

__nostd_api float
f32_csc_mm2(float x)
{
	return (float)(1.0 / f64_sin_mm2((double)x));
}

////////////////////////
// csc(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_csc_mm3(double x)
{
	return 1.0 / f64_sin_mm3(x);
}

__nostd_api float
f32_csc_mm3(float x)
{
	return (float)(1.0 / f64_sin_mm3((double)x));
}

////////////////////////
// csc(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_csc_mm4(double x)
{
	return 1.0 / f64_sin_mm4(x);
}

__nostd_api float
f32_csc_mm4(float x)
{
	return (float)(1.0 / f64_sin_mm4((double)x));
}

////////////////////////
// csc(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_csc_mm5(double x)
{
	return 1.0 / f64_sin_mm5(x);
}

__nostd_api float
f32_csc_mm5(float x)
{
	return (float)(1.0 / f64_sin_mm5((double)x));
}

////////////////////////
// csc(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_csc_mm6(double x)
{
	return 1.0 / f64_sin_mm6(x);
}

__nostd_api float
f32_csc_mm6(float x)
{
	return (float)(1.0 / f64_sin_mm6((double)x));
}

////////////////////////
// csc(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_csc_mm7(double x)
{
	return 1.0 / f64_sin_mm7(x);
}

__nostd_api float
f32_csc_mm7(float x)
{
	return (float)(1.0 / f64_sin_mm7((double)x));
}

////////////////////////
// sec(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_sec_mm2(double x)
{
	return 1.0 / f64_sin_mm2(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm2(float x)
{
	return (float)(1.0 / f64_sin_mm2(F64_PI2 - (double)(x)));
}

////////////////////////
// sec(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_sec_mm3(double x)
{
	return 1.0 / f64_sin_mm3(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm3(float x)
{
	return (float)(1.0 / f64_sin_mm3(F64_PI2 - (double)(x)));
}

////////////////////////
// sec(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_sec_mm4(double x)
{
	return 1.0 / f64_sin_mm4(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm4(float x)
{
	return (float)(1.0 / f64_sin_mm4(F64_PI2 - (double)(x)));
}

////////////////////////
// sec(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_sec_mm5(double x)
{
	return 1.0 / f64_sin_mm5(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm5(float x)
{
	return (float)(1.0 / f64_sin_mm5(F64_PI2 - (double)(x)));
}

////////////////////////
// sec(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_sec_mm6(double x)
{
	return 1.0 / f64_sin_mm6(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm6(float x)
{
	return (float)(1.0 / f64_sin_mm6(F64_PI2 - (double)(x)));
}

////////////////////////
// sec(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_sec_mm7(double x)
{
	return 1.0 / f64_sin_mm7(F64_PI2 - x);
}

__nostd_api float
f32_sec_mm7(float x)
{
	return (float)(1.0 / f64_sin_mm7(F64_PI2 - (double)(x)));
}

////////////////////////
// cot(x) - Minimax 2 //
////////////////////////
__nostd_api double
f64_cot_mm2(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm2(xd);
	ans_cos = f64_sin_mm2(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm2(float x)
{
	return (float)f64_cot_mm2((double)x);
}

////////////////////////
// cot(x) - Minimax 3 //
////////////////////////
__nostd_api double
f64_cot_mm3(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm3(xd);
	ans_cos = f64_sin_mm3(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm3(float x)
{
	return (float)f64_cot_mm3((double)x);
}

////////////////////////
// cot(x) - Minimax 4 //
////////////////////////
__nostd_api double
f64_cot_mm4(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm4(xd);
	ans_cos = f64_sin_mm4(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm4(float x)
{
	return (float)f64_cot_mm4((double)x);
}

////////////////////////
// cot(x) - Minimax 5 //
////////////////////////
__nostd_api double
f64_cot_mm5(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm5(xd);
	ans_cos = f64_sin_mm5(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm5(float x)
{
	return (float)f64_cot_mm5((double)x);
}

////////////////////////
// cot(x) - Minimax 6 //
////////////////////////
__nostd_api double
f64_cot_mm6(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm6(xd);
	ans_cos = f64_sin_mm6(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm6(float x)
{
	return (float)f64_cot_mm6((double)x);
}

////////////////////////
// cot(x) - Minimax 7 //
////////////////////////
__nostd_api double
f64_cot_mm7(double x)
{
	double xd;
	double xt;
	double ans_sin;
	double ans_cos;
	double ans_cot;

	xt = f64_abs(x);
	xd = (xt > F32_PI2) ? F64_PI - xt : xt;

	ans_sin = f64_sin_mm7(xd);
	ans_cos = f64_sin_mm7(F64_PI2 - xd);
	ans_cot = ans_cos / ans_sin;

	if (xt > F32_PI2)
		return (x < 0) ? ans_cot : -ans_cot;

	return (x < 0) ? -ans_cot : ans_cot;
}

__nostd_api float
f32_cot_mm7(float x)
{
	return (float)f64_cot_mm7((double)x);
}

/////////////////////////
// asin(x) - Minimax 2 //
/////////////////////////
__nostd_api double
f64_asin_mm2(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.c427597754a37p-3;

	ans_asin = xd2 * ans_asin + 0x1.fdfcefbdd3154p-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm2(float x)
{
	return (float)f64_asin_mm2((double)x);
}

/////////////////////////
// asin(x) - Minimax 3 //
/////////////////////////
__nostd_api double
f64_asin_mm3(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.0da162d6fae3dp-3;

	ans_asin = xd2 * ans_asin + 0x1.3b5f83d579a47p-3;
	ans_asin = xd2 * ans_asin + 0x1.0019e5b9a7693p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm3(float x)
{
	return (float)f64_asin_mm3((double)x);
}

/////////////////////////
// asin(x) - Minimax 4 //
/////////////////////////
__nostd_api double
f64_asin_mm4(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.b0931e5a07f25p-4;

	ans_asin = xd2 * ans_asin + 0x1.a52ade47d967dp-5;
	ans_asin = xd2 * ans_asin + 0x1.5acaca323d3aep-3;
	ans_asin = xd2 * ans_asin + 0x1.fffa004bed736p-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm4(float x)
{
	return (float)f64_asin_mm4((double)x);
}

/////////////////////////
// asin(x) - Minimax 5 //
/////////////////////////
__nostd_api double
f64_asin_mm5(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.8faf3815344ddp-4;

	ans_asin = xd2 * ans_asin + 0x1.c36707c70d21cp-8;
	ans_asin = xd2 * ans_asin + 0x1.52db50c86c17fp-4;
	ans_asin = xd2 * ans_asin + 0x1.543eb056cd449p-3;
	ans_asin = xd2 * ans_asin + 0x1.0000609783343p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm5(float x)
{
	return (float)f64_asin_mm5((double)x);
}

/////////////////////////
// asin(x) - Minimax 6 //
/////////////////////////
__nostd_api double
f64_asin_mm6(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.912219085f248p-4;

	ans_asin = xd2 * ans_asin - 0x1.ac22c3939a9a9p-6;
	ans_asin = xd2 * ans_asin + 0x1.f96fb970de571p-5;
	ans_asin = xd2 * ans_asin + 0x1.2a2202ec5cb8p-4;
	ans_asin = xd2 * ans_asin + 0x1.558b2dc0be61cp-3;
	ans_asin = xd2 * ans_asin + 0x1.ffffe6586d628p-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm6(float x)
{
	return (float)f64_asin_mm6((double)x);
}

/////////////////////////
// asin(x) - Minimax 7 //
/////////////////////////
__nostd_api double
f64_asin_mm7(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.a906b64a9bdc7p-4;

	ans_asin = xd2 * ans_asin - 0x1.e523c15fbf438p-5;
	ans_asin = xd2 * ans_asin + 0x1.03149c11e9277p-4;
	ans_asin = xd2 * ans_asin + 0x1.391ca703d0d07p-5;
	ans_asin = xd2 * ans_asin + 0x1.35948cff7046bp-4;
	ans_asin = xd2 * ans_asin + 0x1.554b23dabce0bp-3;
	ans_asin = xd2 * ans_asin + 0x1.000001c517503p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm7(float x)
{
	return (float)f64_asin_mm7((double)x);
}

/////////////////////////
// asin(x) - Minimax 8 //
/////////////////////////
__nostd_api double
f64_asin_mm8(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.d3ec796e5ec8bp-4;

	ans_asin = xd2 * ans_asin - 0x1.912818d0401bfp-4;
	ans_asin = xd2 * ans_asin + 0x1.4b1dad0e7b7e5p-4;
	ans_asin = xd2 * ans_asin + 0x1.ea3ebd68f4abp-7;
	ans_asin = xd2 * ans_asin + 0x1.7f40be8459b49p-5;
	ans_asin = xd2 * ans_asin + 0x1.329cc1329ab48p-4;
	ans_asin = xd2 * ans_asin + 0x1.55573c38fd397p-3;
	ans_asin = xd2 * ans_asin + 0x1.ffffff7f5bbbcp-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm8(float x)
{
	return (float)f64_asin_mm8((double)x);
}

/////////////////////////
// asin(x) - Minimax 9 //
/////////////////////////
__nostd_api double
f64_asin_mm9(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.0903eea6d1357p-3;

	ans_asin = xd2 * ans_asin - 0x1.2c7c4453a9b3ep-3;
	ans_asin = xd2 * ans_asin + 0x1.db45568eb9217p-4;
	ans_asin = xd2 * ans_asin - 0x1.5b983df09138p-7;
	ans_asin = xd2 * ans_asin + 0x1.2c25059387c7ep-5;
	ans_asin = xd2 * ans_asin + 0x1.685b6636af595p-5;
	ans_asin = xd2 * ans_asin + 0x1.3356afe11c66p-4;
	ans_asin = xd2 * ans_asin + 0x1.5554fb74b4bffp-3;
	ans_asin = xd2 * ans_asin + 0x1.00000009557b8p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm9(float x)
{
	return (float)f64_asin_mm9((double)x);
}

//////////////////////////
// asin(x) - Minimax 10 //
//////////////////////////
__nostd_api double
f64_asin_mm10(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.32de0e0b3820fp-3;

	ans_asin = xd2 * ans_asin - 0x1.aed7e0dfdbd27p-3;
	ans_asin = xd2 * ans_asin + 0x1.656a2ea43451dp-3;
	ans_asin = xd2 * ans_asin - 0x1.88322c8ce661fp-5;
	ans_asin = xd2 * ans_asin + 0x1.38c39ea555adep-5;
	ans_asin = xd2 * ans_asin + 0x1.cc5fc8ed87fdbp-6;
	ans_asin = xd2 * ans_asin + 0x1.6f3ed264eef28p-5;
	ans_asin = xd2 * ans_asin + 0x1.332b1eab3d6f2p-4;
	ans_asin = xd2 * ans_asin + 0x1.555565c9beb43p-3;
	ans_asin = xd2 * ans_asin + 0x1.fffffffd3e442p-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm10(float x)
{
	return (float)f64_asin_mm10((double)x);
}

//////////////////////////
// asin(x) - Minimax 11 //
//////////////////////////
__nostd_api double
f64_asin_mm11(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.699a7715830d2p-3;

	ans_asin = xd2 * ans_asin - 0x1.2deb335977b56p-2;
	ans_asin = xd2 * ans_asin + 0x1.103aa8bb00a4ep-2;
	ans_asin = xd2 * ans_asin - 0x1.ba657aa72abeep-4;
	ans_asin = xd2 * ans_asin + 0x1.b627b3be92bd4p-5;
	ans_asin = xd2 * ans_asin + 0x1.0076fe3314273p-6;
	ans_asin = xd2 * ans_asin + 0x1.fe5b240c320ebp-6;
	ans_asin = xd2 * ans_asin + 0x1.6d4c8c3659p-5;
	ans_asin = xd2 * ans_asin + 0x1.3334fd1dd69f5p-4;
	ans_asin = xd2 * ans_asin + 0x1.5555525723f64p-3;
	ans_asin = xd2 * ans_asin + 0x1.0000000034db9p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm11(float x)
{
	return (float)f64_asin_mm11((double)x);
}

//////////////////////////
// asin(x) - Minimax 12 //
//////////////////////////
__nostd_api double
f64_asin_mm12(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.b038b25a40e08p-3;

	ans_asin = xd2 * ans_asin - 0x1.a1b0aa236a282p-2;
	ans_asin = xd2 * ans_asin + 0x1.9e1b8885f9661p-2;
	ans_asin = xd2 * ans_asin - 0x1.a8326f2354f8ap-3;
	ans_asin = xd2 * ans_asin + 0x1.73bb5b359003ap-4;
	ans_asin = xd2 * ans_asin - 0x1.ac779f1be0507p-13;
	ans_asin = xd2 * ans_asin + 0x1.992d0b8b03f01p-6;
	ans_asin = xd2 * ans_asin + 0x1.edd05e3dff008p-6;
	ans_asin = xd2 * ans_asin + 0x1.6dd27e8c33d52p-5;
	ans_asin = xd2 * ans_asin + 0x1.3332d0221f548p-4;
	ans_asin = xd2 * ans_asin + 0x1.555555dff5e06p-3;
	ans_asin = xd2 * ans_asin + 0x1.ffffffffeff9dp-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm12(float x)
{
	return (float)f64_asin_mm12((double)x);
}

//////////////////////////
// asin(x) - Minimax 13 //
//////////////////////////
__nostd_api double
f64_asin_mm13(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.056424720e768p-2;

	ans_asin = xd2 * ans_asin - 0x1.1eabdc3fe561ap-1;
	ans_asin = xd2 * ans_asin + 0x1.38e97b1392a69p-1;
	ans_asin = xd2 * ans_asin - 0x1.7a954b7cb46e6p-2;
	ans_asin = xd2 * ans_asin + 0x1.53df7e2c17602p-3;
	ans_asin = xd2 * ans_asin - 0x1.cbd84d319158p-6;
	ans_asin = xd2 * ans_asin + 0x1.9a8031b47fd85p-6;
	ans_asin = xd2 * ans_asin + 0x1.5f396c79d5687p-6;
	ans_asin = xd2 * ans_asin + 0x1.f2f65baf85a8cp-6;
	ans_asin = xd2 * ans_asin + 0x1.6dafeb7453ee6p-5;
	ans_asin = xd2 * ans_asin + 0x1.33334839e1acap-4;
	ans_asin = xd2 * ans_asin + 0x1.5555553c5c8a7p-3;
	ans_asin = xd2 * ans_asin + 0x1.00000000013ap0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm13(float x)
{
	return (float)f64_asin_mm13((double)x);
}

//////////////////////////
// asin(x) - Minimax 14 //
//////////////////////////
__nostd_api double
f64_asin_mm14(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.3f5545e9e11eap-2;

	ans_asin = xd2 * ans_asin - 0x1.8792b45ef365ep-1;
	ans_asin = xd2 * ans_asin + 0x1.d52ccc09ba2cdp-1;
	ans_asin = xd2 * ans_asin - 0x1.4434b98838c1dp-1;
	ans_asin = xd2 * ans_asin + 0x1.3a638b5965e45p-2;
	ans_asin = xd2 * ans_asin - 0x1.5210c527bd7ep-4;
	ans_asin = xd2 * ans_asin + 0x1.1b4dda11bb1d2p-5;
	ans_asin = xd2 * ans_asin + 0x1.d2a82629eb78ep-7;
	ans_asin = xd2 * ans_asin + 0x1.73b9408ccb9b1p-6;
	ans_asin = xd2 * ans_asin + 0x1.f17068ec7ac68p-6;
	ans_asin = xd2 * ans_asin + 0x1.6db88c4cfe8eap-5;
	ans_asin = xd2 * ans_asin + 0x1.33332ecf01c13p-4;
	ans_asin = xd2 * ans_asin + 0x1.55555559d0d4p-3;
	ans_asin = xd2 * ans_asin + 0x1.ffffffffff9fp-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm14(float x)
{
	return (float)f64_asin_mm14((double)x);
}

//////////////////////////
// asin(x) - Minimax 15 //
//////////////////////////
__nostd_api double
f64_asin_mm15(double x)
{
	double xt;
	double xd;
	double xd2;
	double ans_asin;

	// Range Reduction
	xt = f64_abs(x);
	xd = (xt > F32_PI4) ? f64_sqrt(1.0 - (xt * xt)) : xt;

	// Calculation
	xd2 = xd * xd;
	ans_asin = 0x1.8978c6502660ap-2;

	ans_asin = xd2 * ans_asin - 0x1.0a98c5604a5c6p0;
	ans_asin = xd2 * ans_asin + 0x1.5d065bf34c03ep0;
	ans_asin = xd2 * ans_asin - 0x1.0e0b5512f8d35p0;
	ans_asin = xd2 * ans_asin + 0x1.1f42350f23ccep-1;
	ans_asin = xd2 * ans_asin - 0x1.850e0d65729e1p-3;
	ans_asin = xd2 * ans_asin + 0x1.007f36ef69d66p-4;
	ans_asin = xd2 * ans_asin + 0x1.34a6d9f27428dp-8;
	ans_asin = xd2 * ans_asin + 0x1.2f8bd23b33763p-6;
	ans_asin = xd2 * ans_asin + 0x1.6ce213041c326p-6;
	ans_asin = xd2 * ans_asin + 0x1.f1defdcf41a11p-6;
	ans_asin = xd2 * ans_asin + 0x1.6db67483a8f77p-5;
	ans_asin = xd2 * ans_asin + 0x1.3333341adb0b8p-4;
	ans_asin = xd2 * ans_asin + 0x1.5555555487dd3p-3;
	ans_asin = xd2 * ans_asin + 0x1.0000000000079p0;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api float
f32_asin_mm15(float x)
{
	return (float)f64_asin_mm15((double)x);
}

/////////////////////////
// acos(x) - Minimax 2 //
/////////////////////////
__nostd_api double
f64_acos_mm2(double x)
{
	return F64_PI2 - f64_asin_mm2(x);
}

__nostd_api float
f32_acos_mm2(float x)
{
	return (float)(F64_PI2 - f64_asin_mm2((double)x));
}

/////////////////////////
// acos(x) - Minimax 3 //
/////////////////////////
__nostd_api double
f64_acos_mm3(double x)
{
	return F64_PI2 - f64_asin_mm3(x);
}

__nostd_api float
f32_acos_mm3(float x)
{
	return (float)(F64_PI2 - f64_asin_mm3((double)x));
}

/////////////////////////
// acos(x) - Minimax 4 //
/////////////////////////
__nostd_api double
f64_acos_mm4(double x)
{
	return F64_PI2 - f64_asin_mm4(x);
}

__nostd_api float
f32_acos_mm4(float x)
{
	return (float)(F64_PI2 - f64_asin_mm4((double)x));
}

/////////////////////////
// acos(x) - Minimax 5 //
/////////////////////////
__nostd_api double
f64_acos_mm5(double x)
{
	return F64_PI2 - f64_asin_mm5(x);
}

__nostd_api float
f32_acos_mm5(float x)
{
	return (float)(F64_PI2 - f64_asin_mm5((double)x));
}

/////////////////////////
// acos(x) - Minimax 6 //
/////////////////////////
__nostd_api double
f64_acos_mm6(double x)
{
	return F64_PI2 - f64_asin_mm6(x);
}

__nostd_api float
f32_acos_mm6(float x)
{
	return (float)(F64_PI2 - f64_asin_mm6((double)x));
}

/////////////////////////
// acos(x) - Minimax 7 //
/////////////////////////
__nostd_api double
f64_acos_mm7(double x)
{
	return F64_PI2 - f64_asin_mm7(x);
}

__nostd_api float
f32_acos_mm7(float x)
{
	return (float)(F64_PI2 - f64_asin_mm7((double)x));
}

/////////////////////////
// acos(x) - Minimax 8 //
/////////////////////////
__nostd_api double
f64_acos_mm8(double x)
{
	return F64_PI2 - f64_asin_mm8(x);
}

__nostd_api float
f32_acos_mm8(float x)
{
	return (float)(F64_PI2 - f64_asin_mm8((double)x));
}

/////////////////////////
// acos(x) - Minimax 9 //
/////////////////////////
__nostd_api double
f64_acos_mm9(double x)
{
	return F64_PI2 - f64_asin_mm9(x);
}

__nostd_api float
f32_acos_mm9(float x)
{
	return (float)(F64_PI2 - f64_asin_mm9((double)x));
}

//////////////////////////
// acos(x) - Minimax 10 //
//////////////////////////
__nostd_api double
f64_acos_mm10(double x)
{
	return F64_PI2 - f64_asin_mm10(x);
}

__nostd_api float
f32_acos_mm10(float x)
{
	return (float)(F64_PI2 - f64_asin_mm10((double)x));
}

//////////////////////////
// acos(x) - Minimax 11 //
//////////////////////////
__nostd_api double
f64_acos_mm11(double x)
{
	return F64_PI2 - f64_asin_mm11(x);
}

__nostd_api float
f32_acos_mm11(float x)
{
	return (float)(F64_PI2 - f64_asin_mm11((double)x));
}

//////////////////////////
// acos(x) - Minimax 12 //
//////////////////////////
__nostd_api double
f64_acos_mm12(double x)
{
	return F64_PI2 - f64_asin_mm12(x);
}

__nostd_api float
f32_acos_mm12(float x)
{
	return (float)(F64_PI2 - f64_asin_mm12((double)x));
}

//////////////////////////
// acos(x) - Minimax 13 //
//////////////////////////
__nostd_api double
f64_acos_mm13(double x)
{
	return F64_PI2 - f64_asin_mm13(x);
}

__nostd_api float
f32_acos_mm13(float x)
{
	return (float)(F64_PI2 - f64_asin_mm13((double)x));
}

//////////////////////////
// acos(x) - Minimax 14 //
//////////////////////////
__nostd_api double
f64_acos_mm14(double x)
{
	return F64_PI2 - f64_asin_mm14(x);
}

__nostd_api float
f32_acos_mm14(float x)
{
	return (float)(F64_PI2 - f64_asin_mm14((double)x));
}

//////////////////////////
// acos(x) - Minimax 15 //
//////////////////////////
__nostd_api double
f64_acos_mm15(double x)
{
	return F64_PI2 - f64_asin_mm15(x);
}

__nostd_api float
f32_acos_mm15(float x)
{
	return (float)(F64_PI2 - f64_asin_mm15((double)x));
}

/////////////////////////
// atan(x) - Minimax 2 //
/////////////////////////
__nostd_api double
f64_atan_mm2(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm2(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm2(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm2((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 3 //
/////////////////////////
__nostd_api double
f64_atan_mm3(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm3(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm3(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm3((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 4 //
/////////////////////////
__nostd_api double
f64_atan_mm4(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm4(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm4(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm4((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 5 //
/////////////////////////
__nostd_api double
f64_atan_mm5(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm5(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm5(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm5((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 6 //
/////////////////////////
__nostd_api double
f64_atan_mm6(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm6(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm6(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm6((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 7 //
/////////////////////////
__nostd_api double
f64_atan_mm7(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm7(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm7(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm7((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 8 //
/////////////////////////
__nostd_api double
f64_atan_mm8(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm8(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm8(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm8((double)(x) / f64_sqrt(1 + x2)));
}

/////////////////////////
// atan(x) - Minimax 9 //
/////////////////////////
__nostd_api double
f64_atan_mm9(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm9(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm9(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm9((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 10 //
//////////////////////////
__nostd_api double
f64_atan_mm10(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm10(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm10(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm10((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 11 //
//////////////////////////
__nostd_api double
f64_atan_mm11(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm11(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm11(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm11((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 12 //
//////////////////////////
__nostd_api double
f64_atan_mm12(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm12(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm12(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm12((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 13 //
//////////////////////////
__nostd_api double
f64_atan_mm13(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm13(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm13(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm13((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 14 //
//////////////////////////
__nostd_api double
f64_atan_mm14(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm14(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm14(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm14((double)(x) / f64_sqrt(1 + x2)));
}

//////////////////////////
// atan(x) - Minimax 15 //
//////////////////////////
__nostd_api double
f64_atan_mm15(double x)
{
	// TODO: Do Polynomial Approximation using Sollya
	return f64_asin_mm15(x / f64_sqrt(1 + x * x));
}

__nostd_api float
f32_atan_mm15(float x)
{
	double x2 = (double)x * (double)x;
	return (float)(f64_asin_mm15((double)(x) / f64_sqrt(1 + x2)));
}

////////////////////
// Trig Fn Arrays //
////////////////////
__nostd_api f64_trigfn f64_sin_minimax[] = {
	f64_sin_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_sin_mm2,   // Max Err: <= 0.1
	f64_sin_mm2,   // Max Err: <= 0.01
	f64_sin_mm3,   // Max Err: <= 0.001
	f64_sin_mm3,   // Max Err: <= 0.0001
	f64_sin_mm4,   // Max Err: <= 0.00001
	f64_sin_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_sin_mm5,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_sin_minimax[] = {
	f32_sin_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_sin_mm2,   // Max Err: <= 0.1
	f32_sin_mm2,   // Max Err: <= 0.01
	f32_sin_mm3,   // Max Err: <= 0.001
	f32_sin_mm3,   // Max Err: <= 0.0001
	f32_sin_mm4,   // Max Err: <= 0.00001
	f32_sin_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_sin_mm5,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_cos_minimax[] = {
	f64_cos_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_cos_mm2,   // Max Err: <= 0.1
	f64_cos_mm2,   // Max Err: <= 0.01
	f64_cos_mm3,   // Max Err: <= 0.001
	f64_cos_mm3,   // Max Err: <= 0.0001
	f64_cos_mm4,   // Max Err: <= 0.00001
	f64_cos_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_cos_mm5,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_cos_minimax[] = {
	f32_cos_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_cos_mm2,   // Max Err: <= 0.1
	f32_cos_mm2,   // Max Err: <= 0.01
	f32_cos_mm3,   // Max Err: <= 0.001
	f32_cos_mm3,   // Max Err: <= 0.0001
	f32_cos_mm4,   // Max Err: <= 0.00001
	f32_cos_mm4,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_cos_mm5,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_tan_minimax[] = {
	f64_tan_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f64_tan_mm2, // Avg: <= 0.1
	f64_tan_mm3, // Avg: <= 0.01
	f64_tan_mm4, // Avg: <= 0.001
	f64_tan_mm5, // Avg: <= 0.0001
	f64_tan_mm5, // Avg: <= 0.00001
	f64_tan_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f64_tan_mm7, // Avg: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_tan_minimax[] = {
	f32_tan_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f32_tan_mm2, // Avg: <= 0.1
	f32_tan_mm3, // Avg: <= 0.01
	f32_tan_mm4, // Avg: <= 0.001
	f32_tan_mm5, // Avg: <= 0.0001
	f32_tan_mm5, // Avg: <= 0.00001
	f32_tan_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f32_tan_mm7, // Avg: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_csc_minimax[] = {
	f64_csc_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_csc_mm2,   // Max Err: <= 0.1
	f64_csc_mm3,   // Max Err: <= 0.01
	f64_csc_mm4,   // Max Err: <= 0.001
	f64_csc_mm4,   // Max Err: <= 0.0001
	f64_csc_mm5,   // Max Err: <= 0.00001
	f64_csc_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_csc_mm7,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_csc_minimax[] = {
	f32_csc_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_csc_mm2,   // Max Err: <= 0.1
	f32_csc_mm3,   // Max Err: <= 0.01
	f32_csc_mm4,   // Max Err: <= 0.001
	f32_csc_mm4,   // Max Err: <= 0.0001
	f32_csc_mm5,   // Max Err: <= 0.00001
	f32_csc_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_csc_mm7,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_sec_minimax[] = {
	f64_sec_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_sec_mm2,   // Max Err: <= 0.1
	f64_sec_mm3,   // Max Err: <= 0.01
	f64_sec_mm4,   // Max Err: <= 0.001
	f64_sec_mm4,   // Max Err: <= 0.0001
	f64_sec_mm5,   // Max Err: <= 0.00001
	f64_sec_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_sec_mm7,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_sec_minimax[] = {
	f32_sec_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_sec_mm2,   // Max Err: <= 0.1
	f32_sec_mm3,   // Max Err: <= 0.01
	f32_sec_mm4,   // Max Err: <= 0.001
	f32_sec_mm4,   // Max Err: <= 0.0001
	f32_sec_mm5,   // Max Err: <= 0.00001
	f32_sec_mm6,   // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_sec_mm7,   // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_cot_minimax[] = {
	f64_cot_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f64_cot_mm2, // Avg: <= 0.1
	f64_cot_mm3, // Avg: <= 0.01
	f64_cot_mm4, // Avg: <= 0.001
	f64_cot_mm5, // Avg: <= 0.0001
	f64_cot_mm5, // Avg: <= 0.00001
	f64_cot_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f64_cot_mm7, // Avg: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_cot_minimax[] = {
	f32_cot_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f32_cot_mm2, // Avg: <= 0.1
	f32_cot_mm3, // Avg: <= 0.01
	f32_cot_mm4, // Avg: <= 0.001
	f32_cot_mm5, // Avg: <= 0.0001
	f32_cot_mm5, // Avg: <= 0.00001
	f32_cot_mm6, // Avg: <= 0.000001    [Rounded Epsilon]
	f32_cot_mm7, // Avg: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_asin_minimax[] = {
	f64_asin_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_asin_mm2,  // Max Err: <= 0.1
	f64_asin_mm3,  // Max Err: <= 0.01
	f64_asin_mm5,  // Max Err: <= 0.001
	f64_asin_mm7,  // Max Err: <= 0.0001
	f64_asin_mm9,  // Max Err: <= 0.00001
	f64_asin_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_asin_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_asin_minimax[] = {
	f32_asin_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_asin_mm2,  // Max Err: <= 0.1
	f32_asin_mm3,  // Max Err: <= 0.01
	f32_asin_mm5,  // Max Err: <= 0.001
	f32_asin_mm7,  // Max Err: <= 0.0001
	f32_asin_mm9,  // Max Err: <= 0.00001
	f32_asin_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_asin_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_acos_minimax[] = {
	f64_acos_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_acos_mm2,  // Max Err: <= 0.1
	f64_acos_mm3,  // Max Err: <= 0.01
	f64_acos_mm5,  // Max Err: <= 0.001
	f64_acos_mm7,  // Max Err: <= 0.0001
	f64_acos_mm9,  // Max Err: <= 0.00001
	f64_acos_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_acos_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
	// NOTE: mm14's max error for acos is 0.000000238!
	//       same with mm15. Unless its absolutely
	//       necessary to use floats, attempt to use
	//       doubles for higher accuracy.
};

__nostd_api f32_trigfn f32_acos_minimax[] = {
	f32_acos_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_acos_mm2,  // Max Err: <= 0.1
	f32_acos_mm3,  // Max Err: <= 0.01
	f32_acos_mm5,  // Max Err: <= 0.001
	f32_acos_mm7,  // Max Err: <= 0.0001
	f32_acos_mm9,  // Max Err: <= 0.00001
	f32_acos_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_acos_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
	// NOTE: mm14's max error for acos is 0.000000238!
	//       same with mm15. Unless its absolutely
	//       necessary to use floats, attempt to use
	//       doubles for higher accuracy.
};

__nostd_api f64_trigfn f64_atan_minimax[] = {
	f64_atan_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_atan_mm2,  // Max Err: <= 0.1
	f64_atan_mm3,  // Max Err: <= 0.01
	f64_atan_mm5,  // Max Err: <= 0.001
	f64_atan_mm7,  // Max Err: <= 0.0001
	f64_atan_mm9,  // Max Err: <= 0.00001
	f64_atan_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f64_atan_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f32_trigfn f32_atan_minimax[] = {
	f32_atan_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_atan_mm2,  // Max Err: <= 0.1
	f32_atan_mm3,  // Max Err: <= 0.01
	f32_atan_mm5,  // Max Err: <= 0.001
	f32_atan_mm7,  // Max Err: <= 0.0001
	f32_atan_mm9,  // Max Err: <= 0.00001
	f32_atan_mm11, // Max Err: <= 0.000001    [Rounded Epsilon]
	f32_atan_mm14, // Max Err: <= 0.000000119 [Machine Epsilon]
};

__nostd_api f64_trigfn f64_sin = f64_sin_mm4;
__nostd_api f64_trigfn f64_cos = f64_cos_mm4;
__nostd_api f64_trigfn f64_tan = f64_tan_mm6;
__nostd_api f64_trigfn f64_csc = f64_csc_mm4;
__nostd_api f64_trigfn f64_sec = f64_csc_mm4;
__nostd_api f64_trigfn f64_cot = f64_tan_mm6;
__nostd_api f64_trigfn f64_asin = f64_asin_mm11;
__nostd_api f64_trigfn f64_acos = f64_acos_mm11;
__nostd_api f64_trigfn f64_atan = f64_atan_mm11;

__nostd_api f32_trigfn f32_sin = f32_sin_mm4;
__nostd_api f32_trigfn f32_cos = f32_cos_mm4;
__nostd_api f32_trigfn f32_tan = f32_tan_mm6;
__nostd_api f32_trigfn f32_csc = f32_csc_mm4;
__nostd_api f32_trigfn f32_sec = f32_sec_mm4;
__nostd_api f32_trigfn f32_cot = f32_cot_mm6;
__nostd_api f32_trigfn f32_asin = f32_asin_mm11;
__nostd_api f32_trigfn f32_acos = f32_acos_mm11;
__nostd_api f32_trigfn f32_atan = f32_atan_mm11;

#endif // (__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_FLOATS__)
#endif // __STANDALONE_FLOAT_FUNCTIONS_H

