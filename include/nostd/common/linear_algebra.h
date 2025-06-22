#ifndef INCLUDE_NOSTD_COMMON_LINEAR_ALGEBRA_H
#define INCLUDE_NOSTD_COMMON_LINEAR_ALGEBRA_H

/* Something for me to remember :
 * 
 * Mat4x4 (Row Major)
 *      Rx0 Rx1 Rx2 P0
 *      Ry0 Ry1 Ry2 P1
 *      Rz0 Rz1 Rz2 P2
 *       T0  T1  T2 G0
 * 
 * R: Rotation Matrix     [3x3 Matrix]
 * T: Translation Matrix  [3x1 Matrix]
 * P: Perspective Matrix  [1x3 Matrix]
 * G: Global Scale Scalar [1x1 Matrix, Scalar]
 * 
 * Rx: X Rotation Vector   [Vec3]
 * Ry: Y Rotation Vector   [Vec3]
 * Rz: Z Rotation Vector   [Vec3]
 *  T: Translation Vector  [Vec3]
 *  P: Perspective Vector  [f32 x3]
 *  G: Global Scale Scalar [f32 x1]
 */

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef union {
	f32  data[4];
	u32  bits[4];
	struct { f32 x, y, z, w; };
	struct { f32 u, v, p, q; };
	struct { f32 r, g, b, a; };
	struct { f32 R0, R1, R2, P; }; // Mat4 Vec4f's Indices 0-2
	struct { f32 Tx, Ty, Tz, G; }; // Mat4 Vec4f's Indices 3
} vec4f;

typedef union {
	f32   data[16];
	u32   bits[16];
	f32   m[4][4];
	vec4f v[4];
	struct { f32 m00, m01, m02, m03,
	             m10, m11, m12, m13,
	             m20, m21, m22, m23,
	             m30, m31, m32, m33;
	       };
	struct { f32 Rx0, Rx1, Rx2, P0,
	             Ry0, Ry1, Ry2, P1,
	             Rz0, Rz1, Rz2, P2,
	              Tx,  Ty,  Tz,  G;
	       };
} mat4f;

typedef union {
	f32  data[4];
	struct { f32 i, j, k, w; };
} quat4f;

// Function Pointers: Vec4
typedef f32  (*PFN_Vec4FltUni)   (vec4f *v);
typedef int  (*PFN_Vec4UniBool)  (vec4f *v1);
typedef f32  (*PFN_Vec4FltBi)    (vec4f *v1,
                                  vec4f *v2);
typedef int  (*PFN_Vec4BiBool)   (vec4f *v1,
                                  vec4f *v2);
typedef void (*PFN_Vec4Uni)      (vec4f *out,
                                  vec4f *v);
typedef void (*PFN_Vec4Bi)       (vec4f *out,
                                  vec4f *v1,
                                  vec4f *v2);
typedef void (*PFN_Vec4BiScalar) (vec4f *out,
                                  vec4f *v1,
                                  f32 scalar);

local PFN_Vec4UniBool  v4f_iszero;
local PFN_Vec4BiBool   v4f_iseq;
local PFN_Vec4Uni      v4f_abs;
local PFN_Vec4Bi       v4f_add;
local PFN_Vec4Bi       v4f_sub;
local PFN_Vec4Bi       v4f_mulcv;
local PFN_Vec4Bi       v4f_divcv;
local PFN_Vec4BiScalar v4f_mulsv;
local PFN_Vec4BiScalar v4f_divsv;
local PFN_Vec4Bi       v4f_min;
local PFN_Vec4Bi       v4f_max;
local PFN_Vec4FltBi    v4f_dot;
local PFN_Vec4Bi       v4f_cross3;
local PFN_Vec4FltUni   v4f_length;
local PFN_Vec4FltUni   v4f_lengthsq;
local PFN_Vec4Uni      v4f_normalize;
local PFN_Vec4Uni      v4f_homogenize;
local PFN_Vec4Uni      v4f_negate;
local PFN_Vec4FltBi    v4f_anglebetween;
local PFN_Vec4FltBi    v4f_component;
local PFN_Vec4Bi       v4f_project;
local PFN_Vec4Bi       v4f_reflect;

local int
v4fb_iszero(vec4f *v);

local int
v4fb_iseq(vec4f *v1,
          vec4f *v2);

local void
v4fb_abs(vec4f *out,
         vec4f *v);

local void
v4fb_add(vec4f *out,
         vec4f *v1,
         vec4f *v2);

local void
v4fb_sub(vec4f *out,
         vec4f *v1,
         vec4f *v2);

local void
v4fb_mulcv(vec4f *out,
           vec4f *v1,
           vec4f *v2);

local void
v4fb_divcv(vec4f *out,
           vec4f *v1,
           vec4f *v2);

local void
v4fb_mulsv(vec4f *out,
           vec4f *v,
           f32 s);

local void
v4fb_divsv(vec4f *out,
           vec4f *v,
           f32 s);

local void
v4fb_negate(vec4f *out,
            vec4f *v);

local void
v4fb_min(vec4f *out,
         vec4f *v1,
         vec4f *v2);

local void
v4fb_max(vec4f *out,
         vec4f *v1,
         vec4f *v2);

local f32
v4fb_dot(vec4f *v1,
         vec4f *v2);

local f32
v4fb_lengthsq(vec4f *v);

local f32
v4fb_length(vec4f *v);

local void
v4fb_cross3(vec4f *out,
            vec4f *v1,
            vec4f *v2);

local void
v4fb_normalize(vec4f *out,
               vec4f *v);

local void
v4fb_homogenize(vec4f *out,
                vec4f *v);

local f32
v4fb_anglebetween(vec4f *v1,
                  vec4f *v2);

local f32
v4fb_component(vec4f *v1,
               vec4f *v2);

local void
v4fb_project(vec4f *out,
             vec4f *v1,
             vec4f *v2);

local void
v4fb_reflect(vec4f *out,
             vec4f *v1,
             vec4f *v2);

// Function Pointers: Mat4x4
typedef i32  (*PFN_Mat4UniBool)      (mat4f *v);
typedef f32  (*PFN_Mat4FltUni)       (mat4f *v);
typedef i32  (*PFN_Mat4BiBool)       (mat4f *v1,
                                      mat4f *v2);
typedef void (*PFN_Mat4Uni)          (mat4f *out,
                                      mat4f *v);
typedef void (*PFN_Mat4CRotate)      (mat4f *out,
                                      f32 v);
typedef void (*PFN_Mat4Bi)           (mat4f *out,
                                      mat4f *v1,
                                      mat4f *v2);
typedef void (*PFN_Mat4BiScalar)     (mat4f *out,
                                      mat4f *v1,
                                      float s);
typedef void (*PFN_Mat4MulVM)        (vec4f *out,
                                      vec4f *v,
                                      mat4f *m);
typedef void (*PFN_Mat4MulMV)        (vec4f *out,
                                      mat4f *m,
                                      vec4f *v);
typedef void (*PFN_Mat4Rotate)       (mat4f *out,
                                      f32 x,
                                      f32 y,
                                      f32 z);
typedef void (*PFN_Mat4Orthographic) (mat4f *out,
                                      f32 left, f32 top,
                                      f32 right, f32 bottom,
                                      f32 znear, f32 zfar);
typedef void (*PFN_Mat4Perspective) (mat4f *out,
                                     f32 aspect_ratio,
                                     f32 fovy,
                                     f32 znear,
                                     f32 zfar);

typedef f32  (*PFN_Mat4Determinant3)(mat4f *m);
local PFN_Mat4CRotate      m4f_init_xrotate;
local PFN_Mat4CRotate      m4f_init_yrotate;
local PFN_Mat4CRotate      m4f_init_zrotate;
local PFN_Mat4Rotate       m4f_init_rotate;

