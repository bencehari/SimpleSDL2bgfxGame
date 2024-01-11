#include "loaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/consc.h"

// TODO: handle errors like invalid path, fopen returns NULL, etc.
bgfx_shader_handle_t load_shader(const char* _filename) {
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
	
	// printf(ANSI_COLOR_YELLOW "ShaderName: '%s'.\nPath: '%s'\n" ANSI_COLOR_RESET, _filename, shaderPath);
	
	size_t shaderPathLen = strlen(shaderPath);
	size_t fileNameLen = strlen(_filename);
	
	char* filePath = (char*)malloc(shaderPathLen + fileNameLen + 1);
	memcpy(filePath, shaderPath, shaderPathLen);
	memcpy(&filePath[shaderPathLen], _filename, fileNameLen);
	filePath[shaderPathLen + fileNameLen] = '\0';
	
	// printf(ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, filePath);
	
	FILE* file = fopen(filePath, "rb");
	// if (file == NULL) printf(ANSI_COLOR_YELLOW "file is NULL\n" ANSI_COLOR_RESET);
	
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	// printf(ANSI_COLOR_YELLOW "fileSize: %ld\n" ANSI_COLOR_RESET, fileSize);
	
	const bgfx_memory_t* mem = bgfx_alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);
	
	return bgfx_create_shader(mem);
}