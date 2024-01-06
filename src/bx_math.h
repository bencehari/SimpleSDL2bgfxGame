#ifndef BX_MATH
#define BX_MATH

#include <stdbool.h>

// C implementation of math.cpp in bx. Only what is needed for the actual task.
// handedness removed altogether and lefty used instead.

struct Vec3 {
	float x, y, z;
};

extern const struct Vec3 vec3Zero;
extern const struct Vec3 vec3GlobalUp;
extern const struct Vec3 vec3GlobalDown;
extern const struct Vec3 vec3GlobalLeft;
extern const struct Vec3 vec3GlobalRight;
extern const struct Vec3 vec3GlobalForward;
extern const struct Vec3 vec3GlobalBack;

extern struct Vec3 vec3_ctor(float _x, float _y, float _z);
extern void vec3_print(struct Vec3* _v);

extern struct Vec3 vec3_add_vec3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 vec3_add_float(const struct Vec3 _a, const float _b);
extern struct Vec3 vec3_sub_vec3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 vec3_sub_float(const struct Vec3 _a, const float _b);
extern struct Vec3 vec3_mul_vec3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 vec3_mul_float(const struct Vec3 _a, const float _b);
extern struct Vec3 vec3_div_vec3(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 vec3_div_float(const struct Vec3 _a, const float _b);

extern float dot(const struct Vec3 _a, const struct Vec3 _b);
extern struct Vec3 cross(const struct Vec3 _a, const struct Vec3 _b);

extern float mag(const struct Vec3 _a);
extern struct Vec3 norm(const struct Vec3 _a);

extern void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at);
extern void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up);
extern void mtx_rotate_xy(float* _result, float _ax, float _ay);

extern void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc);

extern float to_rad(float _degrees);

#endif // BX_MATH