local PFN_Mat4UniBool      m4f_iszero;
local PFN_Mat4BiBool       m4f_iseq;
local PFN_Mat4Uni          m4f_abs;
local PFN_Mat4Bi           m4f_add;
local PFN_Mat4Bi           m4f_sub;
local PFN_Mat4Bi           m4f_mul;
local PFN_Mat4Bi           m4f_mulcm;
local PFN_Mat4Bi           m4f_divcm;
local PFN_Mat4BiScalar     m4f_mulsm;
local PFN_Mat4BiScalar     m4f_divsm;
local PFN_Mat4MulVM        m4f_mulvm;
local PFN_Mat4MulMV        m4f_mulmv;
local PFN_Mat4Uni          m4f_negate;
local PFN_Mat4Bi           m4f_min;
local PFN_Mat4Bi           m4f_max;

local PFN_Mat4FltUni       m4f_determinant;
local PFN_Mat4Uni          m4f_inverse;
local PFN_Mat4Orthographic m4f_init_orthographic;
local PFN_Mat4Perspective  m4f_init_perspective;
//////////////////////////////////////
// Implementation: Vector 4 (vec4f) //
//////////////////////////////////////
#define v4f_set_comp(out, xval, yval, zval, wval) \
	(out).x = (xval); \
	(out).y = (yval); \
	(out).z = (zval); \
	(out).w = (wval)

#define v4f_set_v4f(out, v) \
	(out).x = (v).x; \
	(out).y = (v).y; \
	(out).z = (v).z; \
	(out).w = (v).w

#define v4f_set_pv4f(out, v) \
	(out).x = (v)->x; \
	(out).y = (v)->y; \
	(out).z = (v)->z; \
	(out).w = (v)->w

#define pv4f_set_comp(out, xval, yval, zval, wval) \
	(out)->x = (xval); \
	(out)->y = (yval); \
	(out)->z = (zval); \
	(out)->w = (wval)

#define pv4f_set_v4f(out, v) \
	(out)->x = (v).x; \
	(out)->y = (v).y; \
	(out)->z = (v).z; \
	(out)->w = (v).w

#define pv4f_set_pv4f(out, v) \
	(out)->x = (v)->x; \
	(out)->y = (v)->y; \
	(out)->z = (v)->z; \
	(out)->w = (v)->w

#define v4f_set_zero(out) v4f_set_comp(out, 0.0f, 0.0f, 0.0f, 0.0f)
#define v4f_set_one(out)  v4f_set_comp(out, 1.0f, 1.0f, 1.0f, 1.0f)
#define v4f_set_min(out) \
	v4f_set_comp(out, F32_MIN, F32_MIN, F32_MIN, F32_MIN)
#define v4f_set_max(out) \
	v4f_set_comp(out, F32_MAX, F32_MAX, F32_MAX, F32_MAX)

local int
v4fb_iszero(vec4f *v)
{
	return ((v->x < F32_EPSILON) && (v->y < F32_EPSILON)) &&
	       ((v->z < F32_EPSILON) && (v->w < F32_EPSILON));
}

local int
v4fb_iseq(vec4f *v1,
          vec4f *v2)
{
	vec4f av1;
	vec4f av2;
	vec4f vdiff;
	vec4f rdiv;
	vec4f vmax;

	v4f_set_max(vmax);

	// Exact Equality
	if ((v1->x == v2->x && v1->y == v2->y) &&
		(v1->z == v2->z && v1->w == v2->w))
	{
		return 1;
	}

	// Near Zero
	v4f_abs(&av1, v1);
	v4f_abs(&av2, v2);
	v4f_sub(&vdiff, &av1, &av2);
	v4f_abs(&vdiff, &vdiff);

	if (((vdiff.x < F32_EPSILON) && (vdiff.y < F32_EPSILON)) &&
	    ((vdiff.z < F32_EPSILON) && (vdiff.w < F32_EPSILON)))
	{
		return 1;
	}

	// Relative Equality
	v4f_add(&rdiv, &av1, &av2);
	v4f_min(&rdiv, &rdiv, &vmax);
	v4f_divcv(&rdiv, &vdiff, &rdiv);
	if (((rdiv.x < F32_EPSILON) && (rdiv.y < F32_EPSILON)) &&
		((rdiv.z < F32_EPSILON) && (rdiv.w < F32_EPSILON)))
	{
		return 1;
	}

	return 0;
}

local void
v4fb_abs(vec4f *out,
         vec4f *v)
{
	out->bits[0] = v->bits[0] & 0x7FFFFFFF;
	out->bits[1] = v->bits[1] & 0x7FFFFFFF;
	out->bits[2] = v->bits[2] & 0x7FFFFFFF;
	out->bits[3] = v->bits[3] & 0x7FFFFFFF;
}

local void
v4fb_add(vec4f *out,
         vec4f *v1,
         vec4f *v2)
{
	out->data[0] = v1->data[0] + v2->data[0];
	out->data[1] = v1->data[1] + v2->data[1];
	out->data[2] = v1->data[2] + v2->data[2];
	out->data[3] = v1->data[3] + v2->data[3];
}

local void
v4fb_sub(vec4f *out,
         vec4f *v1,
         vec4f *v2)
{
	out->data[0] = v1->data[0] - v2->data[0];
	out->data[1] = v1->data[1] - v2->data[1];
	out->data[2] = v1->data[2] - v2->data[2];
	out->data[3] = v1->data[3] - v2->data[3];
}

local void
v4fb_mulcv(vec4f *out,
           vec4f *v1,
           vec4f *v2)
{
	out->data[0] = v1->data[0] * v2->data[0];
	out->data[1] = v1->data[1] * v2->data[1];
	out->data[2] = v1->data[2] * v2->data[2];
	out->data[3] = v1->data[3] * v2->data[3];
}

local void
v4fb_divcv(vec4f *out,
           vec4f *v1,
           vec4f *v2)
{
	out->data[0] = v1->data[0] / v2->data[0];
	out->data[1] = v1->data[1] / v2->data[1];
	out->data[2] = v1->data[2] / v2->data[2];
	out->data[3] = v1->data[3] / v2->data[3];
}

local void
v4fb_mulsv(vec4f *out,
           vec4f *v,
           f32 s)
{
	out->data[0] = v->data[0] * s;
	out->data[1] = v->data[1] * s;
	out->data[2] = v->data[2] * s;
	out->data[3] = v->data[3] * s;
}

local void
v4fb_divsv(vec4f *out,
           vec4f *v,
           f32 s)
{
	out->data[0] = v->data[0] / s;
	out->data[1] = v->data[1] / s;
	out->data[2] = v->data[2] / s;
	out->data[3] = v->data[3] / s;
}

local void
v4fb_negate(vec4f *out,
            vec4f *v)
{
	out->data[0] = -v->data[0];
	out->data[1] = -v->data[1];
	out->data[2] = -v->data[2];
	out->data[3] = -v->data[3];
}

local void
v4fb_min(vec4f *out,
         vec4f *v1,
         vec4f *v2)
{
	out->bits[0] = iminf(v1->bits[0], v2->bits[0], v1->data[0], v2->data[0]);
	out->bits[1] = iminf(v1->bits[1], v2->bits[1], v1->data[1], v2->data[1]);
	out->bits[2] = iminf(v1->bits[2], v2->bits[2], v1->data[2], v2->data[2]);
	out->bits[3] = iminf(v1->bits[3], v2->bits[3], v1->data[3], v2->data[3]);
}

local void
v4fb_max(vec4f *out,
         vec4f *v1,
         vec4f *v2)
{
	out->bits[0] = imaxf(v1->bits[0], v2->bits[0], v1->data[0], v2->data[0]);
	out->bits[1] = imaxf(v1->bits[1], v2->bits[1], v1->data[1], v2->data[1]);
	out->bits[2] = imaxf(v1->bits[2], v2->bits[2], v1->data[2], v2->data[2]);
	out->bits[3] = imaxf(v1->bits[3], v2->bits[3], v1->data[3], v2->data[3]);
}

