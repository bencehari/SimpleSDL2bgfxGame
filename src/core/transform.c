#include "transform.h"

// O -> orientation
// R -> rotation
// Rx, Ry, Rz -> rotation on x, y or z axis

// INSPECT WITH SUSPICION !!! MATH COULD BE WRONG !!!

void tr_rot_x(struct Transform* _t, float _angle) {
	_t->rotation = Q_MUL(_t->rotation, Q_AA(AXIS_X, _angle));
}

void tr_rot_y(struct Transform* _t, float _angle) {
	_t->rotation = Q_MUL(_t->rotation, Q_AA(AXIS_Y, _angle));
}

void tr_rot_z(struct Transform* _t, float _angle) {
	_t->rotation = Q_MUL(_t->rotation, Q_AA(AXIS_Z, _angle));
}

void tr_rot_xy(struct Transform* _t, float _xAngle, float _yAngle) {
	if (_yAngle == 0.0f) tr_rot_x(_t, _xAngle);
	else if (_xAngle == 0.0f) tr_rot_y(_t, _yAngle);
	else _t->rotation = Q_MUL(_t->rotation, Q_MUL(Q_AA(AXIS_X, _xAngle), Q_AA(AXIS_Y, _yAngle)));
}

void tr_rot_xz(struct Transform* _t, float _xAngle, float _zAngle) {
	if (_zAngle == 0.0f) tr_rot_x(_t, _xAngle);
	else if (_xAngle == 0.0f) tr_rot_z(_t, _zAngle);
	else _t->rotation = Q_MUL(Q_MUL(Q_AA(AXIS_X, _xAngle), Q_AA(AXIS_Z, _zAngle)), _t->rotation);
}

void tr_rot_yz(struct Transform* _t, float _yAngle, float _zAngle) {
	if (_zAngle == 0.0f) tr_rot_y(_t, _yAngle);
	else if (_yAngle == 0.0f) tr_rot_z(_t, _zAngle);
	else _t->rotation = Q_MUL(Q_MUL(Q_AA(AXIS_Y, _yAngle), Q_AA(AXIS_Z, _zAngle)), _t->rotation);
}

void tr_rot_xyz(struct Transform* _t, float _xAngle, float _yAngle, float _zAngle) {
	if (_zAngle == 0.0f) tr_rot_xy(_t, _xAngle, _yAngle);
	else if (_yAngle == 0.0f) tr_rot_xz(_t, _xAngle, _zAngle);
	else if (_xAngle == 0.0f) tr_rot_yz(_t, _yAngle, _zAngle);
	else _t->rotation = Q_MUL(Q_MUL(Q_MUL(Q_AA(AXIS_X, _xAngle), Q_AA(AXIS_Y, _yAngle)), Q_AA(AXIS_Z, _zAngle)), _t->rotation);
}

void tr_set_rot_xyz(struct Transform* _t, float _xAngle, float _yAngle, float _zAngle) {
	_t->rotation = Q_MUL(Q_MUL(Q_AA(AXIS_X, _xAngle), Q_AA(AXIS_Y, _yAngle)), Q_AA(AXIS_Z, _zAngle));
}
