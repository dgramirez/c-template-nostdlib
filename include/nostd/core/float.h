/**************************************************************************
 * Name:   Float Functions (floats.h)
 * Author: Derrick G. Ramirez

 * Requirements:
 *     - #define __GET_NOSTD_STANDALONE_IMPLEMENTATION
 *     - Use this define above all nostd standalone files.
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
	- Function pointers. Have swappable math functions based on required
	  speed or accuracy.
		- eg. Pointers:  f64_sin, f64_sinf, f64_asin, f64_asinf
		-     Functions: f64_sin_minimax2, ... f64_sin_minimax11
		                 f64_asinminimax2, ... f64_asin_minimax22
		- Set defaults to accuracy
		- Try to use Sollya for the coefficients

	- Couple of functions:
		- exp
		- pow
		- log, log2, log10

	- Fix tan & atan to use sqrt(2)/2 [Pre-computed] instead of PI/4
*/

/////////////
// Headers //
/////////////
#include <emmintrin.h>

////////////////////////
// Standalone Defines //
////////////////////////
#ifndef __STANDALONE_FLOAT_FUNCTIONS_H
#define __STANDALONE_FLOAT_FUNCTIONS_H

#ifndef __nostd_api
	#define __nostd_api
#endif

#ifndef __nostd_f32iunion
	typedef union {
		float        f;
		unsigned int i;
	} F32i;

	#define __nostd_f32iunion
#endif

#ifndef __nostd_imin
	#ifndef __nostd_minmax_bits
		#define __nostd_minmax_bits(x, y) (((x) ^ (y)) & -((x) < (y)))
	#endif
	#define __nostd_imin(x, y) ((y) ^ __nostd_minmax_bits((x), (y)))
#endif

// F32 Defines
#ifndef F32_DIGITS
	#define F32_DIGITS           6
#endif

#ifndef F32_EPSILON
	#define F32_EPSILON          0.000001f
#endif

#ifndef F32_EPSILON_MACHINE
	#define F32_EPSILON_MACHINE  1.19e-07f
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

// F64
#ifndef F64_DIGITS
	#define F64_DIGITS           15
#endif

#ifndef F64_EPSILON
	#define F64_EPSILON          0.000000000000001f
#endif

#ifndef F64_EPSILON_MACHINE
	#define F64_EPSILON_MACHINE  2.22e-16f
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

#ifndef F32_DEG_TO_RAD
	#define F32_DEG_TO_RAD(deg)  0.01745329251994329576923690768489f * deg
#endif

#ifndef F32_RAD_TO_DEG
	#define F32_RAD_TO_DEG(rad)  57.2957795130823208767981548141051f * rad
#endif

/////////////////
//  Prototypes //
/////////////////

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

__nostd_api double
f64_pow(double x, double e);

__nostd_api double
f64_log2(double x);

__nostd_api double
f64_log10(double x);

__nostd_api double
f64_sqrt(double x);

__nostd_api double
f64_isqrt(double x);

__nostd_api double
f64_sin(double x);

__nostd_api double
f64_cos(double x);

__nostd_api double
f64_tan(double x);

__nostd_api double
f64_asin(double x);

__nostd_api double
f64_acos(double x);

__nostd_api double
f64_atan(double x);

__nostd_api double
f64_sin32(double x);

__nostd_api double
f64_cos32(double x);

__nostd_api double
f64_tan32(double x);

__nostd_api double
f64_csc32(double x);

__nostd_api double
f64_sec32(double x);

__nostd_api double
f64_cot32(double x);

__nostd_api double
f64_asin32(double x);

__nostd_api double
f64_acos32(double x);

__nostd_api double
f64_atan32(double x);

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

__nostd_api float
f32_sin(float x);

__nostd_api float
f32_cos(float x);

__nostd_api float
f32_tan(float x);

__nostd_api float
f32_asin(float x);

__nostd_api float
f32_acos(float x);

__nostd_api float
f32_atan(float x);

//////////////////////////
// Function Definitions //
//////////////////////////

//#if defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_FLOATS__)

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
	double rdiv;

	// Exact Equality
	if (x == y)
		return 1;

	// Near Zero
	f1     = f64_abs(x);
	f2     = f64_abs(y);
	fdiff  = f64_abs(f2 - f1);
	if (fdiff < F64_EPSILON)
		return 1;

	// Relative Equality
	rdiv = f1 + f2;
	rdiv = rdiv < F64_MAX ? rdiv : F64_MAX;
	if ((fdiff / rdiv) < F64_EPSILON)
		return 1;

	return 0;
}

__nostd_api double
f64_ceil(double x)
{
	int val      = (int)x;
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
	return x < 0 ? -x : x;
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
	__m128d mx = _mm_load_sd(&x);
	return _mm_cvtsd_f64(_mm_sqrt_sd(mx, mx));
}

__nostd_api double
f64_isqrt(double x)
{
	return f32_isqrt((float)x);
}

__nostd_api double
f64_sin32(double x)
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

	ans_sin  = xd2 * ans_sin - 0x1.9f6445023f795p-13;
	ans_sin  = xd2 * ans_sin + 0x1.110e7b396c557p-7;
	ans_sin  = xd2 * ans_sin - 0x1.555547ef5150bp-3;
	ans_sin  = xd2 * ans_sin + 0x1.ffffffd25a681p-1;
	ans_sin *= xd;
	
	return (x < 0) ? -ans_sin : ans_sin;
}

