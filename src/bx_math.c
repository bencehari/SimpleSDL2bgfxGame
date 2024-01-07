#include "bx_math.h"

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static void mtx_proj_xywh(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool homogeneousNdc);
// TODO: should place somewhere else
static void mem_set_ref(void* _dst, uint8_t _ch, size_t _numBytes);

const struct Vec3 vec3Zero = { 0.0f, 0.0f, 0.0f };
const struct Vec3 vec3GlobalUp = { 0.0f, 1.0f, 0.0f };
const struct Vec3 vec3GlobalDown = { 0.0f, -1.0f, 0.0f };
const struct Vec3 vec3GlobalLeft = { -1.0f, 0.0f, 0.0f };
const struct Vec3 vec3GlobalRight = { 1.0f, 0.0f, 0.0f };
const struct Vec3 vec3GlobalForward = { 0.0f, 0.0f, 1.0f };
const struct Vec3 vec3GlobalBack = { 0.0f, 0.0f, -1.0f };

inline struct Vec3 vec3_ctor(float _x, float _y, float _z) {
	struct Vec3 ret = { _x, _y, _z };
	return ret;
}

void vec3_print(struct Vec3* _v) {
	printf("{ %f; %f; %f }", _v->x, _v->y, _v->z);
}

inline struct Vec3 vec3_add_vec3(const struct Vec3 _a, const struct Vec3 _b) {
	struct Vec3 ret = { _a.x + _b.x, _a.y + _b.y, _a.z + _b.z };
	return ret;
}

inline struct Vec3 vec3_add_float(const struct Vec3 _a, const float _b) {
	struct Vec3 ret = { _a.x + _b, _a.y + _b, _a.z + _b };
	return ret;
}

inline struct Vec3 vec3_sub_vec3(const struct Vec3 _a, const struct Vec3 _b) {
	struct Vec3 ret = { _a.x - _b.x, _a.y - _b.y, _a.z - _b.z };
	return ret;
}

inline struct Vec3 vec3_sub_float(const struct Vec3 _a, const float _b) {
	struct Vec3 ret = { _a.x - _b, _a.y - _b, _a.z - _b };
	return ret;
}

inline struct Vec3 vec3_mul_vec3(const struct Vec3 _a, const struct Vec3 _b) {
	struct Vec3 ret = { _a.x * _b.x, _a.y * _b.y, _a.z * _b.z };
	return ret;
}

inline struct Vec3 vec3_mul_float(const struct Vec3 _a, const float _b) {
	struct Vec3 ret = { _a.x * _b, _a.y * _b, _a.z * _b };
	return ret;
}

inline struct Vec3 vec3_div_vec3(const struct Vec3 _a, const struct Vec3 _b) {
	struct Vec3 ret = { _a.x / _b.x, _a.y / _b.y, _a.z / _b.z };
	return ret;
}

inline struct Vec3 vec3_div_float(const struct Vec3 _a, const float _b) {
	struct Vec3 ret = { _a.x / _b, _a.y / _b, _a.z / _b };
	return ret;
}

inline float dot(const struct Vec3 _a, const struct Vec3 _b) {
	return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
}

inline struct Vec3 cross(const struct Vec3 _a, const struct Vec3 _b) {
	struct Vec3 ret = {
		_a.y * _b.z - _a.z * _b.y,
		_a.z * _b.x - _a.x * _b.z,
		_a.x * _b.y - _a.y * _b.x
	};
	return ret;
}

// magnitude
inline float mag(const struct Vec3 _a) {
	return (float)sqrt(dot(_a, _a));
}

// normalized
inline struct Vec3 norm(const struct Vec3 _a) {
	const float invLen = 1.0f / mag(_a);
	const struct Vec3 res = vec3_mul_float(_a, invLen);
	return res;
}

void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at) {
	mtx_look_at_with_up(_result, _eye, _at, &vec3GlobalUp);
}

void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up) {
	const struct Vec3 view = norm(vec3_sub_vec3(*_at, *_eye));
	
	struct Vec3 right = {0};
	struct Vec3 up = {0};
	
	const struct Vec3 uxv = cross(*_up, view);
	
	right = dot(uxv, uxv) == 0.0f ? vec3GlobalLeft : norm(uxv);
	up = cross(view, right);
	
	_result[0] = right.x;
	_result[1] = up.x;
	_result[2] = view.x;
	_result[3] = 0.0f;
	
	_result[4] = right.y;
	_result[5] = up.y;
	_result[6] = view.y;
	_result[7] = 0.0f;
	
	_result[8] = right.z;
	_result[9] = up.z;
	_result[10] = view.z;
	_result[11] = 0.0f;
	
	_result[12] = -dot(right, *_eye);
	_result[13] = -dot(up, *_eye);
	_result[14] = -dot(view, *_eye);
	_result[15] = 1.0f;
}

void mtx_rotate_xy(float* _result, float _ax, float _ay) {
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = cy;
	_result[2] = sy;
	_result[4] = sx * sy;
	_result[5] = cx;
	_result[6] = -sx * cy;
	_result[8] = -cx * sy;
	_result[9] = sx;
	_result[10] = cx * cy;
	_result[15] = 1.0f;
}

void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc) {
	const float height = 1.0f / tan(to_rad(_fovy) * 0.5f);
	const float width = height * 1.0f / _aspect;
	mtx_proj_xywh(_result, 0.0f, 0.0f, width, height, _near, _far, _homogeneousNdc);
}

static void mtx_proj_xywh(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool homogeneousNdc) {
	const float diff = _far - _near;
	const float aa = homogeneousNdc ? (_far + _near) / diff : _far / diff;
	const float bb = homogeneousNdc ? (2.0f * _far * _near) / diff : _near * aa;
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = _width;
	_result[5] = _height;
	_result[8] = -_x;
	_result[9] = -_y;
	_result[10] = aa;
	_result[11] = 1.0f;
	_result[14] = -bb;
}

inline float to_rad(float _degrees) {
	return _degrees * (M_PI / 180.0f);
}

// TODO: should place somewhere else
static void mem_set_ref(void* _dst, uint8_t _ch, size_t _numBytes) {
	uint8_t* dst = (uint8_t*)_dst;
	const uint8_t* end = dst + _numBytes;
	while (dst != end) *dst++ = (char)_ch;
}
