#ifndef KE_MATH_H
#define KE_MATH_H

#include "HandmadeMath.h"

// float
#define RAD_TO_DEG(_f) ((_f) * HMM_RadToDeg)
#define DEG_TO_RAD(_f) ((_f) * HMM_DegToRad)

typedef HMM_Vec2 Vector2;
#define V2_NEW(_x, _y) (HMM_V2((_x), (_y)))
#define V2_ZERO (V2_NEW(0.0f, 0.0f))

#define V2_NORM(_v2) (HMM_NormV2((_v2)))

typedef HMM_Vec3 Vector3;
#define V3_NEW(_x, _y, _z) (HMM_V3((_x), (_y), (_z)))
#define V3_ZERO (V3_NEW(0.0f, 0.0f, 0.0f))
#define V3_ONE (V3_NEW(0.0f, 0.0f, 0.0f))
#define V3_FROM_Q(_q) (V3_NEW((_q).X, (_q).Y, (_q).Z))

#define V3_UP (V3_NEW(0.0f, 1.0f, 0.0f))
#define V3_DOWN (V3_NEW(0.0f, -1.0f, 0.0f))
#define V3_LEFT (V3_NEW(-1.0f, 0.0f, 0.0f))
#define V3_RIGHT (V3_NEW(1.0f, 0.0f, 0.0f))
#define V3_FORWARD (V3_NEW(0.0f, 0.0f, 1.0f))
#define V3_BACK (V3_NEW(0.0f, 0.0f, -1.0f))

#define AXIS_X (V3_RIGHT)
#define AXIS_Y (V3_UP)
#define AXIS_Z (V3_FORWARD)

#define V3_NORM(_v3) (HMM_NormV3((_v3)))
#define V3_DOT(_v3_1, _v3_2) (HMM_DotV3((_v3_1), (_v3_2)))
#define V3_CROSS(_v3_1, _v3_2) (HMM_Cross((_v3_1), (_v3_2)))

typedef HMM_Vec4 Vector4;
#define V4_NEW(_x, _y, _z, _w) (HMM_V4((_x), (_y), (_z), (_w)))
#define V4_V3_NEW(_v3, _w) (HMM_V4V((_v3), (_w)))
#define V4_ZERO (HMM_V4(0.0f, 0.0f, 0.0f, 0.0f))

typedef HMM_Quat Quaternion;
#define Q_NEW(_x, _y, _z, _w) (HMM_Q((_x), (_y), (_z), (_w)))
#define Q_IDENTITY (Q_NEW(0.0f, 0.0f, 0.0f, 1.0f))
#define Q_V4_NEW(_v4) (HMM_QV4((_v4)))

#define Q_AA(_axis, _angle) (HMM_QFromAxisAngle_LH((_axis), (_angle)))

#define Q_DOT(_q_1, _q_2) (HMM_DotQ((_q_1), (_q_2)))
#define Q_INV(_q) (HMM_InvQ((_q)))
// in case of unit Q, the Q's conjugate (Q*) equals the inverse (Q^-1) of that Q
#define Q_CONJ(_q) (Q_NEW(-((_q).X), -((_q).Y), -((_q).Z), ((_q).W)))
#define Q_NORM(_q) (HMM_NormQ((_q)))
#define Q_NLERP(_q_1, _f, _q_2) (HMM_NLerp((_q_1), (_f), (_q_2)))
#define Q_SLERP(_q_1, _f, _q_2) (HMM_SLerp((_q_1), (_f), (_q_2)))

#define Q_TO_M4x4(_q) (HMM_QToM4((_q)))

typedef HMM_Mat4 Matrix4x4;
#define M4x4_IDENTITY (HMM_M4D(1.0f))
#define M4x4_ZERO (HMM_M4())
#define M4x4_POS(_x, _y, _z) {{ { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } , { 0.0f, 0.0f, 1.0f, 0.0f }, { (_x), (_y), (_z), 1.0f } }}
#define M4x4_POS_V3(_v3) {{ { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } , { 0.0f, 0.0f, 1.0f, 0.0f }, { (_v3).X, (_v3).Y, (_v3).Z, 1.0f } }}

#define M4x4_TO_Q(_m4x4) (HMM_M4ToQ_LH((_m4x4)))

#define ROT(_m4x4_1, _m4x4_2) (HMM_Rotate_LH((_m4x4_1), (_m4x4_2)))
#define TRANSLATE(_m4x4) (HMM_Translate((_m4x4)))
#define LOOK_AT(_v3_1, _v3_2) (HMM_LookAt_LH((_v3_1), (_v3_2), V3_UP))

#define PERSPECTIVE(_fov, _ratio, _near, _far) \
	(bgfx::getCaps()->homogeneousDepth ? \
	HMM_Perspective_LH_NO((_fov), (_ratio), (_near), (_far)) : \
	HMM_Perspective_LH_ZO((_fov), (_ratio), (_near), (_far)))

static const Vector3 globalForward = V3_FORWARD;
static const Vector3 globalUp = V3_UP;
static const Vector3 globalRight = V3_RIGHT;

static inline Vector3 rotate_v3_by_q(const Vector3& _v3, const Quaternion& _q) {
	Vector3 u = V3_NEW(_q.X, _q.Y, _q.Z);
    float s = _q.W;
	
	float dot = V3_DOT(u, _v3);
	Vector3 cross = V3_CROSS(u, _v3);
	
	return ((u * (2.0f * dot)) + (_v3 * (s * s - V3_DOT(u, u)))) + (cross * (2.0f * s));
}

#endif // KE_MATH_H