local f32
v4fb_dot(vec4f *v1,
         vec4f *v2)
{
	return ((v1->x * v2->x) + (v1->y * v2->y)) +
	       ((v1->z * v2->z) + (v1->w * v2->w));
}

local f32
v4fb_lengthsq(vec4f *v)
{
	return ((v->x * v->x) + (v->y * v->y)) +
	       ((v->z * v->z) + (v->w * v->w));
}

local f32
v4fb_length(vec4f *v)
{
	return f32_sqrt(v4f_lengthsq(v));
}

local void
v4fb_cross3(vec4f *out,
            vec4f *v1,
            vec4f *v2)
{
	vec4f tmp;

	tmp.x = (v1->y * v2->z) - (v1->z * v2->y);
	tmp.y = (v1->z * v2->x) - (v1->x * v2->z);
	tmp.z = (v1->x * v2->y) - (v1->y * v2->x);

	out->x = tmp.x;
	out->y = tmp.y;
	out->z = tmp.z;
	out->w = 0.0f;
}

local void
v4fb_normalize(vec4f *out,
               vec4f *v)
{
	float lensq;
	float len;

	lensq = v4f_lengthsq(v);
	if (lensq == 0) {
		pv4f_set_pv4f(out, v);
		return;
	}

	len = f32_sqrt(lensq);
	out->x = v->x / len;
	out->y = v->y / len;
	out->z = v->z / len;
	out->w = v->w / len;
}

local void
v4fb_homogenize(vec4f *out,
                vec4f *v)
{
	if (f32_iszero(v->w)) {
		pv4f_set_pv4f(out, v);
		return;
	}

	out->x = v->x / v->w;
	out->y = v->y / v->w;
	out->z = v->z / v->w;
	out->w = 1.0f;
}

local f32
v4fb_anglebetween(vec4f *v1,
                  vec4f *v2)
{
	float lensq = v4f_lengthsq(v1) * v4f_lengthsq(v2);
	if (lensq == 0.0f)
		return 0.0f;

	return f32_acos(v4f_dot(v1, v2) / f32_sqrt(lensq));
}

local f32
v4fb_component(vec4f *v1,
               vec4f *v2)
{
	vec4f v2norm;

	v4f_normalize(&v2norm, v2);
	return v4f_dot(v1, &v2norm);
}

local void
v4fb_project(vec4f *out,
             vec4f *v1,
             vec4f *v2)
{
	vec4f v2norm;

	v4f_normalize(&v2norm, v2);
	v4f_mulsv(out, &v2norm, v4f_dot(v1, &v2norm));
}

local void
v4fb_reflect(vec4f *out,
             vec4f *v1,
             vec4f *v2)
{
	if (v4f_iszero(v2)) {
		pv4f_set_pv4f(out, v1);
		v4f_negate(out, out);
		return;
	}

	v4f_project(out, v1, v2);
	v4f_mulsv(out, out, -2.0f);
	v4f_add(out, out, v1);
	v4f_negate(out, out);
}

////////////////////////////////////////
// Implementation: Matrix 4x4 (mat4f) //
////////////////////////////////////////
#define m4f_set_comp(out, v00, v01, v02, v03, v10, v11, v12, v13, v20, v21, v22, v23, v30, v31, v32, v33) \
	(out).m00 = (v00); (out).m01 = (v01); (out).m02 = (v02); (out).m03 = (v03); \
	(out).m10 = (v10); (out).m11 = (v11); (out).m12 = (v12); (out).m13 = (v13); \
	(out).m20 = (v20); (out).m21 = (v21); (out).m22 = (v22); (out).m23 = (v23); \
	(out).m30 = (v30); (out).m31 = (v31); (out).m32 = (v32); (out).m33 = (v33)

#define m4f_set_m4f(out, m) \
	(out).m00 = (m).m00; (out).m01 = (m).m01; (out).m02 = (m).m02; (out).m03 = (m).m03; \
	(out).m10 = (m).m10; (out).m11 = (m).m11; (out).m12 = (m).m12; (out).m13 = (m).m13; \
	(out).m20 = (m).m20; (out).m21 = (m).m21; (out).m22 = (m).m22; (out).m23 = (m).m23; \
	(out).m30 = (m).m30; (out).m31 = (m).m31; (out).m32 = (m).m32; (out).m33 = (m).m33

#define m4f_set_pm4f(out, m) \
	(out).m00 = (m)->m00; (out).m01 = (m)->m01; (out).m02 = (m)->m02; (out).m03 = (m)->m03; \
	(out).m10 = (m)->m10; (out).m11 = (m)->m11; (out).m12 = (m)->m12; (out).m13 = (m)->m13; \
	(out).m20 = (m)->m20; (out).m21 = (m)->m21; (out).m22 = (m)->m22; (out).m23 = (m)->m23; \
	(out).m30 = (m)->m30; (out).m31 = (m)->m31; (out).m32 = (m)->m32; (out).m33 = (m)->m33

#define pm4f_set_comp(out, v00, v01, v02, v03, v10, v11, v12, v13, v20, v21, v22, v23, v30, v31, v32, v33) \
	(out)->m00 = (v00); (out)->m01 = (v01); (out)->m02 = (v02); (out)->m03 = (v03); \
	(out)->m10 = (v10); (out)->m11 = (v11); (out)->m12 = (v12); (out)->m13 = (v13); \
	(out)->m20 = (v20); (out)->m21 = (v21); (out)->m22 = (v22); (out)->m23 = (v23); \
	(out)->m30 = (v30); (out)->m31 = (v31); (out)->m32 = (v32); (out)->m33 = (v33)

#define pm4f_set_m4f(out, m) \
	(out)->m00 = (m).m00; (out)->m01 = (m).m01; (out)->m02 = (m).m02; (out)->m03 = (m).m03; \
	(out)->m10 = (m).m10; (out)->m11 = (m).m11; (out)->m12 = (m).m12; (out)->m13 = (m).m13; \
	(out)->m20 = (m).m20; (out)->m21 = (m).m21; (out)->m22 = (m).m22; (out)->m23 = (m).m23; \
	(out)->m30 = (m).m30; (out)->m31 = (m).m31; (out)->m32 = (m).m32; (out)->m33 = (m).m33

#define pm4f_set_pm4f(out, m) \
	(out)->m00 = (m)->m00; (out)->m01 = (m)->m01; (out)->m02 = (m)->m02; (out)->m03 = (m)->m03; \
	(out)->m10 = (m)->m10; (out)->m11 = (m)->m11; (out)->m12 = (m)->m12; (out)->m13 = (m)->m13; \
	(out)->m20 = (m)->m20; (out)->m21 = (m)->m21; (out)->m22 = (m)->m22; (out)->m23 = (m)->m23; \
	(out)->m30 = (m)->m30; (out)->m31 = (m)->m31; (out)->m32 = (m)->m32; (out)->m33 = (m)->m33

#define m4f_transpose_comp(out, v00, v01, v02, v03, v10, v11, v12, v13, v20, v21, v22, v23, v30, v31, v32, v33) \
	(out).m00 = (v00); (out).m01 = (v10); (out).m02 = (v20); (out).m03 = (v30); \
	(out).m10 = (v01); (out).m11 = (v11); (out).m12 = (v21); (out).m13 = (v31); \
	(out).m20 = (v02); (out).m21 = (v12); (out).m22 = (v22); (out).m23 = (v32); \
	(out).m30 = (v03); (out).m31 = (v13); (out).m32 = (v23); (out).m33 = (v33)

