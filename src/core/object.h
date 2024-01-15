#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/c99/bgfx.h>

#include "../math/vector3.h"
#include "models.h"

#define OBJECT_EMPTY_CTOR ((struct Object) { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f } })
#define OBJECT_CTOR(_m, _p) ((struct Object) { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }, (_m), (_p)})

struct Object {
	float transform[16];
	struct Model* model;
	bgfx_program_handle_t program;
};

extern void obj_set_position(struct Object* _o, const struct Vec3* _v);
extern void obj_set_position_xyz(struct Object* _o, float _x, float _y, float _z);
extern struct Vec3 obj_get_position(const struct Object* _o);

extern void obj_render(const struct Object* _o);
extern void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o);

#endif // KE_OBJECT_H
