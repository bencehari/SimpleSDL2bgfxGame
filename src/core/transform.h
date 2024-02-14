#ifndef KE_TRANSFORM_H
#define KE_TRANSFORM_H

#include "../math/math.h"

struct Transform {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	
	Transform(Vector3 _position, Quaternion _rotation, Vector3 _scale) :
		position(_position),
		rotation(_rotation),
		scale(_scale) {}
};

#define TRANSFORM_NEW Transform(V3_ZERO, Q_IDENTITY, V3_ONE)

extern void tr_fps_rotate(Transform* _t, const Vector2 _input);

extern Vector3 tr_get_forward(const Transform* _t);
extern Vector3 tr_get_right(const Transform* _t);
extern Vector3 tr_get_up(const Transform* _t);
extern Vector3 tr_get_look_at(const Transform* _t);

#endif // KE_TRANSFORM_H