#define m4f_transpose_m4f(out, m) \
	(out).m00 = (m).m00; (out).m01 = (m).m10; (out).m02 = (m).m20; (out).m03 = (m).m30; \
	(out).m10 = (m).m01; (out).m11 = (m).m11; (out).m12 = (m).m21; (out).m13 = (m).m31; \
	(out).m20 = (m).m02; (out).m21 = (m).m12; (out).m22 = (m).m22; (out).m23 = (m).m32; \
	(out).m30 = (m).m03; (out).m31 = (m).m13; (out).m32 = (m).m23; (out).m33 = (m).m33

#define pm4f_transpose_comp(out, v00, v01, v02, v03, v10, v11, v12, v13, v20, v21, v22, v23, v30, v31, v32, v33) \
	(out)->m00 = (v00); (out)->m01 = (v10); (out)->m02 = (v20); (out)->m03 = (v30); \
	(out)->m10 = (v01); (out)->m11 = (v11); (out)->m12 = (v21); (out)->m13 = (v31); \
	(out)->m20 = (v02); (out)->m21 = (v12); (out)->m22 = (v22); (out)->m23 = (v32); \
	(out)->m30 = (v03); (out)->m31 = (v13); (out)->m32 = (v23); (out)->m33 = (v33)

#define pm4f_transpose_m4f(out, m) \
	(out)->m00 = (m).m00; (out)->m01 = (m).m10; (out)->m02 = (m).m20; (out)->m03 = (m).m30; \
	(out)->m10 = (m).m01; (out)->m11 = (m).m11; (out)->m12 = (m).m21; (out)->m13 = (m).m31; \
	(out)->m20 = (m).m02; (out)->m21 = (m).m12; (out)->m22 = (m).m22; (out)->m23 = (m).m32; \
	(out)->m30 = (m).m03; (out)->m31 = (m).m13; (out)->m32 = (m).m23; (out)->m33 = (m).m33

#define pm4f_transpose_pm4f(out, m) \
	(out)->m00 = (m)->m00; (out)->m01 = (m)->m10; (out)->m02 = (m)->m20; (out)->m03 = (m)->m30; \
	(out)->m10 = (m)->m01; (out)->m11 = (m)->m11; (out)->m12 = (m)->m21; (out)->m13 = (m)->m31; \
	(out)->m20 = (m)->m02; (out)->m21 = (m)->m12; (out)->m22 = (m)->m22; (out)->m23 = (m)->m32; \
	(out)->m30 = (m)->m03; (out)->m31 = (m)->m13; (out)->m32 = (m)->m23; (out)->m33 = (m)->m33

#define m4f_set_diag(out, s, g) \
	(out).m00 =  (s); (out).m01 = 0.0f; (out).m02 = 0.0f; (out).m03 = 0.0f; \
	(out).m10 = 0.0f; (out).m11 =  (s); (out).m12 = 0.0f; (out).m13 = 0.0f; \
	(out).m20 = 0.0f; (out).m21 = 0.0f; (out).m22 =  (s); (out).m23 = 0.0f; \
	(out).m30 = 0.0f; (out).m31 = 0.0f; (out).m32 = 0.0f; (out).m33 =  (g)
#define m4f_set_zero(out)        m4f_set_diag(out, 0.0f, 0.0f)
#define m4f_set_identity(out)    m4f_set_diag(out, 1.0f, 1.0f)
#define m4f_set_scale(out, s)    m4f_set_diag(out,    s, 1.0f)

#define pm4f_set_diag(out, s, g) \
	(out)->m10 = 0.0f; (out)->m11 =  (s); (out)->m12 = 0.0f; (out)->m13 = 0.0f; \
	(out)->m00 =  (s); (out)->m01 = 0.0f; (out)->m02 = 0.0f; (out)->m03 = 0.0f; \
	(out)->m20 = 0.0f; (out)->m21 = 0.0f; (out)->m22 =  (s); (out)->m23 = 0.0f; \
	(out)->m30 = 0.0f; (out)->m31 = 0.0f; (out)->m32 = 0.0f; (out)->m33 =  (g)
#define pm4f_set_zero(out)        pm4f_set_diag(out, 0.0f, 0.0f)
#define pm4f_set_identity(out)    pm4f_set_diag(out, 1.0f, 1.0f)
#define pm4f_set_scale(out, s)    pm4f_set_diag(out,    s, 1.0f)

#define m4f_set_min(out) \
	m4f_set_comp(out, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN, F32_MIN)
#define m4f_set_max(out) \
	m4f_set_comp(out, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX, F32_MAX)

#define m4f_set_translation(out, xval, yval, zval) \
	(out).m00 =   1.0f; (out).m01 =   0.0f; (out).m02 =   0.0f; (out).m03 = 0.0f; \
	(out).m10 =   0.0f; (out).m11 =   1.0f; (out).m12 =   0.0f; (out).m13 = 0.0f; \
	(out).m20 =   0.0f; (out).m21 =   0.0f; (out).m22 =   1.0f; (out).m23 = 0.0f; \
	(out).m30 = (xval); (out).m31 = (yval); (out).m32 = (zval); (out).m33 = 1.0f

#define m4f_set_shear(xy, xz, yx, yz, zx, zy) \
	(out).m00 = 1.0f; (out).m01 = (xy); (out).m02 = (xz); (out).m03 = 0.0f; \
	(out).m10 = (yx); (out).m11 = 1.0f; (out).m12 = (yz); (out).m13 = 0.0f; \
	(out).m20 = (zx); (out).m21 = (zy); (out).m22 = 1.0f; (out).m23 = 0.0f; \
	(out).m30 = 0.0f; (out).m31 = 0.0f; (out).m32 = 0.0f; (out).m33 = 1.0f

local void
m4fb_init_orthographic(mat4f *out,
                       f32 left,  f32 top,
                       f32 right, f32 bottom,
                       f32 znear, f32 zfar)
{
	out->m00 = 2.0f / (right - left);
	out->m01 = 0.0f;
	out->m02 = 0.0f;
	out->m03 = 0.0f;

	out->m10 = 0.0f;
	out->m11 = 2.0f / (top - bottom);
	out->m12 = 0.0f;
	out->m13 = 0.0f;

	out->m20 = 0.0f;
	out->m21 = 0.0f;
	out->m22 = 1.0f / (zfar - znear);
	out->m23 = 0.0f;

	out->m30 = (left + right) / (left - right);
	out->m31 = (top + bottom) / (bottom - top);
	out->m32 = -znear / (zfar - znear);
	out->m33 = 1.0f;
}

local void
m4fb_init_perspective(mat4f *out,
                      f32 aspect_ratio,
                      f32 fovy,
                      f32 znear,
                      f32 zfar)
{
	float yscale = f32_cot(fovy * 0.5f);
	float xscale = yscale / aspect_ratio;

	out->m00 = xscale;
	out->m01 = 0.0f;
	out->m02 = 0.0f;
	out->m03 = 0.0f;

	out->m10 = 0.0f;
	out->m11 = yscale;
	out->m12 = 0.0f;
	out->m13 = 0.0f;

	out->m20 = 0.0f;
	out->m21 = 0.0f;
	out->m22 = zfar / (zfar - znear);
	out->m23 = 1.0f;


	out->m30 = 0.0f;
	out->m31 = 0.0f;
	out->m32 = ((-znear) * zfar) / (zfar - znear);
	out->m33 = 0.0f;
}

local void
m4fb_init_xrotate(mat4f *out,
                  float rad)
{
	float sinval;
	float cosval;

	sinval = f32_sin(rad);
	cosval = f32_cos(rad);

	out->m00 = 1.0f;
	out->m01 = 0.0f;
	out->m02 = 0.0f;
	out->m03 = 0.0f;

	out->m10 = 0.0f;
	out->m11 =  cosval;
	out->m12 = -sinval;
	out->m13 = 0.0f;

	out->m20 = 0.0f;
	out->m21 = sinval;
	out->m22 = cosval;
	out->m23 = 0.0f;

	out->m30 = 0.0f;
	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
}

