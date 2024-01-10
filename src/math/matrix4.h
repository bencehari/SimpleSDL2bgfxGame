#ifndef KE_MATRIX4_H
#define KE_MATRIX4_H

#include <stdbool.h>

#include "vector3.h"

// Matrix4x4
#define IDENTITY_MTX { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }

extern void mtx_print(const float* _a);

extern void mul_mtx_mtx(float* _result, const float* _a, const float* _b);

extern void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at);
extern void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up);
extern void mtx_rotate_x(float* _result, float _ax);
extern void mtx_rotate_y(float* _result, float _ay);
extern void mtx_rotate_z(float* _result, float _az);
extern void mtx_rotate_xy(float* _result, float _ax, float _ay);
extern void mtx_rotate_xyz(float* _result, float _ax, float _ay, float _az);
extern void mtx_rotate_zyx(float* _result, float _ax, float _ay, float _az);
extern void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc);

#endif // KE_MATRIX4_H