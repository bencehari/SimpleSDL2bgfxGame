// it is a refactored version of
// "https://github.com/HandmadeMath/HandmadeMath"

/*
  HandmadeMath.h v2.0.0

  This is a single header file with a bunch of useful types and functions for
  games and graphics. Consider it a lightweight alternative to GLM that works
  both C and C++.

  =============================================================================
  CONFIG
  =============================================================================

  By default, all angles in Handmade Math are specified in radians. However, it
  can be configured to use degrees or turns instead. Use one of the following
  defines to specify the default unit for angles:

	#define HANDMADE_MATH_USE_RADIANS
	#define HANDMADE_MATH_USE_DEGREES
	#define HANDMADE_MATH_USE_TURNS

  Regardless of the default angle, you can use the following functions to
  specify an angle in a particular unit:

	HMM_AngleRad(radians)
	HMM_AngleDeg(degrees)
	HMM_AngleTurn(turns)

  The definitions of these functions change depending on the default unit.

  -----------------------------------------------------------------------------

  Handmade Math ships with SSE (SIMD) implementations of several common
  operations. To disable the use of SSE intrinsics, you must define
  HANDMADE_MATH_NO_SSE before including this file:

	#define HANDMADE_MATH_NO_SSE
	#include "HandmadeMath.h"

  -----------------------------------------------------------------------------

  To use Handmade Math without the C runtime library, you must provide your own
  implementations of basic math functions. Otherwise, HandmadeMath.h will use
  the runtime library implementation of these functions.

  Define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS and provide your own
  implementations of HMM_SINF, HMM_COSF, HMM_TANF, HMM_ACOSF, and HMM_SQRTF
  before including HandmadeMath.h, like so:

	#define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS
	#define HMM_SINF MySinF
	#define HMM_COSF MyCosF
	#define HMM_TANF MyTanF
	#define HMM_ACOSF MyACosF
	#define HMM_SQRTF MySqrtF
	#include "HandmadeMath.h"
  
  By default, it is assumed that your math functions take radians. To use
  different units, you must define HMM_ANGLE_USER_TO_INTERNAL and
  HMM_ANGLE_INTERNAL_TO_USER. For example, if you want to use degrees in your
  code but your math functions use turns:

	#define HMM_ANGLE_USER_TO_INTERNAL(a) ((a)*HMM_DegToTurn)
	#define HMM_ANGLE_INTERNAL_TO_USER(a) ((a)*HMM_TurnToDeg)

  =============================================================================
  
  LICENSE

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.

  =============================================================================

  CREDITS

  Originally written by Zakary Strange.

  Functionality:
   Zakary Strange (strangezak@protonmail.com && @strangezak)
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness)
   Trinton Bullard (@Peliex_Dev)
   @AntonDan
   Logan Forman (@dev_dwarf)

  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

// let's figure out if SSE is really available (unless disabled anyway)
// (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
// => only use "#ifdef HANDMADE_MATH__USE_SSE" to check for SSE support below this block!
#ifndef HANDMADE_MATH_NO_SSE
	#ifdef _MSC_VER /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
		#if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
			#define HANDMADE_MATH__USE_SSE 1
		#endif
	#else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
		#ifdef __SSE__ /* they #define __SSE__ if it's supported */
			#define HANDMADE_MATH__USE_SSE 1
		#endif /*  __SSE__ */
	#endif /* not _MSC_VER */
#endif /* #ifndef HANDMADE_MATH_NO_SSE */

#ifndef HANDMADE_MATH__USE_C11_GENERICS
	#if HANDMADE_MATH__USE_C11_GENERICS == 1
		#if (!defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
			#define HANDMADE_MATH__USE_C11_GENERICS 1
		#endif
	#endif
#endif

#ifdef HANDMADE_MATH__USE_SSE
	#include <xmmintrin.h>
#endif

#ifdef _MSC_VER
	#pragma warning(disable:4201)
#endif

#if defined(__GNUC__) || defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	#if (defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)) || defined(__clang__)
		#pragma GCC diagnostic ignored "-Wmissing-braces"
	#endif
	#ifdef __clang__
		#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
		#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
	#define HMM_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
	#define HMM_DEPRECATED(msg) __declspec(deprecated(msg))
#else
	#define HMM_DEPRECATED(msg)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HANDMADE_MATH_USE_DEGREES) && !defined(HANDMADE_MATH_USE_TURNS) && !defined(HANDMADE_MATH_USE_RADIANS)
	#define HANDMADE_MATH_USE_RADIANS
#endif
	
