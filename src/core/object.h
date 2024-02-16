#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../math/math.h"
#include "transform.h"
#include "models.h"

/**
 * @brief Object class represent a visible entity.
*/
class Object {
private:
	Model* model;
	bgfx::ProgramHandle program;
	
public:
	Transform transform;

	/**
	 * @brief DO NOT call this directly!
	*/
	Object(Model* _model, bgfx::ProgramHandle _program, Transform _transform) :
		model(_model),
		program(_program),
		transform(_transform) {}

	static Object* create(Model* _model, bgfx::ProgramHandle _program, Transform _transform = TRANSFORM_NEW);

	void render(void);
};

namespace ObjectManager {
	extern void init(int _maxObjectCount);
	extern void cleanup(void);

	extern Object* create(Model* _model, bgfx::ProgramHandle _program, Transform _transform);
	extern Object* createTestCube(bgfx::ProgramHandle _programHandle, Transform _transform = TRANSFORM_NEW);

	extern void render(void);
}

#endif // KE_OBJECT_H