local void
m4fb_init_yrotate(mat4f *out,
                  float rad)
{
	float sinval;
	float cosval;

	sinval = f32_sin(rad);
	cosval = f32_cos(rad);

	out->m00 = cosval;
	out->m01 = 0.0f;
	out->m02 = sinval;
	out->m03 = 0.0f;

	out->m10 = 0.0f;
	out->m11 = 1.0f;
	out->m12 = 0.0f;
	out->m13 = 0.0f;

	out->m20 = -sinval;
	out->m21 = 0.0f;
	out->m22 =  cosval;
	out->m23 = 0.0f;

	out->m30 = 0.0f;
	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
}

local void
m4fb_init_zrotate(mat4f *out,
                  float rad)
{
	float sinval;
	float cosval;

	sinval = f32_sin(rad);
	cosval = f32_cos(rad);

	out->m00 =  cosval;
	out->m01 = -sinval;
	out->m02 = 0.0f;
	out->m03 = 0.0f;

	out->m10 = sinval;
	out->m11 = cosval;
	out->m12 = 0.0f;
	out->m13 = 0.0f;

	out->m20 = 0.0f;
	out->m21 = 0.0f;
	out->m22 = 1.0f;
	out->m23 = 0.0f;

	out->m30 = 0.0f;
	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
}

local void
m4fb_init_rotate(mat4f *out,
                 float xrad,
                 float yrad,
                 float zrad)
{
	float sx, cx;
	float sy, cy;
	float sz, cz;

	sx = f32_sin(xrad);
	sy = f32_sin(yrad);
	sz = f32_sin(zrad);
	cx = f32_cos(xrad);
	cy = f32_cos(yrad);
	cz = f32_cos(zrad);

	out->m00 = cy * cz;
	out->m01 = cy * sz;
	out->m02 = -sy;
	out->m03 = 0.0f;

	out->m10 = (sx * sy * cz) - (cx * sz);
	out->m11 = (sz * sy * sz) + (cx * cz);
	out->m12 = sx * cy;
	out->m13 = 0.0f;

	out->m20 = (cx * sy * cz) + (sx * sz);
	out->m21 = (cx * sy * sz) - (sx * cz);
	out->m22 = cx * cy;
	out->m23 = 0.0f;

	out->m30 = 0.0f;
	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
}

local int
m4fb_iszero(mat4f *m)
{
	return ((((m->m00 < F32_EPSILON) && (m->m01 < F32_EPSILON))   &&
	         ((m->m02 < F32_EPSILON) && (m->m03 < F32_EPSILON)))  &&
	        (((m->m10 < F32_EPSILON) && (m->m11 < F32_EPSILON))   &&
	         ((m->m12 < F32_EPSILON) && (m->m13 < F32_EPSILON)))) &&
	       ((((m->m20 < F32_EPSILON) && (m->m21 < F32_EPSILON))   &&
	         ((m->m22 < F32_EPSILON) && (m->m23 < F32_EPSILON)))  &&
	        (((m->m30 < F32_EPSILON) && (m->m31 < F32_EPSILON))   &&
	         ((m->m32 < F32_EPSILON) && (m->m33 < F32_EPSILON))));
}

local int
m4fb_iseq(mat4f *m1, mat4f *m2)
{
	mat4f am1;
	mat4f am2;
	mat4f mdiff;
	mat4f rdiv;
	mat4f mmax;

	m4f_set_max(mmax);

	// Exact Equality
	if (((((m1->m00 == m2->m00) && (m1->m01 == m2->m01))   &&
	      ((m1->m02 == m2->m02) && (m1->m03 == m2->m03)))  &&
	     (((m1->m10 == m2->m10) && (m1->m11 == m2->m11))   &&
	      ((m1->m12 == m2->m12) && (m1->m13 == m2->m13)))) &&
	    ((((m1->m20 == m2->m20) && (m1->m21 == m2->m21))   &&
	      ((m1->m22 == m2->m22) && (m1->m23 == m2->m23)))  &&
	     (((m1->m30 == m2->m30) && (m1->m31 == m2->m31))   &&
	      ((m1->m32 == m2->m32) && (m1->m33 == m2->m33)))))
	{
		return 1;
	}

	// Near Zero
	m4f_abs(&am1, m1);
	m4f_abs(&am2, m2);
	m4f_sub(&mdiff, &am1, &am2);
	m4f_abs(&mdiff, &mdiff);
	if (((((mdiff.m00 < F32_EPSILON) && (mdiff.m01 < F32_EPSILON))   &&
	      ((mdiff.m02 < F32_EPSILON) && (mdiff.m03 < F32_EPSILON)))  &&
	     (((mdiff.m10 < F32_EPSILON) && (mdiff.m11 < F32_EPSILON))   &&
	      ((mdiff.m12 < F32_EPSILON) && (mdiff.m13 < F32_EPSILON)))) &&
	    ((((mdiff.m20 < F32_EPSILON) && (mdiff.m21 < F32_EPSILON))   &&
	      ((mdiff.m22 < F32_EPSILON) && (mdiff.m23 < F32_EPSILON)))  &&
	     (((mdiff.m30 < F32_EPSILON) && (mdiff.m31 < F32_EPSILON))   &&
	      ((mdiff.m32 < F32_EPSILON) && (mdiff.m33 < F32_EPSILON)))))
	{
		return 1;
	}

	// Relative Equality
	m4f_add(&rdiv, &am1, &am2);
	m4f_min(&rdiv, &rdiv, &mmax);
	m4f_divcm(&rdiv, &mdiff, &rdiv);
	if (((((rdiv.m00 < F32_EPSILON) && (rdiv.m01 < F32_EPSILON))   &&
	      ((rdiv.m02 < F32_EPSILON) && (rdiv.m03 < F32_EPSILON)))  &&
	     (((rdiv.m10 < F32_EPSILON) && (rdiv.m11 < F32_EPSILON))   &&
	      ((rdiv.m12 < F32_EPSILON) && (rdiv.m13 < F32_EPSILON)))) &&
	    ((((rdiv.m20 < F32_EPSILON) && (rdiv.m21 < F32_EPSILON))   &&
	      ((rdiv.m22 < F32_EPSILON) && (rdiv.m23 < F32_EPSILON)))  &&
	     (((rdiv.m30 < F32_EPSILON) && (rdiv.m31 < F32_EPSILON))   &&
	      ((rdiv.m32 < F32_EPSILON) && (rdiv.m33 < F32_EPSILON)))))
	{
		return 1;
	}

	return 0;
}

local void
m4fb_abs(mat4f *out,
         mat4f *m)
{
	out->bits[0]  = m->bits[0]  & 0x7FFFFFFF;
	out->bits[1]  = m->bits[1]  & 0x7FFFFFFF;
	out->bits[2]  = m->bits[2]  & 0x7FFFFFFF;
	out->bits[3]  = m->bits[3]  & 0x7FFFFFFF;

	out->bits[4]  = m->bits[4]  & 0x7FFFFFFF;
	out->bits[5]  = m->bits[5]  & 0x7FFFFFFF;
	out->bits[6]  = m->bits[6]  & 0x7FFFFFFF;
	out->bits[7]  = m->bits[7]  & 0x7FFFFFFF;

	out->bits[8]  = m->bits[8]  & 0x7FFFFFFF;
	out->bits[9]  = m->bits[9]  & 0x7FFFFFFF;
	out->bits[10] = m->bits[10] & 0x7FFFFFFF;
	out->bits[11] = m->bits[11] & 0x7FFFFFFF;

	out->bits[12] = m->bits[12] & 0x7FFFFFFF;
	out->bits[13] = m->bits[13] & 0x7FFFFFFF;
	out->bits[14] = m->bits[14] & 0x7FFFFFFF;
	out->bits[15] = m->bits[15] & 0x7FFFFFFF;
}

