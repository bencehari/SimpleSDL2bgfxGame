#include "transform.h"

void tr_fps_rotate(Transform* _t, const Vector2 _input) {
	_t->rotation = Q_AA(AXIS_Y, _input.Y) * _t->rotation;
	Vector3 localRight = rotate_v3_by_q(&globalRight, &_t->rotation);
	_t->rotation = Q_AA(localRight, _input.X) * _t->rotation;
}

Vector3 tr_get_forward(const Transform* _t) {
	return rotate_v3_by_q(&globalForward, &_t->rotation);
}

Vector3 tr_get_right(const Transform* _t) {
	return rotate_v3_by_q(&globalRight, &_t->rotation);
}

Vector3 tr_get_up(const Transform* _t) {
	return rotate_v3_by_q(&globalUp, &_t->rotation);
}

Vector3 tr_get_look_at(const Transform* _t) {
	return _t->position + rotate_v3_by_q(&globalForward, &_t->rotation);
}
