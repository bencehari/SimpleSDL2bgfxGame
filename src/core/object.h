#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/c99/bgfx.h>

#include "../math/math.h"
#include "transform.h"
#include "models.h"

class Object {
private:
	Model* model;
	bgfx_program_handle_t program;
	
public:
	Transform transform;

	Object(Model* _model, bgfx_program_handle_t _program, Transform _transform = TRANSFORM_NEW) :
		model(_model),
		program(_program),
		transform(_transform) {}

	void render(void);
	void encoder_render(bgfx_encoder_t& _encoder);
};

#endif // KE_OBJECT_H
