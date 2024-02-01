#ifndef KE_TRANSFORM_H
#define KE_TRANSFORM_H

#include "../math/math_include.h"

struct Transform {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

#define TRANSFORM_NEW(_v3_pos, _q_rot, _v3_sc) ((struct Transform) { (_v3_pos), (_q_rot), (_v3_sc) })
#define TRANSFORM_DEFAULT ((struct Transform) { V3_ZERO, QUAT_IDENTITY, V3_ONE })

extern void tr_rot_x(struct Transform* _t, float _angle);
extern void tr_rot_y(struct Transform* _t, float _angle);
extern void tr_rot_z(struct Transform* _t, float _angle);
extern void tr_rot_xy(struct Transform* _t, float _xAngle, float _yAngle);
extern void tr_rot_xz(struct Transform* _t, float _xAngle, float _zAngle);
extern void tr_rot_yz(struct Transform* _t, float _yAngle, float _zAngle);
extern void tr_rot_xyz(struct Transform* _t, float _xAngle, float _yAngle, float _zAngle);

extern void tr_set_rot_xyz(struct Transform* _t, float _xAngle, float _yAngle, float _zAngle);

#endif // KE_TRANSFORM_H
