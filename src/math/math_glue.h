#ifndef KE_HMM_EXT_H
#define KE_HMM_EXT_H

typedef HMM_Vec2 Vec2;
#define VEC2_CTOR(_x, _y) (HMM_V2((_x), (_y)))

typedef HMM_Vec3 Vec3;
#define VEC3_CTOR(_x, _y, _z) (HMM_V3((_x), (_y), (_z)))

typedef HMM_Vec4 Vec4;
#define VEC4_CTOR(_x, _y, _z, _w) (HMM_V4((_x), (_y), (_z), (_w)))
#define VEC4_V3_CTOR(_a, _w) (HMM_V4V((_a), (_w)))

typedef HMM_Mat4 Mat4;
#define MAT4_CTOR (HMM_M4D(1.0f))
#define MAT4_CTOR_ZERO (HMM_M4())

#define ZERO_V3 (VEC3_CTOR(0.0f, 0.0f, 0.0f))
#define UP_V3 (VEC3_CTOR(0.0f, 1.0f, 0.0f))
#define DOWN_V3 (VEC3_CTOR(0.0f, -1.0f, 0.0f))
#define LEFT_V3 (VEC3_CTOR(-1.0f, 0.0f, 0.0f))
#define RIGHT_V3 (VEC3_CTOR(-1.0f, 0.0f, 0.0f))
#define FORWARD_V3 (VEC3_CTOR(0.0f, 0.0f, 1.0f))
#define BACK_V3 (VEC3_CTOR(0.0f, 0.0f, -1.0f))

#define AXIS_X (RIGHT_V3)
#define AXIS_Y (UP_V3)
#define AXIS_Z (FORWARD_V3)

#define EQ(_a, _b) (HMM_Eq((_a), (_b)))
#define ADD(_a, _b) (HMM_Add((_a), (_b)))
#define SUB(_a, _b) (HMM_Sub((_a), (_b)))
#define MUL(_a, _b) (HMM_Mul((_a), (_b)))
#define DIV(_a, _b) (HMM_Div((_a), (_b)))

#define NORM_V3(_a) (HMM_NormV3((_a)))

#define ROT(_a, _b) (HMM_Rotate_LH(_a, _b))
#define TRANSLATE(_a) (HMM_Translate((_a)))

#define PERSPECTIVE(_fov, _ratio, _near, _far) \
	(bgfx_get_caps()->homogeneousDepth ? \
	HMM_Perspective_LH_NO((_fov), (_ratio), (_near), (_far)) : \
	HMM_Perspective_LH_ZO((_fov), (_ratio), (_near), (_far)))

#define RAD_TO_DEG (HMM_RadToDeg)
#define DEG_TO_RAD (HMM_DegToRad)

#endif // KE_HMM_EXT_H
