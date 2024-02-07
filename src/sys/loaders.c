#include "loaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/file.h"
#include "../utils/consc.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
struct Model* load_external_obj_model(const char* _objPath, const bgfx_vertex_layout_t _vertexLayout) {
#pragma GCC diagnostic pop

	return NULL;
}

bool load_shader(const char* _filename, bgfx_shader_handle_t* _shaderHandle) {
	const char* shaderPath;
	
	switch (bgfx_get_renderer_type()) {
		case BGFX_RENDERER_TYPE_NOOP:
		case BGFX_RENDERER_TYPE_AGC: shaderPath = "assets/shaders/dx9/"; break;
		case BGFX_RENDERER_TYPE_DIRECT3D11:
		case BGFX_RENDERER_TYPE_DIRECT3D12: shaderPath = "assets/shaders/dx11/"; break;
		case BGFX_RENDERER_TYPE_GNM: shaderPath = "assets/shaders/pssl/"; break;
		case BGFX_RENDERER_TYPE_METAL: shaderPath = "assets/shaders/metal/"; break;
		case BGFX_RENDERER_TYPE_NVN: break;
		case BGFX_RENDERER_TYPE_OPENGLES: shaderPath = "assets/shaders/essl"; break;
		case BGFX_RENDERER_TYPE_OPENGL: shaderPath = "assets/shaders/glsl/"; break;
		case BGFX_RENDERER_TYPE_VULKAN: shaderPath = "assets/shaders/spirv/"; break;
		case BGFX_RENDERER_TYPE_COUNT: break;
	}
	
	// printf(AC_YELLOW "ShaderName: '%s'.\nPath: '%s'\n" AC_RESET, _filename, shaderPath);
	
	size_t shaderPathLen = strlen(shaderPath);
	size_t fileNameLen = strlen(_filename);
	
	char* filePath = (char*)malloc(shaderPathLen + fileNameLen + 1);
	memcpy(filePath, shaderPath, shaderPathLen);
	memcpy(&filePath[shaderPathLen], _filename, fileNameLen);
	filePath[shaderPathLen + fileNameLen] = '\0';
	
	// printf(AC_YELLOW "%s\n" AC_RESET, filePath);
	
	FILE* file = NULL;
	long fileSize = 0;
	if (!file_get_size(filePath, &fileSize, &file)) return false;
	
	// printf(AC_YELLOW "fileSize: %ld\n" AC_RESET, fileSize);
	
	const bgfx_memory_t* mem = bgfx_alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);
	
	bgfx_shader_handle_t h = bgfx_create_shader(mem);
	
	*_shaderHandle = h;
	
	return true;
}
