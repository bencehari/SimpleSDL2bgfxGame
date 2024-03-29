#ifndef KE_OBJECT_HH
#define KE_OBJECT_HH

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../sys/err.h"
#include "../math/math.h"
#include "transform.hh"
#include "models.hh"

/**
 * @brief Object class represent a visible entity.
*/
class Object {
private:
	Model* model;
	bgfx::ProgramHandle program;
	
public:
	/**
	 * @brief Transform of the Object.
	*/
	Transform transform;

	/**
	 * @brief DO NOT call this directly!
	 *
	 * Create Object by Object::create(..) static function (or ObjectManager::create(..)).
	*/
	Object(Model* _model, bgfx::ProgramHandle _program, Transform _transform);

	/**
	 * @brief Creates a new Object.
	 *
	 * It also adds the Object to the ObjectManager.
	 *
	 * @param _model Model.
	 * @param _program bgfx::ProgramHandle.
	 * @param _transform Transform (default: TRANSFORM_NEW).
	 *
	 * @return Pointer to the Object, nullptr on error.
	*/
	static Object* create(Model* _model, bgfx::ProgramHandle _program, Transform _transform = TRANSFORM_NEW);

	/**
	 * @brief DO NOT call this directly!
	 *
	 * Let ObjectManager handle rendering.
	*/
	void render(void);
};

namespace ObjectManager {
	/**
	 * @brief Initializes the ObjectManager.
	 *
	 * Allocates memory for Objects based on parameters.
	 *
	 * @param _maxObjectCount One could not create more objects then this.
	 *
	 * @return ErrorCode.
	*/
	extern ErrorCode init(int _maxObjectCount);
	
	/**
	 * @brief Cleans up Objects.
	 *
	 * Frees all occupied memory.
	*/
	extern void cleanup(void);

	/**
	 * @brief Creates a new Object.
	 *
	 * @param _model Model.
	 * @param _program bgfx::ProgramHandle.
	 * @param _transform Transform (default: TRANSFORM_NEW).
	 *
	 * @return Pointer to the Object, nullptr on error.
	*/
	extern Object* create(Model* _model, bgfx::ProgramHandle _program, Transform _transform);
	
	/**
	 * @brief Creates a new Object.
	 *
	 * @param _program bgfx::ProgramHandle.
	 * @param _transform Transform (default: TRANSFORM_NEW).
	 *
	 * @return Pointer to the Object, nullptr on error.
	*/
	extern Object* createTestCube(bgfx::ProgramHandle _programHandle, Transform _transform = TRANSFORM_NEW);

	extern void render(void);
}

#endif // KE_OBJECT_HH
