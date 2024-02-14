#include "programs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../sys/loaders.h"
#include "../utils/consc.h"

static bool initialized;

static int programCount;
static int currentProgramIndex = 0;
static bgfx::ProgramHandle* programs;

void programs_init(int _maxProgramCount) {
	if (initialized) {
		programs_cleanup();
		currentProgramIndex = 0;
	}
	
	programCount = _maxProgramCount;
	programs = (bgfx::ProgramHandle*)malloc(sizeof(bgfx::ProgramHandle) * _maxProgramCount);
	
	initialized = true;
}

void programs_cleanup(void) {
	while (--currentProgramIndex >= 0) {
		bgfx::destroy(programs[currentProgramIndex]);
		free(&programs[currentProgramIndex]);
	}
	
	programs = nullptr;
	initialized = false;
}

bgfx::ProgramHandle program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders) {
	bgfx::ShaderHandle vertexShaderHnd = {0};
	bgfx::ShaderHandle fragmentShaderHnd = {0};
	
	if (!load_shader(_vertexShader, &vertexShaderHnd)) {
		// TODO: load fallback shader
	}
	
	if (!load_shader(_fragmentShader, &fragmentShaderHnd)) {
		// TODO: load fallback shader
	}
	
	programs[currentProgramIndex] = bgfx::createProgram(vertexShaderHnd, fragmentShaderHnd, _destroyShaders);

	return programs[currentProgramIndex++];
}