#define HMM_PI 3.14159265358979323846
#define HMM_PI32 3.14159265359f
#define HMM_DEG180 180.0
#define HMM_DEG18032 180.0f
#define HMM_TURNHALF 0.5
#define HMM_TURNHALF32 0.5f
#define HMM_RadToDeg ((float)(HMM_DEG180/HMM_PI))
#define HMM_RadToTurn ((float)(HMM_TURNHALF/HMM_PI))
#define HMM_DegToRad ((float)(HMM_PI/HMM_DEG180))
#define HMM_DegToTurn ((float)(HMM_TURNHALF/HMM_DEG180))
#define HMM_TurnToRad ((float)(HMM_PI/HMM_TURNHALF))
#define HMM_TurnToDeg ((float)(HMM_DEG180/HMM_TURNHALF))

#if defined(HANDMADE_MATH_USE_RADIANS)
	#define HMM_AngleRad(a) (a)
	#define HMM_AngleDeg(a) ((a)*HMM_DegToRad)
	#define HMM_AngleTurn(a) ((a)*HMM_TurnToRad)
#elif defined(HANDMADE_MATH_USE_DEGREES)
	#define HMM_AngleRad(a) ((a)*HMM_RadToDeg)
	#define HMM_AngleDeg(a) (a)
	#define HMM_AngleTurn(a) ((a)*HMM_TurnToDeg)
#elif defined(HANDMADE_MATH_USE_TURNS)
	#define HMM_AngleRad(a) ((a)*HMM_RadToTurn)
	#define HMM_AngleDeg(a) ((a)*HMM_DegToTurn)
	#define HMM_AngleTurn(a) (a)
#endif

#if !defined(HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS)
	#include <math.h>
	#define HMM_SINF sinf
	#define HMM_COSF cosf
	#define HMM_TANF tanf
	#define HMM_SQRTF sqrtf
	#define HMM_ACOSF acosf
#endif

#if !defined(HMM_ANGLE_USER_TO_INTERNAL)
	#define HMM_ANGLE_USER_TO_INTERNAL(a) (HMM_ToRad(a))
#endif

#if !defined(HMM_ANGLE_INTERNAL_TO_USER)
	#if defined(HANDMADE_MATH_USE_RADIANS)
		#define HMM_ANGLE_INTERNAL_TO_USER(a) (a) 
	#elif defined(HANDMADE_MATH_USE_DEGREES)
		#define HMM_ANGLE_INTERNAL_TO_USER(a) ((a)*HMM_RadToDeg)
	#elif defined(HANDMADE_MATH_USE_TURNS)
		#define HMM_ANGLE_INTERNAL_TO_USER(a) ((a)*HMM_RadToTurn)
	#endif
#endif

#define HMM_MIN(a, b) ((a) > (b) ? (b) : (a))
#define HMM_MAX(a, b) ((a) < (b) ? (b) : (a))
#define HMM_ABS(a) ((a) > 0 ? (a) : -(a))
#define HMM_MOD(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define HMM_SQUARE(x) ((x) * (x))

typedef union HMM_Vec2 {
	struct { float X, Y; };
	struct { float U, V; };
	struct { float Left, Right; };
	struct { float Width, Height; };

	float Elements[2];

#ifdef __cplusplus
	inline float& operator[](const int& Index) {
		return Elements[Index];
	}
#endif
} HMM_Vec2;

typedef union HMM_Vec3 {
	struct { float X, Y, Z; };
	struct { float U, V, W; };
	struct { float R, G, B; };

	struct {
		HMM_Vec2 XY;
		float _Ignored0;
	};
	struct {
		float _Ignored1;
		HMM_Vec2 YZ;
	};
	struct {
		HMM_Vec2 UV;
		float _Ignored2;
	};
	struct {
		float _Ignored3;
		HMM_Vec2 VW;
	};
	float Elements[3];

#ifdef __cplusplus
	inline float& operator[](const int& Index) {
		return Elements[Index];
	}
#endif
} HMM_Vec3;

typedef union HMM_Vec4 {
	struct {
		union {
			HMM_Vec3 XYZ;
			struct { float X, Y, Z; };
		};
		float W;
	};
	struct {
		union {
			HMM_Vec3 RGB;
			struct { float R, G, B; };
		};
		float A;
	};
	struct {
		HMM_Vec2 XY;
		float _Ignored0;
		float _Ignored1;
	};
	struct {
		float _Ignored2;
		HMM_Vec2 YZ;
		float _Ignored3;
	};
	struct {
		float _Ignored4;
		float _Ignored5;
		HMM_Vec2 ZW;
	};
	float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
	__m128 SSE;
#endif
#ifdef __cplusplus
	inline float& operator[](const int& Index) {
		return Elements[Index];
	}
#endif
} HMM_Vec4;

typedef union HMM_Mat2 {
	float Elements[2][2];
	HMM_Vec2 Columns[2];

#ifdef __cplusplus
	inline HMM_Vec2& operator[](const int& Index) {
		return Columns[Index];
	}
#endif
} HMM_Mat2;
	
