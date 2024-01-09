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

// Matrix4x4
extern void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at);
extern void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up);
extern void mtx_rotate_xy(float* _result, float _ax, float _ay);
extern void mtx_rotate_xyz(float* _result, float _ax, float _ay, float _az);
extern void mtx_rotate_zyx(float* _result, float _ax, float _ay, float _az);
extern void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc);

#endif // BX_MATH