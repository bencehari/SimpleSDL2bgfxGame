#include "object.h"

void obj_rot_x(struct Object* _o, float _angle) {
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(QUAT_AA(AXIS_X, _angle)));
}

void obj_rot_y(struct Object* _o, float _angle) {
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(QUAT_AA(AXIS_Y, _angle)));
}

void obj_rot_z(struct Object* _o, float _angle) {
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(QUAT_AA(AXIS_Z, _angle)));
}

void obj_rot_xy(struct Object* _o, float _xAngle, float _yAngle) {
	if (_yAngle == 0.0f) {
		obj_rot_x(_o, _xAngle);
		return;
	}
	else if (_xAngle == 0.0f) {
		obj_rot_y(_o, _yAngle);
		return;
	}
	
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(MUL_Q(QUAT_AA(AXIS_X, _xAngle), QUAT_AA(AXIS_Y, _yAngle))));
}

void obj_rot_xz(struct Object* _o, float _xAngle, float _zAngle) {
	if (_zAngle == 0.0f) {
		obj_rot_x(_o, _xAngle);
		return;
	}
	else if (_xAngle == 0.0f) {
		obj_rot_z(_o, _zAngle);
		return;
	}
	
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(MUL_Q(QUAT_AA(AXIS_X, _xAngle), QUAT_AA(AXIS_Z, _zAngle))));
}

void obj_rot_yz(struct Object* _o, float _yAngle, float _zAngle) {
	if (_zAngle == 0.0f) {
		obj_rot_y(_o, _yAngle);
		return;
	}
	else if (_yAngle == 0.0f) {
		obj_rot_z(_o, _zAngle);
		return;
	}
	
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(MUL_Q(QUAT_AA(AXIS_Y, _yAngle), QUAT_AA(AXIS_Z, _zAngle))));
}

void obj_rot_xyz(struct Object* _o, float _xAngle, float _yAngle, float _zAngle) {
	if (_zAngle == 0.0f) {
		obj_rot_xy(_o, _xAngle, _yAngle);
		return;
	}
	else if (_yAngle == 0.0f) {
		obj_rot_xz(_o, _xAngle, _zAngle);
		return;
	}
	else if (_xAngle == 0.0f) {
		obj_rot_yz(_o, _yAngle, _zAngle);
		return;
	}
	
	_o->transform = MUL(_o->transform, QUAT_TO_MAT4(MUL_Q(MUL_Q(QUAT_AA(AXIS_X, _xAngle), QUAT_AA(AXIS_Y, _yAngle)), QUAT_AA(AXIS_Z, _zAngle))));
}

void obj_set_rot_xyz(struct Object* _o, float _xAngle, float _yAngle, float _zAngle) {
	_o->transform = MUL(MAT4_IDENTITY, QUAT_TO_MAT4(MUL_Q(MUL_Q(QUAT_AA(AXIS_X, _xAngle), QUAT_AA(AXIS_Y, _yAngle)), QUAT_AA(AXIS_Z, _zAngle))));
}

void obj_render(const struct Object* _o) {
	bgfx_set_transform(_o->transform.Elements, 1);
		
	bgfx_set_vertex_buffer(0, _o->model->vertexBufferHnd, 0, _o->model->verticesLen);
	bgfx_set_index_buffer(_o->model->indexBufferHnd, 0, _o->model->indicesLen);
	
	bgfx_submit(0, _o->program, 0, BGFX_DISCARD_ALL);
}

void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o) {
	bgfx_encoder_set_transform(_encoder, _o->transform.Elements, 1);
		
	bgfx_encoder_set_vertex_buffer(_encoder, 0, _o->model->vertexBufferHnd, 0, _o->model->verticesLen);
	bgfx_encoder_set_index_buffer(_encoder, _o->model->indexBufferHnd, 0, _o->model->indicesLen);
	
	bgfx_encoder_submit(_encoder, 0, _o->program, 0, BGFX_DISCARD_ALL);
}
