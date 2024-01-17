#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/c99/bgfx.h>

#include "../math/HMM_include.h"
#include "models.h"

#define OBJECT_CTOR(_m, _p) ((struct Object) {HMM_M4D(1.0f), (_m), (_p)})

struct Object {
	HMM_Mat4 transform;
	struct Model* model;
	bgfx_program_handle_t program;
};

extern void obj_render(const struct Object* _o);
extern void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o);

#endif // KE_OBJECT_H