local void
m4fb_add(mat4f *out,
         mat4f *m1,
         mat4f *m2)
{
	out->data[0]  = m1->data[0]  + m2->data[0];
	out->data[1]  = m1->data[1]  + m2->data[1];
	out->data[2]  = m1->data[2]  + m2->data[2];
	out->data[3]  = m1->data[3]  + m2->data[3];

	out->data[4]  = m1->data[4]  + m2->data[4];
	out->data[5]  = m1->data[5]  + m2->data[5];
	out->data[6]  = m1->data[6]  + m2->data[6];
	out->data[7]  = m1->data[7]  + m2->data[7];

	out->data[8]  = m1->data[8]  + m2->data[8];
	out->data[9]  = m1->data[9]  + m2->data[9];
	out->data[10] = m1->data[10] + m2->data[10];
	out->data[11] = m1->data[11] + m2->data[11];

	out->data[12] = m1->data[12] + m2->data[12];
	out->data[13] = m1->data[13] + m2->data[13];
	out->data[14] = m1->data[14] + m2->data[14];
	out->data[15] = m1->data[15] + m2->data[15];
}

local void
m4fb_sub(mat4f *out,
         mat4f *m1,
         mat4f *m2)
{
	out->data[0]  = m1->data[0]  - m2->data[0];
	out->data[1]  = m1->data[1]  - m2->data[1];
	out->data[2]  = m1->data[2]  - m2->data[2];
	out->data[3]  = m1->data[3]  - m2->data[3];

	out->data[4]  = m1->data[4]  - m2->data[4];
	out->data[5]  = m1->data[5]  - m2->data[5];
	out->data[6]  = m1->data[6]  - m2->data[6];
	out->data[7]  = m1->data[7]  - m2->data[7];

	out->data[8]  = m1->data[8]  - m2->data[8];
	out->data[9]  = m1->data[9]  - m2->data[9];
	out->data[10] = m1->data[10] - m2->data[10];
	out->data[11] = m1->data[11] - m2->data[11];

	out->data[12] = m1->data[12] - m2->data[12];
	out->data[13] = m1->data[13] - m2->data[13];
	out->data[14] = m1->data[14] - m2->data[14];
	out->data[15] = m1->data[15] - m2->data[15];
}

local void
m4fb_mul(mat4f *out,
         mat4f *m1,
         mat4f *m2)
{
	mat4f tmp;

	tmp.m00 = ((m1->m00 * m2->m00) + (m1->m01 * m2->m10)) +
	          ((m1->m02 * m2->m20) + (m1->m03 * m2->m30));

	tmp.m01 = ((m1->m00 * m2->m01) + (m1->m01 * m2->m11)) +
	          ((m1->m02 * m2->m21) + (m1->m03 * m2->m31));

	tmp.m02 = ((m1->m00 * m2->m02) + (m1->m01 * m2->m12)) +
	          ((m1->m02 * m2->m22) + (m1->m03 * m2->m32));

	tmp.m03 = ((m1->m00 * m2->m03) + (m1->m01 * m2->m13)) +
	          ((m1->m02 * m2->m23) + (m1->m03 * m2->m33));

	tmp.m10 = ((m1->m10 * m2->m00) + (m1->m11 * m2->m10)) +
	          ((m1->m12 * m2->m20) + (m1->m13 * m2->m30));

	tmp.m11 = ((m1->m10 * m2->m01) + (m1->m11 * m2->m11)) +
	          ((m1->m12 * m2->m21) + (m1->m13 * m2->m31));

	tmp.m12 = ((m1->m10 * m2->m02) + (m1->m11 * m2->m12)) +
	          ((m1->m12 * m2->m22) + (m1->m13 * m2->m32));

	tmp.m13 = ((m1->m10 * m2->m03) + (m1->m11 * m2->m13)) +
	          ((m1->m12 * m2->m23) + (m1->m13 * m2->m33));

	tmp.m20 = ((m1->m20 * m2->m00) + (m1->m21 * m2->m10)) +
	          ((m1->m22 * m2->m20) + (m1->m23 * m2->m30));

	tmp.m21 = ((m1->m20 * m2->m01) + (m1->m21 * m2->m11)) +
	          ((m1->m22 * m2->m21) + (m1->m23 * m2->m31));

	tmp.m22 = ((m1->m20 * m2->m02) + (m1->m21 * m2->m12)) +
	          ((m1->m22 * m2->m22) + (m1->m23 * m2->m32));

	tmp.m23 = ((m1->m20 * m2->m03) + (m1->m21 * m2->m13)) +
	          ((m1->m22 * m2->m23) + (m1->m23 * m2->m33));

	tmp.m30 = ((m1->m30 * m2->m00) + (m1->m31 * m2->m10)) +
	          ((m1->m32 * m2->m20) + (m1->m33 * m2->m30));

	tmp.m31 = ((m1->m30 * m2->m01) + (m1->m31 * m2->m11)) +
	          ((m1->m32 * m2->m21) + (m1->m33 * m2->m31));

	tmp.m32 = ((m1->m30 * m2->m02) + (m1->m31 * m2->m12)) +
	          ((m1->m32 * m2->m22) + (m1->m33 * m2->m32));

	tmp.m33 = ((m1->m30 * m2->m03) + (m1->m31 * m2->m13)) +
	          ((m1->m32 * m2->m23) + (m1->m33 * m2->m33));

	pm4f_set_m4f(out, tmp);
}

local void
m4fb_mulcm(mat4f *out,
           mat4f *m1,
           mat4f *m2)
{
	out->m00 = m1->m00 * m2->m00;
	out->m01 = m1->m01 * m2->m01;
	out->m02 = m1->m02 * m2->m02;
	out->m03 = m1->m03 * m2->m03;

	out->m10 = m1->m10 * m2->m10;
	out->m11 = m1->m11 * m2->m11;
	out->m12 = m1->m12 * m2->m12;
	out->m13 = m1->m13 * m2->m13;

	out->m20 = m1->m20 * m2->m20;
	out->m21 = m1->m21 * m2->m21;
	out->m22 = m1->m22 * m2->m22;
	out->m23 = m1->m23 * m2->m23;

	out->m30 = m1->m30 * m2->m30;
	out->m31 = m1->m31 * m2->m31;
	out->m32 = m1->m32 * m2->m32;
	out->m33 = m1->m33 * m2->m33;
}

local void
m4fb_mulsm(mat4f *out,
           mat4f *m,
           float s)
{
	out->m00 = m->m00 * s;
	out->m01 = m->m01 * s;
	out->m02 = m->m02 * s;
	out->m03 = m->m03 * s;

	out->m10 = m->m10 * s;
	out->m11 = m->m11 * s;
	out->m12 = m->m12 * s;
	out->m13 = m->m13 * s;

	out->m20 = m->m20 * s;
	out->m21 = m->m21 * s;
	out->m22 = m->m22 * s;
	out->m23 = m->m23 * s;

	out->m30 = m->m30 * s;
	out->m31 = m->m31 * s;
	out->m32 = m->m32 * s;
	out->m33 = m->m33 * s;
}

local void
m4fb_divcm(mat4f *out,
           mat4f *m1,
           mat4f *m2)
{
	out->m00 = m1->m00 / m2->m00;
	out->m01 = m1->m01 / m2->m01;
	out->m02 = m1->m02 / m2->m02;
	out->m03 = m1->m03 / m2->m03;

	out->m10 = m1->m10 / m2->m10;
	out->m11 = m1->m11 / m2->m11;
	out->m12 = m1->m12 / m2->m12;
	out->m13 = m1->m13 / m2->m13;

	out->m20 = m1->m20 / m2->m20;
	out->m21 = m1->m21 / m2->m21;
	out->m22 = m1->m22 / m2->m22;
	out->m23 = m1->m23 / m2->m23;

	out->m30 = m1->m30 / m2->m30;
	out->m31 = m1->m31 / m2->m31;
	out->m32 = m1->m32 / m2->m32;
	out->m33 = m1->m33 / m2->m33;
}

