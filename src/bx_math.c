#include "bx_math.h"
#include "ke_math.h"

#include <stdlib.h>
#include <stdint.h>

static void mtx_proj_xywh_internal(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool homogeneousNdc);

// TODO: should place somewhere else
static void mem_set_ref(void* _dst, uint8_t _ch, size_t _numBytes) {
	uint8_t* dst = (uint8_t*)_dst;
	const uint8_t* end = dst + _numBytes;
	while (dst != end) *dst++ = (char)_ch;
}

void mtx_look_at(float* _result, const struct Vec3* _eye, const struct Vec3* _at) {
	mtx_look_at_with_up(_result, _eye, _at, &UP_V3);
}

void mtx_look_at_with_up(float* _result, const struct Vec3* _eye, const struct Vec3* _at, const struct Vec3* _up) {
	const struct Vec3 view = NORM_V3(SUB_V3_V3(*_at, *_eye));
	
	struct Vec3 right = {0};
	struct Vec3 up = {0};
	
	const struct Vec3 uxv = CROSS_V3(*_up, view);
	
	right = DOT_V3(uxv, uxv) == 0.0f ? LEFT_V3 : NORM_V3(uxv);
	up = CROSS_V3(view, right);
	
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
	
	_result[12] = -DOT_V3(right, *_eye);
	_result[13] = -DOT_V3(up, *_eye);
	_result[14] = -DOT_V3(view, *_eye);
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

void mtx_rotate_xyz(float* _result, float _ax, float _ay, float _az) {
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	const float sz = sin(_az);
	const float cz = cos(_az);
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = cy * cz;
	_result[1] = -cy * sz;
	_result[2] = sy;
	_result[4] = cz * sx * sy + cx * sz;
	_result[5] = cx * cz - sx * sy * sz;
	_result[6] = -cy * sx;
	_result[8] = -cx * cz * sy + sx * sz;
	_result[9] = cz * sx + cx * sy * sz;
	_result[10] = cx * cy;
	_result[15] = 1.0f;
}

void mtx_rotate_zyx(float* _result, float _ax, float _ay, float _az) {
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	const float sz = sin(_az);
	const float cz = cos(_az);

	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = cy * cz;
	_result[1] = cz * sx * sy - cx * sz;
	_result[2] = cx * cz * sy + sx * sz;
	_result[4] = cy * sz;
	_result[5] = cx * cz + sx * sy * sz;
	_result[6] = -cz * sx + cx * sy * sz;
	_result[8] = -sy;
	_result[9] = cy * sx;
	_result[10] = cx * cy;
	_result[15] = 1.0f;
}

void mtx_proj(float* _result, float _fovy, float _aspect, float _near, float _far, bool _homogeneousNdc) {
	const float height = 1.0f / tan(RAD(_fovy) * 0.5f);
	const float width = height * 1.0f / _aspect;
	mtx_proj_xywh_internal(_result, 0.0f, 0.0f, width, height, _near, _far, _homogeneousNdc);
}

static void mtx_proj_xywh_internal(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool homogeneousNdc) {
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
