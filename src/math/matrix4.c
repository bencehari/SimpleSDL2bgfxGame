#include "matrix4.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vector3.h"
#include "conv.h"

static void mtx_proj_xywh_internal(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool homogeneousNdc);

static void mem_set_ref(void* _dst, uint8_t _ch, size_t _numBytes) {
	uint8_t* dst = (uint8_t*)_dst;
	const uint8_t* end = dst + _numBytes;
	while (dst != end) *dst++ = (char)_ch;
}

void mtx_print(const float* _a) {
	printf("%2.2f %2.2f %2.2f %2.2f\n%2.2f %2.2f %2.2f %2.2f\n%2.2f %2.2f %2.2f %2.2f\n%2.2f %2.2f %2.2f %2.2f\n",
		_a[0], _a[1], _a[2], _a[3],
		_a[4], _a[5], _a[6], _a[7],
		_a[8], _a[9], _a[10], _a[11],
		_a[12], _a[13], _a[14], _a[15]);
}

void mul_mtx_mtx(float* _result, const float* _a, const float* _b) {
	_result[0] = _a[0] * _b[0] + _a[1] * _b[4] + _a[2] * _b[8] + _a[3] * _b[12];
	_result[1] = _a[0] * _b[1] + _a[1] * _b[5] + _a[2] * _b[9] + _a[3] * _b[13];
	_result[2] = _a[0] * _b[2] + _a[1] * _b[6] + _a[2] * _b[10] + _a[3] * _b[14];
	_result[3] = _a[0] * _b[3] + _a[1] * _b[7] + _a[2] * _b[11] + _a[3] * _b[15];
	_result[4] = _a[4] * _b[0] + _a[5] * _b[4] + _a[6] * _b[8] + _a[7] * _b[12];
	_result[5] = _a[4] * _b[1] + _a[5] * _b[5] + _a[6] * _b[9] + _a[7] * _b[13];
	_result[6] = _a[4] * _b[2] + _a[5] * _b[6] + _a[6] * _b[10] + _a[7] * _b[14];
	_result[7] = _a[4] * _b[3] + _a[5] * _b[7] + _a[6] * _b[11] + _a[7] * _b[15];
	_result[8] = _a[8] * _b[0] + _a[9] * _b[4] + _a[10] * _b[8] + _a[11] * _b[12];
	_result[9] = _a[8] * _b[1] + _a[9] * _b[5] + _a[10] * _b[9] + _a[11] * _b[13];
	_result[10] = _a[8] * _b[2] + _a[9] * _b[6] + _a[10] * _b[10] + _a[11] * _b[14];
	_result[11] = _a[8] * _b[3] + _a[9] * _b[7] + _a[10] * _b[11] + _a[11] * _b[15];
	_result[12] = _a[12] * _b[0] + _a[13] * _b[4] + _a[14] * _b[8] + _a[15] * _b[12];
	_result[13] = _a[12] * _b[1] + _a[13] * _b[5] + _a[14] * _b[9] + _a[15] * _b[13];
	_result[14] = _a[12] * _b[2] + _a[13] * _b[6] + _a[14] * _b[10] + _a[15] * _b[14];
	_result[15] = _a[12] * _b[3] + _a[13] * _b[7] + _a[14] * _b[11] + _a[15] * _b[15];
	
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("_result[%d] = ", i * 4 + j);
			for (int k = 0; k < 4; k++) {
				printf("_a[%d] * _b[%d]", i * 4 + k, j + 4 * k);
				if (k < 3) printf(" + ");
			}
			printf(";\n");
		}
	}*/
}

void mtx_rotate_x(float* _result, float _ax) {
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = 1.0f;
	_result[5] = cx;
	_result[6] = -sx;
	_result[9] = sx;
	_result[10] = cx;
	_result[15] = 1.0f;
}

void mtx_rotate_y(float* _result, float _ay) {
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = cy;
	_result[2] = sy;
	_result[5] = 1.0f;
	_result[8] = -sy;
	_result[10] = cy;
	_result[15] = 1.0f;
}

void mtx_rotate_z(float* _result, float _az) {
	const float sz = sin(_az);
	const float cz = cos(_az);
	
	mem_set_ref(_result, 0, sizeof(float) * 16);
	_result[0] = cz;
	_result[1] = -sz;
	_result[4] = sz;
	_result[5] = cz;
	_result[14] = 1.0f;
	_result[15] = 1.0f;
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
	const float height = 1.0f / tan(RAD_TO_DEG(_fovy) * 0.5f);
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