local void
m4fb_divsm(mat4f *out,
           mat4f *m,
           float s)
{
	out->m00 = m->m00 / s;
	out->m01 = m->m01 / s;
	out->m02 = m->m02 / s;
	out->m03 = m->m03 / s;

	out->m10 = m->m10 / s;
	out->m11 = m->m11 / s;
	out->m12 = m->m12 / s;
	out->m13 = m->m13 / s;

	out->m20 = m->m20 / s;
	out->m21 = m->m21 / s;
	out->m22 = m->m22 / s;
	out->m23 = m->m23 / s;

	out->m30 = m->m30 / s;
	out->m31 = m->m31 / s;
	out->m32 = m->m32 / s;
	out->m33 = m->m33 / s;
}

local void
m4fb_mulmv(vec4f *out,
           mat4f *m,
           vec4f *v)
{
	vec4f tmp;

	tmp.x = ((m->m00 * v->x) + (m->m01 * v->y)) +
            ((m->m02 * v->z) + (m->m03 * v->w));

	tmp.y = ((m->m10 * v->x) + (m->m11 * v->y)) +
            ((m->m12 * v->z) + (m->m13 * v->w));

	tmp.z = ((m->m20 * v->x) + (m->m21 * v->y)) +
            ((m->m22 * v->z) + (m->m23 * v->w));

	tmp.w = ((m->m30 * v->x) + (m->m31 * v->y)) +
            ((m->m32 * v->z) + (m->m33 * v->w));

	v4f_set_v4f(*out, tmp);
}

local void
m4fb_mulvm(vec4f *out,
           vec4f *v,
           mat4f *m)
{
	vec4f tmp;

	tmp.x = ((m->m00 * v->x) + (m->m10 * v->y)) +
            ((m->m20 * v->z) + (m->m30 * v->w));

	tmp.y = ((m->m01 * v->x) + (m->m11 * v->y)) +
            ((m->m21 * v->z) + (m->m31 * v->w));

	tmp.z = ((m->m02 * v->x) + (m->m12 * v->y)) +
            ((m->m22 * v->z) + (m->m32 * v->w));

	tmp.w = ((m->m03 * v->x) + (m->m13 * v->y)) +
            ((m->m23 * v->z) + (m->m33 * v->w));

	v4f_set_v4f(*out, tmp);
}

local void
m4fb_negate(mat4f *out,
            mat4f *m)
{
	out->m00 = -m->m00;
	out->m01 = -m->m01;
	out->m02 = -m->m02;
	out->m03 = -m->m03;

	out->m10 = -m->m10;
	out->m11 = -m->m11;
	out->m12 = -m->m12;
	out->m13 = -m->m13;

	out->m20 = -m->m20;
	out->m21 = -m->m21;
	out->m22 = -m->m22;
	out->m23 = -m->m23;

	out->m30 = -m->m30;
	out->m31 = -m->m31;
	out->m32 = -m->m32;
	out->m33 = -m->m33;
}

local void
m4fb_min(mat4f *out,
         mat4f *m1,
         mat4f *m2)
{
	out->bits[0]  = iminf(m1->bits[0],  m2->bits[0],  m1->data[0],  m2->data[0]);
	out->bits[1]  = iminf(m1->bits[1],  m2->bits[1],  m1->data[1],  m2->data[1]);
	out->bits[2]  = iminf(m1->bits[2],  m2->bits[2],  m1->data[2],  m2->data[2]);
	out->bits[3]  = iminf(m1->bits[3],  m2->bits[3],  m1->data[3],  m2->data[3]);

	out->bits[4]  = iminf(m1->bits[4],  m2->bits[4],  m1->data[4],  m2->data[4]);
	out->bits[5]  = iminf(m1->bits[5],  m2->bits[5],  m1->data[5],  m2->data[5]);
	out->bits[6]  = iminf(m1->bits[6],  m2->bits[6],  m1->data[6],  m2->data[6]);
	out->bits[7]  = iminf(m1->bits[7],  m2->bits[7],  m1->data[7],  m2->data[7]);

	out->bits[8]  = iminf(m1->bits[8],  m2->bits[8],  m1->data[8],  m2->data[8]);
	out->bits[9]  = iminf(m1->bits[9],  m2->bits[9],  m1->data[9],  m2->data[9]);
	out->bits[10] = iminf(m1->bits[10], m2->bits[10], m1->data[10], m2->data[10]);
	out->bits[11] = iminf(m1->bits[11], m2->bits[11], m1->data[11], m2->data[11]);

	out->bits[12] = iminf(m1->bits[12], m2->bits[12], m1->data[12], m2->data[12]);
	out->bits[13] = iminf(m1->bits[13], m2->bits[13], m1->data[13], m2->data[13]);
	out->bits[14] = iminf(m1->bits[14], m2->bits[14], m1->data[14], m2->data[14]);
	out->bits[15] = iminf(m1->bits[15], m2->bits[15], m1->data[15], m2->data[15]);
}

local void
m4fb_max(mat4f *out,
         mat4f *m1,
         mat4f *m2)
{
	out->bits[0]  = imaxf(m1->bits[0],  m2->bits[0],  m1->data[0],  m2->data[0]);
	out->bits[1]  = imaxf(m1->bits[1],  m2->bits[1],  m1->data[1],  m2->data[1]);
	out->bits[2]  = imaxf(m1->bits[2],  m2->bits[2],  m1->data[2],  m2->data[2]);
	out->bits[3]  = imaxf(m1->bits[3],  m2->bits[3],  m1->data[3],  m2->data[3]);

	out->bits[4]  = imaxf(m1->bits[4],  m2->bits[4],  m1->data[4],  m2->data[4]);
	out->bits[5]  = imaxf(m1->bits[5],  m2->bits[5],  m1->data[5],  m2->data[5]);
	out->bits[6]  = imaxf(m1->bits[6],  m2->bits[6],  m1->data[6],  m2->data[6]);
	out->bits[7]  = imaxf(m1->bits[7],  m2->bits[7],  m1->data[7],  m2->data[7]);

	out->bits[8]  = imaxf(m1->bits[8],  m2->bits[8],  m1->data[8],  m2->data[8]);
	out->bits[9]  = imaxf(m1->bits[9],  m2->bits[9],  m1->data[9],  m2->data[9]);
	out->bits[10] = imaxf(m1->bits[10], m2->bits[10], m1->data[10], m2->data[10]);
	out->bits[11] = imaxf(m1->bits[11], m2->bits[11], m1->data[11], m2->data[11]);

	out->bits[12] = imaxf(m1->bits[12], m2->bits[12], m1->data[12], m2->data[12]);
	out->bits[13] = imaxf(m1->bits[13], m2->bits[13], m1->data[13], m2->data[13]);
	out->bits[14] = imaxf(m1->bits[14], m2->bits[14], m1->data[14], m2->data[14]);
	out->bits[15] = imaxf(m1->bits[15], m2->bits[15], m1->data[15], m2->data[15]);
}


#define m4fb_determinant3(m00, m01, m02, m10, m11, m12, m20, m21, m22) \
   (((m00) * (((m11) * (m22)) - ((m21) * (m12)))) - \
    ((m01) * (((m10) * (m22)) - ((m20) * (m12)))) + \
	((m02) * (((m10) * (m21)) - ((m20) * (m11)))))

