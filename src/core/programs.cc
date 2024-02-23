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
	void init(int _maxProgramCount) {
		if (initialized) {
			puts("Already initialized.");
			return;
		}
		
		programCount = _maxProgramCount;
		programs = (bgfx::ProgramHandle*)malloc(sizeof(bgfx::ProgramHandle) * _maxProgramCount);
		if (programs == NULL) {
			puts(AC_RED "Failed to allocate memory." AC_RESET);
			return;
		}
		
		initialized = true;
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
		
		if (!loadShader(_vertexShader, &vertexShaderHnd)) {
			// TODO: load fallback shader
		}
		
		if (!loadShader(_fragmentShader, &fragmentShaderHnd)) {
			// TODO: load fallback shader
		}
		
		programs[currentIndex] = bgfx::createProgram(vertexShaderHnd, fragmentShaderHnd, _destroyShaders);

		return programs[currentIndex++];
	}
}
