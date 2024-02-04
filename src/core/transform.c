#include "transform.h"

void tr_fps_rotate(struct Transform* _t, const Vector2 _input) {
	_t->rotation = Q_MUL(Q_AA(AXIS_Y, _input.Y), _t->rotation);
	Vector3 localRight = rotate_v3_by_q(&globalRight, &_t->rotation);
	_t->rotation = Q_MUL(Q_AA(localRight, _input.X), _t->rotation);
}

Vector3 tr_get_look_at(const struct Transform* _t) {
	return V3_ADD(_t->position, rotate_v3_by_q(&globalForward, &_t->rotation));
}
