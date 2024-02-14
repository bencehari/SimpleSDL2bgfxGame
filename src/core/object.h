#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <bgfx/bgfx.h>

#include "../math/math.h"
#include "transform.h"
#include "models.h"

class Object {
private:
	Model* model;
	bgfx::ProgramHandle program;
	
public:
	Transform transform;

	Object(Model* _model, bgfx::ProgramHandle _program, Transform _transform = TRANSFORM_NEW) :
		model(_model),
		program(_program),
		transform(_transform) {}

	void render(void);
};

#endif // KE_OBJECT_H
