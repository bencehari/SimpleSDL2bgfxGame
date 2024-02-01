#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/c99/bgfx.h>

#include "../math/math_include.h"
#include "transform.h"
#include "models.h"

#define OBJECT_NEW(_m, _p) ((struct Object) { TRANSFORM_DEFAULT, (_m), (_p)})

struct Object {
	struct Transform transform;
	struct Model* model;
	bgfx_program_handle_t program;
};

extern void obj_render(const struct Object* _o);
extern void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o);

#endif // KE_OBJECT_H
