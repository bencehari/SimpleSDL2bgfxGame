#ifndef BX_MATH
#define BX_MATH

#include <stdbool.h>

// C implementation of math.cpp in bx. Only what is needed for the actual task.
// handedness removed altogether and lefty used instead.

struct Vec3 {
	float x, y, z;
};

struct Vec2 {
	float x, y;
};

// Vector3
extern const struct Vec3 vec3Zero;
extern const struct Vec3 vec3GlobalUp;
extern const struct Vec3 vec3GlobalDown;
extern const struct Vec3 vec3GlobalLeft;
extern const struct Vec3 vec3GlobalRight;
extern const struct Vec3 vec3GlobalForward;
extern const struct Vec3 vec3GlobalBack;

// Vector3
extern struct Vec3 v3_ctor(float _x, float _y, float _z);
extern bool v3_equals(const struct Vec3 _a, const struct Vec3 _b);
extern void v3_print(struct Vec3* _v);

// Vector3
extern struct Vec3 v3_add_v3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 v3_add_f(const struct Vec3 _a, const float _b);
extern struct Vec3 v3_sub_v3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 v3_sub_f(const struct Vec3 _a, const float _b);
extern struct Vec3 v3_mul_v3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 v3_mul_f(const struct Vec3 _a, const float _b);
extern struct Vec3 v3_div_v3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 v3_div_f(const struct Vec3 _a, const float _b);
// Vector2
extern struct Vec2 v2_add_v2(const struct Vec2 _a, const struct Vec2 _b);
extern struct Vec2 v2_add_f(const struct Vec2 _a, const float _b);
extern struct Vec2 v2_sub_v2(const struct Vec2 _a, const struct Vec2 _b);
extern struct Vec2 v2_sub_f(const struct Vec2 _a, const float _b);
extern struct Vec2 v2_mul_v2(const struct Vec2 _a, const struct Vec2 _b);
extern struct Vec2 v2_mul_f(const struct Vec2 _a, const float _b);
extern struct Vec2 v2_div_v2(const struct Vec2 _a, const struct Vec2 _b);
extern struct Vec2 v2_div_f(const struct Vec2 _a, const float _b);

// Vector3
extern float dot(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 cross(const struct Vec3 _a, const struct Vec3 _b);
// Vector2
extern float dot_v2(const struct Vec2 _a, const struct Vec2 _b);
extern struct Vec3 cross_v2(const struct Vec2 _a, const struct Vec2 _b);

// Vector3
extern float mag(const struct Vec3 _a);
extern struct Vec3 norm(const struct Vec3 _a);
// Vector2
extern float mag_v2(const struct Vec2 _a);
extern struct Vec2 norm_v2(const struct Vec2 _a);

// Matrix4x4
extern void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at);
extern void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up);
extern void mtx_rotate_xy(float* _result, float _ax, float _ay);
extern void mtx_rotate_xyz(float* _result, float _ax, float _ay, float _az);
extern void mtx_rotate_zyx(float* _result, float _ax, float _ay, float _az);
extern void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc);

// common
extern float to_rad(float _degrees);

#endif // BX_MATH