#include "programs.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../sys/loaders.hh"
#include "../utils/consc.h"

namespace ProgramManager {
	static bool initialized;

	static int programCount;
	static int currentIndex;
	static bgfx::ProgramHandle* programs;

	// TODO: handle errors!
	ErrorCode init(int _maxProgramCount) {
		if (initialized) return err_create(ALREADY_INITED, "ProgramManager::Init");
		
		programCount = _maxProgramCount;
		programs = (bgfx::ProgramHandle*)malloc(sizeof(bgfx::ProgramHandle) * _maxProgramCount);
		if (programs == NULL) return err_create(MEM_ALLOC, "ProgramManager::Init");
		
		initialized = true;
		return NONE;
	}

	void cleanup(void) {
		while (--currentIndex >= 0) {
			bgfx::destroy(programs[currentIndex]);
		}
		
		free(programs);
		
		programs = nullptr;
		currentIndex = 0;
		initialized = false;
	}

	bgfx::ProgramHandle create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders) {
		bgfx::ShaderHandle vertexShaderHnd { 0 };
		bgfx::ShaderHandle fragmentShaderHnd { 0 };
		
		if (loadShader(_vertexShader, &vertexShaderHnd) != NONE) {
			// TODO: load fallback shader
		}
		
		if (loadShader(_fragmentShader, &fragmentShaderHnd) != NONE) {
			// TODO: load fallback shader
		}
		
		programs[currentIndex] = bgfx::createProgram(vertexShaderHnd, fragmentShaderHnd, _destroyShaders);

		return programs[currentIndex++];
	}
}