local f32
m4fb_determinant(mat4f *m)
{
	return ((m->m00 * m4fb_determinant3(m->m11, m->m12, m->m13,
	                                    m->m21, m->m22, m->m23,
	                                    m->m31, m->m32, m->m33))  -
	        (m->m01 * m4fb_determinant3(m->m10, m->m12, m->m13,
	                                    m->m20, m->m22, m->m23,
	                                    m->m30, m->m32, m->m33))) +
	       ((m->m02 * m4fb_determinant3(m->m10, m->m11, m->m13,
	                                    m->m20, m->m21, m->m23,
	                                    m->m30, m->m31, m->m33)) -
	        (m->m03 * m4fb_determinant3(m->m10, m->m11, m->m12,
	                                    m->m20, m->m21, m->m22,
	                                    m->m30, m->m31, m->m32)));
}

local void
m4fb_inverse(mat4f *out, mat4f *m)
{
	mat4f tmp;
	float det;

	det = m4f_determinant(m);
	if (f32_iszero(det)) {
		if (out == m) return;

		pm4f_set_pm4f(out, m);
		return;
	}

	// X
	tmp.m00 =  m4fb_determinant3(m->m11, m->m12, m->m13,
	                             m->m21, m->m22, m->m23,
	                             m->m31, m->m32, m->m33);

	tmp.m01 = -m4fb_determinant3(m->m01, m->m02, m->m03,
	                             m->m21, m->m22, m->m23,
	                             m->m31, m->m32, m->m33);

	tmp.m02 =  m4fb_determinant3(m->m01, m->m02, m->m03,
	                             m->m11, m->m12, m->m13,
	                             m->m31, m->m32, m->m33);

	tmp.m03 = -m4fb_determinant3(m->m01, m->m02, m->m03,
	                             m->m11, m->m12, m->m13,
	                             m->m21, m->m22, m->m23);

	// Y
	tmp.m10 = -m4fb_determinant3(m->m10, m->m12, m->m13,
	                             m->m20, m->m22, m->m23,
	                             m->m30, m->m32, m->m33);

	tmp.m11 =  m4fb_determinant3(m->m00, m->m02, m->m03,
	                             m->m20, m->m22, m->m23,
	                             m->m30, m->m32, m->m33);

	tmp.m12 = -m4fb_determinant3(m->m00, m->m02, m->m03,
	                             m->m10, m->m12, m->m13,
	                             m->m30, m->m32, m->m33);

	tmp.m13 =  m4fb_determinant3(m->m00, m->m02, m->m03,
	                             m->m10, m->m12, m->m13,
	                             m->m20, m->m22, m->m23);

	// Z
	tmp.m20 =  m4fb_determinant3(m->m10, m->m11, m->m13,
	                             m->m20, m->m21, m->m23,
	                             m->m30, m->m31, m->m33);

	tmp.m21 = -m4fb_determinant3(m->m00, m->m01, m->m03,
	                             m->m20, m->m21, m->m23,
	                             m->m30, m->m31, m->m33);

	tmp.m22 =  m4fb_determinant3(m->m00, m->m01, m->m03,
	                             m->m10, m->m11, m->m13,
	                             m->m30, m->m31, m->m33);

	tmp.m23 = -m4fb_determinant3(m->m00, m->m01, m->m03,
	                             m->m10, m->m11, m->m13,
	                             m->m20, m->m21, m->m23);

	// W
	tmp.m30 = -m4fb_determinant3(m->m10, m->m11, m->m12,
	                             m->m20, m->m21, m->m22,
	                             m->m30, m->m31, m->m32);

	tmp.m31 =  m4fb_determinant3(m->m00, m->m01, m->m02,
	                             m->m20, m->m21, m->m22,
	                             m->m30, m->m31, m->m32);

	tmp.m32 = -m4fb_determinant3(m->m00, m->m01, m->m02,
	                             m->m10, m->m11, m->m12,
	                             m->m30, m->m31, m->m32);

	tmp.m33 = m4fb_determinant3(m->m00, m->m01, m->m02,
	                            m->m10, m->m11, m->m12,
	                            m->m20, m->m21, m->m22);

	m4f_mulsm(out, &tmp, 1.0f / det);
}

/////////////////////
// Defaults: Vec4f //
/////////////////////
local PFN_Vec4UniBool      v4f_iszero            = v4fb_iszero;
local PFN_Vec4BiBool       v4f_iseq              = v4fb_iseq;
local PFN_Vec4Uni          v4f_abs               = v4fb_abs;
local PFN_Vec4Bi           v4f_add               = v4fb_add;
local PFN_Vec4Bi           v4f_sub               = v4fb_sub;
local PFN_Vec4Bi           v4f_mulcv             = v4fb_mulcv;
local PFN_Vec4Bi           v4f_divcv             = v4fb_divcv;
local PFN_Vec4BiScalar     v4f_mulsv             = v4fb_mulsv;
local PFN_Vec4BiScalar     v4f_divsv             = v4fb_divsv;
local PFN_Vec4Bi           v4f_min               = v4fb_min;
local PFN_Vec4Bi           v4f_max               = v4fb_max;
local PFN_Vec4FltBi        v4f_dot               = v4fb_dot;
local PFN_Vec4Bi           v4f_cross3            = v4fb_cross3;
local PFN_Vec4FltUni       v4f_length            = v4fb_length;
local PFN_Vec4FltUni       v4f_lengthsq          = v4fb_lengthsq;
local PFN_Vec4Uni          v4f_normalize         = v4fb_normalize;
local PFN_Vec4Uni          v4f_homogenize        = v4fb_homogenize;
local PFN_Vec4Uni          v4f_negate            = v4fb_negate;
local PFN_Vec4FltBi        v4f_anglebetween      = v4fb_anglebetween;
local PFN_Vec4FltBi        v4f_component         = v4fb_component;
local PFN_Vec4Bi           v4f_project           = v4fb_project;
local PFN_Vec4Bi           v4f_reflect           = v4fb_reflect;

/////////////////////
// Defaults: Mat4f //
/////////////////////
local PFN_Mat4CRotate      m4f_init_xrotate      = m4fb_init_xrotate;
local PFN_Mat4CRotate      m4f_init_yrotate      = m4fb_init_yrotate;
local PFN_Mat4CRotate      m4f_init_zrotate      = m4fb_init_zrotate;
local PFN_Mat4Rotate       m4f_init_rotate       = m4fb_init_rotate;

local PFN_Mat4UniBool      m4f_iszero            = m4fb_iszero;
local PFN_Mat4BiBool       m4f_iseq              = m4fb_iseq;
local PFN_Mat4Uni          m4f_abs               = m4fb_abs;
local PFN_Mat4Bi           m4f_add               = m4fb_add;
local PFN_Mat4Bi           m4f_sub               = m4fb_sub;
local PFN_Mat4Bi           m4f_mul               = m4fb_mul;
local PFN_Mat4Bi           m4f_mulcm             = m4fb_mulcm;
local PFN_Mat4Bi           m4f_divcm             = m4fb_divcm;
local PFN_Mat4BiScalar     m4f_mulsm             = m4fb_mulsm;
local PFN_Mat4BiScalar     m4f_divsm             = m4fb_divsm;
local PFN_Mat4MulVM        m4f_mulvm             = m4fb_mulvm;
local PFN_Mat4MulMV        m4f_mulmv             = m4fb_mulmv;
local PFN_Mat4Uni          m4f_negate            = m4fb_negate;
local PFN_Mat4Bi           m4f_min               = m4fb_min;
local PFN_Mat4Bi           m4f_max               = m4fb_max;

local PFN_Mat4FltUni       m4f_determinant       = m4fb_determinant;
local PFN_Mat4Uni          m4f_inverse           = m4fb_inverse;
local PFN_Mat4Orthographic m4f_init_orthographic = m4fb_init_orthographic;
local PFN_Mat4Perspective  m4f_init_perspective  = m4fb_init_perspective;

#endif // INCLUDE_NOSTD_COMMON_LINEAR_ALGEBRA_H

