#include "programs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../sys/loaders.h"
#include "../utils/consc.h"

static bool initialized;

static int programCount;
static int currentProgramIndex = 0;
static bgfx_program_handle_t* programs;

void programs_init(int _maxProgramCount) {
	if (initialized) {
		programs_cleanup();
		currentProgramIndex = 0;
	}
	
	programCount = _maxProgramCount;
	programs = malloc(sizeof(bgfx_program_handle_t) * _maxProgramCount);
	
	initialized = true;
}

void programs_cleanup(void) {
	while (--currentProgramIndex >= 0) {
		bgfx_destroy_program(programs[currentProgramIndex]);
		free(&programs[currentProgramIndex]);
	}
	
	programs = NULL;
	initialized = false;
}

bgfx_program_handle_t program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders) {
	/* TODO:
	warning: ../../../src/bgfx.cpp (2196): BGFX LEAK: IndexBufferHandle 1 (max: 4096)
	warning: ../../../src/bgfx.cpp (2196): BGFX       0:    3
	warning: ../../../src/bgfx.cpp (2198): BGFX LEAK: VertexBufferHandle 1 (max: 4096)
	warning: ../../../src/bgfx.cpp (2198): BGFX       0:    4
	warning: ../../../src/bgfx.cpp (3625): BGFX Shutdown complete.*/
	
	bgfx_shader_handle_t vertexShaderHnd = load_shader(_vertexShader);
	bgfx_shader_handle_t fragmentShaderHnd = load_shader(_fragmentShader);
	
	programs[currentProgramIndex] = bgfx_create_program(vertexShaderHnd, fragmentShaderHnd, _destroyShaders);

	return programs[currentProgramIndex++];
}