typedef union HMM_Mat3 {
	float Elements[3][3];
	HMM_Vec3 Columns[3];

#ifdef __cplusplus
	inline HMM_Vec3& operator[](const int& Index) {
		return Columns[Index];
	}
#endif
} HMM_Mat3;

typedef union HMM_Mat4 {
	float Elements[4][4];
	HMM_Vec4 Columns[4];

#ifdef __cplusplus
	inline HMM_Vec4& operator[](const int& Index) {
		return Columns[Index];
	}
#endif
} HMM_Mat4;

typedef union HMM_Quat {
	struct {
		union {
			HMM_Vec3 XYZ;
			struct { float X, Y, Z; };
		};
		float W;
	};
	float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
	__m128 SSE;
#endif
} HMM_Quat;

typedef signed int HMM_Bool;

// Angle unit conversion functions

extern float HMM_ToRad(float Angle);
extern float HMM_ToDeg(float Angle);
extern float HMM_ToTurn(float Angle);

// Floating-point math functions

extern float HMM_SinF(float Angle);
extern float HMM_CosF(float Angle);
extern float HMM_TanF(float Angle);
extern float HMM_ACosF(float Arg);
extern float HMM_SqrtF(float Float);
extern float HMM_InvSqrtF(float Float);

// Utility functions

extern float HMM_Lerp(float A, float Time, float B);
extern float HMM_Clamp(float Min, float Value, float Max);

// Vector initialization

extern HMM_Vec2 HMM_V2(float X, float Y);
extern HMM_Vec3 HMM_V3(float X, float Y, float Z);
extern HMM_Vec4 HMM_V4(float X, float Y, float Z, float W);
extern HMM_Vec4 HMM_V4V(HMM_Vec3 Vector, float W);

// Binary vector operations

extern HMM_Vec2 HMM_AddV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 HMM_AddV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 HMM_AddV4(HMM_Vec4 Left, HMM_Vec4 Right);

extern HMM_Vec2 HMM_SubV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 HMM_SubV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 HMM_SubV4(HMM_Vec4 Left, HMM_Vec4 Right);