__nostd_api double
f64_cos32(double x)
{
	return f64_sin32(F64_PI2 - x);
}

__nostd_api double
f64_tan32(double x)
{
	double ans_sin;
	double ans_cos;
	double ans_tan;
	double sign;

	sign = 0.0;
	if (x > F64_PI4) {
		x = F64_PI2 - x;
		sign = 1.0f;
	}
	if (x < -F64_PI4) {
		x = F64_PI2 + x;
		sign = -1.0f;
	}

	ans_sin = f64_sin32(x);
	ans_cos = f64_cos32(x);      // TODO: If zero, return inf
	ans_tan = ans_sin / ans_cos;

	if (sign)
		return sign / ans_tan;

	return ans_tan;
}

__nostd_api double
f64_csc32(double x)
{
	double ans_sin;

	ans_sin = f64_sin32(x); // TODO: If zero, return inf
	return 1 / ans_sin;
}

__nostd_api double
f64_sec32(double x)
{
	double ans_cos;

	ans_cos = f64_cos32(x); // TODO: If zero, return inf
	return 1 / ans_cos;
}

__nostd_api double
f64_cot32(double x)
{
	double ans_sin;
	double ans_cos;
	double ans_tan;
	double sign;

	sign = 0.0;
	if (x > F64_PI4) {
		x = F64_PI2 - x;
		sign = 1.0f;
	}
	if (x < -F64_PI4) {
		x = F64_PI2 + x;
		sign = -1.0f;
	}

	ans_sin = f64_sin32(x); // TODO: If zero, return inf
	ans_cos = f64_cos32(x);
	ans_tan = ans_cos / ans_sin;

	if (sign)
		return sign / ans_tan;

	return ans_tan;
}

__nostd_api double
f64_asin32(double x)
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

	ans_asin  = xd2 * ans_asin - 0x1.a1b0aa236a282p-2;
	ans_asin  = xd2 * ans_asin + 0x1.9e1b8885f9661p-2;
	ans_asin  = xd2 * ans_asin - 0x1.a8326f2354f8ap-3;
	ans_asin  = xd2 * ans_asin + 0x1.73bb5b359003ap-4;
	ans_asin  = xd2 * ans_asin - 0x1.ac779f1be0507p-13;
	ans_asin  = xd2 * ans_asin + 0x1.992d0b8b03f01p-6;
	ans_asin  = xd2 * ans_asin + 0x1.edd05e3dff008p-6;
	ans_asin  = xd2 * ans_asin + 0x1.6dd27e8c33d52p-5;
	ans_asin  = xd2 * ans_asin + 0x1.3332d0221f548p-4;
	ans_asin  = xd2 * ans_asin + 0x1.555555dff5e06p-3;
	ans_asin  = xd2 * ans_asin + 0x1.ffffffffeff9dp-1;
	ans_asin *= xd;

	ans_asin = (xt > F32_PI4) ? (F32_PI2 - ans_asin) : ans_asin;
	return (x < 0) ? -ans_asin : ans_asin;
}

__nostd_api double
f64_acos32(double x)
{
	return F64_PI2 - f64_asin32(x);
}

__nostd_api double
f64_atan32(double x)
{
	return f64_asin32(x / f64_sqrt(1 + x*x));
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
	f1     = f32_abs(x);
	f2     = f32_abs(y);
	fdiff  = f32_abs(f2 - f1);
	if (fdiff < F32_EPSILON)
		return 1;

	// Relative Equality
	rdiv.f = f1 + f2;
	rdiv.i = __nostd_imin(rdiv.i, F32_MAX_BITS);
	if ((fdiff / rdiv.f) < F32_EPSILON)
		return 1;

	return 0;
}

__nostd_api float
f32_ceil(float x)
{
	int val = (int)x;
	return (float)(val + 1 - ((val & 0x80000000) >> 31));
}

__nostd_api float
f32_floor(float x)
{
	int val = (int)x;
	return (float)(val - ((val & 0x80000000) >> 31));
}

__nostd_api float
f32_abs(float x)
{
	F32i val = {.f = x};
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
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_load_ss(&x)));
}

__nostd_api float
f32_isqrt(float x)
{
	return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_load_ss(&x)));
}

__nostd_api float
f32_sin(float x)
{
	return (float)f64_sin32((double)x);
}

__nostd_api float
f32_cos(float x)
{
	return (float)f64_cos32((double)x);
}

__nostd_api float
f32_tan(float x)
{
	return (float)f64_tan32((double)x);
}

__nostd_api float
f32_csc(float x)
{
	return (float)f64_csc32((double)x);
}

__nostd_api float
f32_sec(float x)
{
	return (float)f64_sec32((double)x);
}

__nostd_api float
f32_cot(float x)
{
	return (float)f64_cot32((double)x);
}

__nostd_api float
f32_asin(float x)
{
	return (float)f64_asin32((double)x);
}

__nostd_api float
f32_acos(float x)
{
	return (float)f64_acos32((double)x);
}

__nostd_api float
f32_atan(float x)
{
	return (float)f64_tan32((double)x);
}

//#endif // (__GET_NOSTD_STANDALONE_IMPLEMENTATION__) || defined(__GET_NOSTD_STANDALONE_IMPLEMENTATION_FLOATS__)
#endif // __STANDALONE_FLOAT_FUNCTIONS_H

