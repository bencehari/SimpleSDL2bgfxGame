#include "programs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../sys/loaders.h"
#include "../utils/consc.h"

static int programCount;
static int currentProgramIndex = 0;
static bgfx_program_handle_t** programs;

void programs_initialize(int _maxProgramsCount) {
	programCount = _maxProgramsCount;
	programs = malloc(sizeof(bgfx_program_handle_t*) * _maxProgramsCount);
}

void programs_deinitialize() {
	for (int i = 0; i < programCount; i++) {
		bgfx_destroy_program(*programs[i]);
	}
}

int program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders) {
	if (currentProgramIndex >= programCount) {
		puts(ANSI_COLOR_YELLOW "Max shader program count reached." ANSI_COLOR_RESET);
		return -1;
	}
	
	/* TODO: handle somehow. Throws at shutdown:
	warning: ../../../src/bgfx.cpp (2196): BGFX LEAK: IndexBufferHandle 1 (max: 4096)
	warning: ../../../src/bgfx.cpp (2196): BGFX       0:    3
	warning: ../../../src/bgfx.cpp (2198): BGFX LEAK: VertexBufferHandle 1 (max: 4096)
	warning: ../../../src/bgfx.cpp (2198): BGFX       0:    4
	warning: ../../../src/bgfx.cpp (3625): BGFX Shutdown complete.
	warning: ../../../src/bgfx.cpp(1475): ASSERT (NULL != s_ctx && s_ctx->m_singleThreaded) || ~BGFX_API_THREAD_MAGIC == s_threadIndex -> Must be called from render thread.
	warning: ../../../src/bgfx.cpp(1475): BGFX FATAL 0x00000000: Must be called from render thread.*/
	
	bgfx_shader_handle_t vertexShaderHnd = load_shader(_vertexShader);
	bgfx_shader_handle_t fragmentShaderHnd = load_shader(_fragmentShader);
	// TODO: are loads were successful?
	
	bgfx_program_handle_t program = bgfx_create_program(vertexShaderHnd, fragmentShaderHnd, _destroyShaders);
	
	size_t size = sizeof(bgfx_program_handle_t);
	bgfx_program_handle_t* pprogram = malloc(size);
	memcpy(pprogram, &program, size);

	programs[currentProgramIndex] = pprogram;
	return currentProgramIndex++;
}

bgfx_program_handle_t* program_get_by_idx(int _idx) {
	if (_idx >= programCount) {
		printf(ANSI_COLOR_YELLOW "Shader program index (%d) is too high (max: %d)." ANSI_COLOR_RESET, _idx, programCount);
		return NULL;
	}
	
	return programs[_idx];
}
