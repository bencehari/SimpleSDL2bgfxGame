#ifndef KE_TRANSFORM_H
#define KE_TRANSFORM_H

#include "../math/math.h"

struct Transform {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

#define TRANSFORM_NEW(_v3_pos, _q_rot, _v3_sc) ((struct Transform) { (_v3_pos), (_q_rot), (_v3_sc) })
#define TRANSFORM_DEFAULT ((struct Transform) { V3_ZERO, Q_IDENTITY, V3_ONE })

extern void tr_fps_rotate(struct Transform* _t, const Vector2 _input);

extern Vector3 tr_get_forward(const struct Transform* _t);
extern Vector3 tr_get_right(const struct Transform* _t);
extern Vector3 tr_get_up(const struct Transform* _t);
extern Vector3 tr_get_look_at(const struct Transform* _t);

#endif // KE_TRANSFORM_H