extern HMM_Vec2 HMM_MulV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec2 HMM_MulV2F(HMM_Vec2 Left, float Right);
extern HMM_Vec3 HMM_MulV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec3 HMM_MulV3F(HMM_Vec3 Left, float Right);
extern HMM_Vec4 HMM_MulV4(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec4 HMM_MulV4F(HMM_Vec4 Left, float Right);

extern HMM_Vec2 HMM_DivV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec2 HMM_DivV2F(HMM_Vec2 Left, float Right);
extern HMM_Vec3 HMM_DivV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec3 HMM_DivV3F(HMM_Vec3 Left, float Right);
extern HMM_Vec4 HMM_DivV4(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec4 HMM_DivV4F(HMM_Vec4 Left, float Right);

extern HMM_Bool HMM_EqV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Bool HMM_EqV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Bool HMM_EqV4(HMM_Vec4 Left, HMM_Vec4 Right);

extern float HMM_DotV2(HMM_Vec2 Left, HMM_Vec2 Right);
extern float HMM_DotV3(HMM_Vec3 Left, HMM_Vec3 Right);
extern float HMM_DotV4(HMM_Vec4 Left, HMM_Vec4 Right);

extern HMM_Vec3 HMM_Cross(HMM_Vec3 Left, HMM_Vec3 Right);

// Unary vector operations

extern float HMM_LenSqrV2(HMM_Vec2 A);
extern float HMM_LenSqrV3(HMM_Vec3 A);
extern float HMM_LenSqrV4(HMM_Vec4 A);

extern float HMM_LenV2(HMM_Vec2 A);
extern float HMM_LenV3(HMM_Vec3 A);
extern float HMM_LenV4(HMM_Vec4 A);

extern HMM_Vec2 HMM_NormV2(HMM_Vec2 A);
extern HMM_Vec3 HMM_NormV3(HMM_Vec3 A);
extern HMM_Vec4 HMM_NormV4(HMM_Vec4 A);

// Utility vector functions

extern HMM_Vec2 HMM_LerpV2(HMM_Vec2 A, float Time, HMM_Vec2 B);
extern HMM_Vec3 HMM_LerpV3(HMM_Vec3 A, float Time, HMM_Vec3 B);
extern HMM_Vec4 HMM_LerpV4(HMM_Vec4 A, float Time, HMM_Vec4 B);

// SSE stuff

extern HMM_Vec4 HMM_LinearCombineV4M4(HMM_Vec4 Left, HMM_Mat4 Right);

// 2x2 Matrices

extern HMM_Mat2 HMM_M2(void);
extern HMM_Mat2 HMM_M2D(float Diagonal);

extern HMM_Mat2 HMM_TransposeM2(HMM_Mat2 Matrix);

extern HMM_Mat2 HMM_AddM2(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat2 HMM_SubM2(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Vec2 HMM_MulM2V2(HMM_Mat2 Matrix, HMM_Vec2 Vector);
extern HMM_Mat2 HMM_MulM2(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat2 HMM_MulM2F(HMM_Mat2 Matrix, float Scalar);
extern HMM_Mat2 HMM_DivM2F(HMM_Mat2 Matrix, float Scalar);

extern float HMM_DeterminantM2(HMM_Mat2 Matrix);

extern HMM_Mat2 HMM_InvGeneralM2(HMM_Mat2 Matrix);

// 3x3 Matrices

extern HMM_Mat3 HMM_M3(void);
extern HMM_Mat3 HMM_M3D(float Diagonal);

extern HMM_Mat3 HMM_TransposeM3(HMM_Mat3 Matrix);

extern HMM_Mat3 HMM_AddM3(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat3 HMM_SubM3(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Vec3 HMM_MulM3V3(HMM_Mat3 Matrix, HMM_Vec3 Vector);
extern HMM_Mat3 HMM_MulM3(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat3 HMM_MulM3F(HMM_Mat3 Matrix, float Scalar);
extern HMM_Mat3 HMM_DivM3F(HMM_Mat3 Matrix, float Scalar);

extern float HMM_DeterminantM3(HMM_Mat3 Matrix);

extern HMM_Mat3 HMM_InvGeneralM3(HMM_Mat3 Matrix);

// 4x4 Matrices

extern HMM_Mat4 HMM_M4(void);
extern HMM_Mat4 HMM_M4D(float Diagonal);

extern HMM_Mat4 HMM_TransposeM4(HMM_Mat4 Matrix);

extern HMM_Mat4 HMM_AddM4(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Mat4 HMM_SubM4(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Mat4 HMM_MulM4(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Mat4 HMM_MulM4F(HMM_Mat4 Matrix, float Scalar);
extern HMM_Vec4 HMM_MulM4V4(HMM_Mat4 Matrix, HMM_Vec4 Vector);
extern HMM_Mat4 HMM_DivM4F(HMM_Mat4 Matrix, float Scalar);

extern float HMM_DeterminantM4(HMM_Mat4 Matrix);

// Returns a general-purpose inverse of an HMM_Mat4. Note that special-purpose inverses of many transformations
// are available and will be more efficient.
extern HMM_Mat4 HMM_InvGeneralM4(HMM_Mat4 Matrix);

// Common graphics transformations

// Produces a right-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
extern HMM_Mat4 HMM_Orthographic_RH_NO(float Left, float Right, float Bottom, float Top, float Near, float Far);
// Produces a right-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
extern HMM_Mat4 HMM_Orthographic_RH_ZO(float Left, float Right, float Bottom, float Top, float Near, float Far);
// Produces a left-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
extern HMM_Mat4 HMM_Orthographic_LH_NO(float Left, float Right, float Bottom, float Top, float Near, float Far);
// Produces a left-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
extern HMM_Mat4 HMM_Orthographic_LH_ZO(float Left, float Right, float Bottom, float Top, float Near, float Far);
// Returns an inverse for the given orthographic projection matrix. Works for all orthographic
// projection matrices, regardless of handedness or NDC convention.
extern HMM_Mat4 HMM_InvOrthographic(HMM_Mat4 OrthoMatrix);

extern HMM_Mat4 HMM_Perspective_RH_NO(float FOV, float AspectRatio, float Near, float Far);
extern HMM_Mat4 HMM_Perspective_RH_ZO(float FOV, float AspectRatio, float Near, float Far);
extern HMM_Mat4 HMM_Perspective_LH_NO(float FOV, float AspectRatio, float Near, float Far);
extern HMM_Mat4 HMM_Perspective_LH_ZO(float FOV, float AspectRatio, float Near, float Far);

extern HMM_Mat4 HMM_InvPerspective_RH(HMM_Mat4 PerspectiveMatrix);
extern HMM_Mat4 HMM_InvPerspective_LH(HMM_Mat4 PerspectiveMatrix);

extern HMM_Mat4 HMM_Translate(HMM_Vec3 Translation);
extern HMM_Mat4 HMM_InvTranslate(HMM_Mat4 TranslationMatrix);

extern HMM_Mat4 HMM_Rotate_RH(float Angle, HMM_Vec3 Axis);
extern HMM_Mat4 HMM_Rotate_LH(float Angle, HMM_Vec3 Axis);
extern HMM_Mat4 HMM_InvRotate(HMM_Mat4 RotationMatrix);
extern HMM_Mat4 HMM_Scale(HMM_Vec3 Scale);
extern HMM_Mat4 HMM_InvScale(HMM_Mat4 ScaleMatrix);

extern HMM_Mat4 _HMM_LookAt(HMM_Vec3 F,  HMM_Vec3 S, HMM_Vec3 U,  HMM_Vec3 Eye);
extern HMM_Mat4 HMM_LookAt_RH(HMM_Vec3 Eye, HMM_Vec3 Center, HMM_Vec3 Up);
extern HMM_Mat4 HMM_LookAt_LH(HMM_Vec3 Eye, HMM_Vec3 Center, HMM_Vec3 Up);
extern HMM_Mat4 HMM_InvLookAt(HMM_Mat4 Matrix);

// Quaternion operations

extern HMM_Quat HMM_Q(float X, float Y, float Z, float W);
extern HMM_Quat HMM_QV4(HMM_Vec4 Vector);

extern HMM_Quat HMM_AddQ(HMM_Quat Left, HMM_Quat Right);
extern HMM_Quat HMM_SubQ(HMM_Quat Left, HMM_Quat Right);
extern HMM_Quat HMM_MulQ(HMM_Quat Left, HMM_Quat Right);
extern HMM_Quat HMM_MulQF(HMM_Quat Left, float Multiplicative);
extern HMM_Quat HMM_DivQF(HMM_Quat Left, float Divnd);

extern float HMM_DotQ(HMM_Quat Left, HMM_Quat Right);
extern HMM_Quat HMM_InvQ(HMM_Quat Left);
extern HMM_Quat HMM_NormQ(HMM_Quat Quat);

extern HMM_Quat _HMM_MixQ(HMM_Quat Left, float MixLeft, HMM_Quat Right, float MixRight);

extern HMM_Quat HMM_NLerp(HMM_Quat Left, float Time, HMM_Quat Right);
extern HMM_Quat HMM_SLerp(HMM_Quat Left, float Time, HMM_Quat Right);

extern HMM_Mat4 HMM_QToM4(HMM_Quat Left);

extern HMM_Quat HMM_M4ToQ_RH(HMM_Mat4 M);
extern HMM_Quat HMM_M4ToQ_LH(HMM_Mat4 M);

extern HMM_Quat HMM_QFromAxisAngle_RH(HMM_Vec3 Axis, float AngleOfRotation);
extern HMM_Quat HMM_QFromAxisAngle_LH(HMM_Vec3 Axis, float AngleOfRotation);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

extern float HMM_Len(HMM_Vec2 A);
extern float HMM_Len(HMM_Vec3 A);
extern float HMM_Len(HMM_Vec4 A);
extern float HMM_LenSqr(HMM_Vec2 A);
extern float HMM_LenSqr(HMM_Vec3 A);
extern float HMM_LenSqr(HMM_Vec4 A);
extern HMM_Vec2 HMM_Norm(HMM_Vec2 A);
extern HMM_Vec3 HMM_Norm(HMM_Vec3 A);
extern HMM_Vec4 HMM_Norm(HMM_Vec4 A);
extern HMM_Quat HMM_Norm(HMM_Quat A);
extern float HMM_Dot(HMM_Vec2 Left, HMM_Vec2 VecTwo);
extern float HMM_Dot(HMM_Vec3 Left, HMM_Vec3 VecTwo);
extern float HMM_Dot(HMM_Vec4 Left, HMM_Vec4 VecTwo);
extern HMM_Vec2 HMM_Lerp(HMM_Vec2 Left, float Time, HMM_Vec2 Right);
extern HMM_Vec3 HMM_Lerp(HMM_Vec3 Left, float Time, HMM_Vec3 Right);
extern HMM_Vec4 HMM_Lerp(HMM_Vec4 Left, float Time, HMM_Vec4 Right);
extern HMM_Mat2 HMM_Transpose(HMM_Mat2 Matrix);
extern HMM_Mat3 HMM_Transpose(HMM_Mat3 Matrix);
extern HMM_Mat4 HMM_Transpose(HMM_Mat4 Matrix);
extern float HMM_Determinant(HMM_Mat2 Matrix);
extern float HMM_Determinant(HMM_Mat3 Matrix);
extern float HMM_Determinant(HMM_Mat4 Matrix);
extern HMM_Mat2 HMM_InvGeneral(HMM_Mat2 Matrix);
extern HMM_Mat3 HMM_InvGeneral(HMM_Mat3 Matrix);
extern HMM_Mat4 HMM_InvGeneral(HMM_Mat4 Matrix);
extern float HMM_Dot(HMM_Quat QuatOne, HMM_Quat QuatTwo);
extern HMM_Vec2 HMM_Add(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 HMM_Add(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 HMM_Add(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Mat2 HMM_Add(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 HMM_Add(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 HMM_Add(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Quat HMM_Add(HMM_Quat Left, HMM_Quat Right);
extern HMM_Vec2 HMM_Sub(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 HMM_Sub(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 HMM_Sub(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Mat2 HMM_Sub(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 HMM_Sub(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 HMM_Sub(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Quat HMM_Sub(HMM_Quat Left, HMM_Quat Right);
extern HMM_Vec2 HMM_Mul(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec2 HMM_Mul(HMM_Vec2 Left, float Right);
extern HMM_Vec3 HMM_Mul(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec3 HMM_Mul(HMM_Vec3 Left, float Right);
extern HMM_Vec4 HMM_Mul(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec4 HMM_Mul(HMM_Vec4 Left, float Right);
extern HMM_Mat2 HMM_Mul(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 HMM_Mul(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 HMM_Mul(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Mat2 HMM_Mul(HMM_Mat2 Left, float Right);
extern HMM_Mat3 HMM_Mul(HMM_Mat3 Left, float Right);
extern HMM_Mat4 HMM_Mul(HMM_Mat4 Left, float Right);
extern HMM_Vec2 HMM_Mul(HMM_Mat2 Matrix, HMM_Vec2 Vector);
extern HMM_Vec3 HMM_Mul(HMM_Mat3 Matrix, HMM_Vec3 Vector);
extern HMM_Vec4 HMM_Mul(HMM_Mat4 Matrix, HMM_Vec4 Vector);
extern HMM_Quat HMM_Mul(HMM_Quat Left, HMM_Quat Right);
extern HMM_Quat HMM_Mul(HMM_Quat Left, float Right);
extern HMM_Vec2 HMM_Div(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec2 HMM_Div(HMM_Vec2 Left, float Right);
extern HMM_Vec3 HMM_Div(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec3 HMM_Div(HMM_Vec3 Left, float Right);
extern HMM_Vec4 HMM_Div(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec4 HMM_Div(HMM_Vec4 Left, float Right);
extern HMM_Mat2 HMM_Div(HMM_Mat2 Left, float Right);
extern HMM_Mat3 HMM_Div(HMM_Mat3 Left, float Right);
extern HMM_Mat4 HMM_Div(HMM_Mat4 Left, float Right);
extern HMM_Quat HMM_Div(HMM_Quat Left, float Right);
extern HMM_Bool HMM_Eq(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Bool HMM_Eq(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Bool HMM_Eq(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec2 operator+(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 operator+(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 operator+(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Mat2 operator+(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 operator+(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 operator+(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Quat operator+(HMM_Quat Left, HMM_Quat Right);
extern HMM_Vec2 operator-(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 operator-(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 operator-(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Mat2 operator-(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 operator-(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 operator-(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Quat operator-(HMM_Quat Left, HMM_Quat Right);
extern HMM_Vec2 operator*(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 operator*(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 operator*(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Mat2 operator*(HMM_Mat2 Left, HMM_Mat2 Right);
extern HMM_Mat3 operator*(HMM_Mat3 Left, HMM_Mat3 Right);
extern HMM_Mat4 operator*(HMM_Mat4 Left, HMM_Mat4 Right);
extern HMM_Quat operator*(HMM_Quat Left, HMM_Quat Right);
extern HMM_Vec2 operator*(HMM_Vec2 Left, float Right);
extern HMM_Vec3 operator*(HMM_Vec3 Left, float Right);
extern HMM_Vec4 operator*(HMM_Vec4 Left, float Right);
extern HMM_Mat2 operator*(HMM_Mat2 Left, float Right);
extern HMM_Mat3 operator*(HMM_Mat3 Left, float Right);
extern HMM_Mat4 operator*(HMM_Mat4 Left, float Right);
extern HMM_Quat operator*(HMM_Quat Left, float Right);
extern HMM_Vec2 operator*(float Left, HMM_Vec2 Right);
extern HMM_Vec3 operator*(float Left, HMM_Vec3 Right);
extern HMM_Vec4 operator*(float Left, HMM_Vec4 Right);
extern HMM_Mat2 operator*(float Left, HMM_Mat2 Right);
extern HMM_Mat3 operator*(float Left, HMM_Mat3 Right);
extern HMM_Mat4 operator*(float Left, HMM_Mat4 Right);
extern HMM_Quat operator*(float Left, HMM_Quat Right);
extern HMM_Vec2 operator*(HMM_Mat2 Matrix, HMM_Vec2 Vector);
extern HMM_Vec3 operator*(HMM_Mat3 Matrix, HMM_Vec3 Vector);
extern HMM_Vec4 operator*(HMM_Mat4 Matrix, HMM_Vec4 Vector);
extern HMM_Vec2 operator/(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Vec3 operator/(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Vec4 operator/(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec2 operator/(HMM_Vec2 Left, float Right);
extern HMM_Vec3 operator/(HMM_Vec3 Left, float Right);
extern HMM_Vec4 operator/(HMM_Vec4 Left, float Right);
extern HMM_Mat4 operator/(HMM_Mat4 Left, float Right);
extern HMM_Mat3 operator/(HMM_Mat3 Left, float Right);
extern HMM_Mat2 operator/(HMM_Mat2 Left, float Right);
extern HMM_Quat operator/(HMM_Quat Left, float Right);
extern HMM_Vec2 &operator+=(HMM_Vec2 &Left, HMM_Vec2 Right);
extern HMM_Vec3 &operator+=(HMM_Vec3 &Left, HMM_Vec3 Right);
extern HMM_Vec4 &operator+=(HMM_Vec4 &Left, HMM_Vec4 Right);
extern HMM_Mat2 &operator+=(HMM_Mat2 &Left, HMM_Mat2 Right);
extern HMM_Mat3 &operator+=(HMM_Mat3 &Left, HMM_Mat3 Right);
extern HMM_Mat4 &operator+=(HMM_Mat4 &Left, HMM_Mat4 Right);
extern HMM_Quat &operator+=(HMM_Quat &Left, HMM_Quat Right);
extern HMM_Vec2 &operator-=(HMM_Vec2 &Left, HMM_Vec2 Right);
extern HMM_Vec3 &operator-=(HMM_Vec3 &Left, HMM_Vec3 Right);
extern HMM_Vec4 &operator-=(HMM_Vec4 &Left, HMM_Vec4 Right);
extern HMM_Mat2 &operator-=(HMM_Mat2 &Left, HMM_Mat2 Right);
extern HMM_Mat3 &operator-=(HMM_Mat3 &Left, HMM_Mat3 Right);
extern HMM_Mat4 &operator-=(HMM_Mat4 &Left, HMM_Mat4 Right);
extern HMM_Quat &operator-=(HMM_Quat &Left, HMM_Quat Right);
extern HMM_Vec2 &operator*=(HMM_Vec2 &Left, HMM_Vec2 Right);
extern HMM_Vec3 &operator*=(HMM_Vec3 &Left, HMM_Vec3 Right);
extern HMM_Vec4 &operator*=(HMM_Vec4 &Left, HMM_Vec4 Right);
extern HMM_Vec2 &operator*=(HMM_Vec2 &Left, float Right);
extern HMM_Vec3 &operator*=(HMM_Vec3 &Left, float Right);
extern HMM_Vec4 &operator*=(HMM_Vec4 &Left, float Right);
extern HMM_Mat2 &operator*=(HMM_Mat2 &Left, float Right);
extern HMM_Mat3 &operator*=(HMM_Mat3 &Left, float Right);
extern HMM_Mat4 &operator*=(HMM_Mat4 &Left, float Right);
extern HMM_Quat &operator*=(HMM_Quat &Left, float Right);
extern HMM_Vec2 &operator/=(HMM_Vec2 &Left, HMM_Vec2 Right);
extern HMM_Vec3 &operator/=(HMM_Vec3 &Left, HMM_Vec3 Right);
extern HMM_Vec4 &operator/=(HMM_Vec4 &Left, HMM_Vec4 Right);
extern HMM_Vec2 &operator/=(HMM_Vec2 &Left, float Right);
extern HMM_Vec3 &operator/=(HMM_Vec3 &Left, float Right);
extern HMM_Vec4 &operator/=(HMM_Vec4 &Left, float Right);
extern HMM_Mat4 &operator/=(HMM_Mat4 &Left, float Right);
extern HMM_Quat &operator/=(HMM_Quat &Left, float Right);
extern HMM_Bool operator==(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Bool operator==(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Bool operator==(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Bool operator!=(HMM_Vec2 Left, HMM_Vec2 Right);
extern HMM_Bool operator!=(HMM_Vec3 Left, HMM_Vec3 Right);
extern HMM_Bool operator!=(HMM_Vec4 Left, HMM_Vec4 Right);
extern HMM_Vec2 operator-(HMM_Vec2 In);
extern HMM_Vec3 operator-(HMM_Vec3 In);
extern HMM_Vec4 operator-(HMM_Vec4 In);

#endif /* __cplusplus*/

#ifdef HANDMADE_MATH__USE_C11_GENERICS
#if HANDMADE_MATH__USE_C11_GENERICS == 1

#define HMM_Add(A, B) _Generic((A), \
		HMM_Vec2: HMM_AddV2, \
		HMM_Vec3: HMM_AddV3, \
		HMM_Vec4: HMM_AddV4, \
		HMM_Mat2: HMM_AddM2, \
		HMM_Mat3: HMM_AddM3, \
		HMM_Mat4: HMM_AddM4, \
		HMM_Quat: HMM_AddQ \
)(A, B)

#define HMM_Sub(A, B) _Generic((A), \
		HMM_Vec2: HMM_SubV2, \
		HMM_Vec3: HMM_SubV3, \
		HMM_Vec4: HMM_SubV4, \
		HMM_Mat2: HMM_SubM2, \
		HMM_Mat3: HMM_SubM3, \
		HMM_Mat4: HMM_SubM4, \
		HMM_Quat: HMM_SubQ \
)(A, B)

#define HMM_Mul(A, B) _Generic((B), \
	 float: _Generic((A), \
		HMM_Vec2: HMM_MulV2F, \
		HMM_Vec3: HMM_MulV3F, \
		HMM_Vec4: HMM_MulV4F, \
		HMM_Mat2: HMM_MulM2F, \
		HMM_Mat3: HMM_MulM3F, \
		HMM_Mat4: HMM_MulM4F, \
		HMM_Quat: HMM_MulQF \
	 ), \
	 HMM_Mat2: HMM_MulM2, \
	 HMM_Mat3: HMM_MulM3, \
	 HMM_Mat4: HMM_MulM4, \
	 HMM_Quat: HMM_MulQ, \
	 default: _Generic((A), \
		HMM_Vec2: HMM_MulV2, \
		HMM_Vec3: HMM_MulV3, \
		HMM_Vec4: HMM_MulV4, \
		HMM_Mat2: HMM_MulM2V2, \
		HMM_Mat3: HMM_MulM3V3, \
		HMM_Mat4: HMM_MulM4V4 \
	) \
)(A, B)

#define HMM_Div(A, B) _Generic((B), \
	 float: _Generic((A), \
		HMM_Mat2: HMM_DivM2F, \
		HMM_Mat3: HMM_DivM3F, \
		HMM_Mat4: HMM_DivM4F, \
		HMM_Vec2: HMM_DivV2F, \
		HMM_Vec3: HMM_DivV3F, \
		HMM_Vec4: HMM_DivV4F, \
		HMM_Quat: HMM_DivQF  \
	 ), \
	 HMM_Mat2: HMM_DivM2, \
	 HMM_Mat3: HMM_DivM3, \
	 HMM_Mat4: HMM_DivM4, \
	 HMM_Quat: HMM_DivQ, \
	 default: _Generic((A), \
		HMM_Vec2: HMM_DivV2, \
		HMM_Vec3: HMM_DivV3, \
		HMM_Vec4: HMM_DivV4  \
	) \
)(A, B)

#define HMM_Len(A) _Generic((A), \
		HMM_Vec2: HMM_LenV2, \
		HMM_Vec3: HMM_LenV3, \
		HMM_Vec4: HMM_LenV4  \
)(A)

#define HMM_LenSqr(A) _Generic((A), \
		HMM_Vec2: HMM_LenSqrV2, \
		HMM_Vec3: HMM_LenSqrV3, \
		HMM_Vec4: HMM_LenSqrV4  \
)(A)

#define HMM_Norm(A) _Generic((A), \
		HMM_Vec2: HMM_NormV2, \
		HMM_Vec3: HMM_NormV3, \
		HMM_Vec4: HMM_NormV4  \
)(A)

#define HMM_Dot(A, B) _Generic((A), \
		HMM_Vec2: HMM_DotV2, \
		HMM_Vec3: HMM_DotV3, \
		HMM_Vec4: HMM_DotV4  \
)(A, B)

#define HMM_Lerp(A, T, B) _Generic((A), \
		float: HMM_Lerp, \
		HMM_Vec2: HMM_LerpV2, \
		HMM_Vec3: HMM_LerpV3, \
		HMM_Vec4: HMM_LerpV4 \
)(A, T, B)

#define HMM_Eq(A, B) _Generic((A), \
		HMM_Vec2: HMM_EqV2, \
		HMM_Vec3: HMM_EqV3, \
		HMM_Vec4: HMM_EqV4  \
)(A, B)

#define HMM_Transpose(M) _Generic((M), \
		HMM_Mat2: HMM_TransposeM2, \
		HMM_Mat3: HMM_TransposeM3, \
		HMM_Mat4: HMM_TransposeM4  \
)(M)

#define HMM_Determinant(M) _Generic((M), \
		HMM_Mat2: HMM_DeterminantM2, \
		HMM_Mat3: HMM_DeterminantM3, \
		HMM_Mat4: HMM_DeterminantM4  \
)(M)

#define HMM_InvGeneral(M) _Generic((M), \
		HMM_Mat2: HMM_InvGeneralM2, \
		HMM_Mat3: HMM_InvGeneralM3, \
		HMM_Mat4: HMM_InvGeneralM4  \
)(M)

#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif // HANDMADE_MATH_H
