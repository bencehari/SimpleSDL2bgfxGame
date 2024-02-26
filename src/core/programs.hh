#ifndef KE_PROGRAMS_HH
#define KE_PROGRAMS_HH

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../sys/err.h"

namespace ProgramManager {
	/**
	 * @brief Initializes the Program Manager
	 *
	 * It reservers enough space in memory for program handles.
	 *
	 * @param _maxProgramCount Maximum number of programs it can create.
	 *
	 * @return ErrorCode.
	*/
	extern ErrorCode init(int _maxProgramCount);

	/**
	 * @brief Free up space reserved for program handles.
	*/
	extern void cleanup(void);

	/**
	 * @brief Creates the program.
	 *
	 * Creates and returns the program handle using bgfx. Operates only on binary (.bin) shaders shipped with the bgfx examples.
	 *
	 * @param _vertexShader Path to the vertex shader.
	 * @param _fragmentShader Path to the fragment shader.
	 * @param _destroyShaders Should the shaders destroyed with the program.
	 *
	 * @return The created bgfx::ProgramHandle.
	*/
	extern bgfx::ProgramHandle create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders);
}

#endif // KE_PROGRAMS_HH
