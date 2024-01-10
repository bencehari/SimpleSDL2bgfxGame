#include "object.h"

void set_position(struct Object* _o, const struct Vec3* _v) {
	_o->transform[3] = _v->x;
	_o->transform[7] = _v->y;
	_o->transform[11] = _v->z;
}

void set_position_xyz(struct Object* _o, float _x, float _y, float _z) {
	_o->transform[3] = _x;
	_o->transform[7] = _y;
	_o->transform[11] = _z;
}

struct Vec3 get_position(const struct Object* _o) {
	return (struct Vec3) { _o->transform[3], _o->transform[7], _o->transform[11] };
}
