#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/c99/bgfx.h>

#include "../math/HMM_include.h"
#include "models.h"

#define OBJECT_CTOR(_m, _p) ((struct Object) {MAT4_IDENTITY, QUAT_IDENTITY, (_m), (_p)})

struct Object {
	Mat4 transform;
	Quat rotation;
	struct Model* model;
	bgfx_program_handle_t program;
};

extern void obj_rot_x(struct Object* _o, float _angle);
extern void obj_rot_y(struct Object* _o, float _angle);
extern void obj_rot_z(struct Object* _o, float _angle);
extern void obj_rot_xy(struct Object* _o, float _xAngle, float _yAngle);
extern void obj_rot_xz(struct Object* _o, float _xAngle, float _zAngle);
extern void obj_rot_yz(struct Object* _o, float _yAngle, float _zAngle);
extern void obj_rot_xyz(struct Object* _o, float _xAngle, float _yAngle, float _zAngle);

extern void obj_set_rot_xyz(struct Object* _o, float _xAngle, float _yAngle, float _zAngle);

extern void obj_render(const struct Object* _o);
extern void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o);

#endif // KE_OBJECT_H
