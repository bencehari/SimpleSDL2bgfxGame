#include "loaders.hh"

#include <string.h>

#include "../utils/file.h" // <stdio.h>

#include "../utils/consc.h"

ErrorCode loadShader(const char* _filename, bgfx::ShaderHandle* _shaderHandle) {
	const char* shaderPath { nullptr };
	
	switch (bgfx::getRendererType()) {
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Agc:			shaderPath = "assets/shaders/dx9/"; break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12:	shaderPath = "assets/shaders/dx11/"; break;
		case bgfx::RendererType::Gnm:			shaderPath = "assets/shaders/pssl/"; break;
		case bgfx::RendererType::Metal:			shaderPath = "assets/shaders/metal/"; break;
		case bgfx::RendererType::Nvn:			break;
		case bgfx::RendererType::OpenGLES:		shaderPath = "assets/shaders/essl"; break;
		case bgfx::RendererType::OpenGL:		shaderPath = "assets/shaders/glsl/"; break;
		case bgfx::RendererType::Vulkan:		shaderPath = "assets/shaders/spirv/"; break;
		case bgfx::RendererType::Count:			break;
	}
	
	// printf(AC_YELLOW "ShaderName: '%s'.\nPath: '%s'\n" AC_RESET, _filename, shaderPath);
	
	size_t shaderPathLen { strlen(shaderPath) };
	size_t fileNameLen { strlen(_filename) };
	
	char* filePath { (char*)malloc(shaderPathLen + fileNameLen + 1) };
	if (filePath == NULL) return err_create(MEM_ALLOC, "loaders.cc:loadShader");
	
	memcpy(filePath, shaderPath, shaderPathLen);
	memcpy(&filePath[shaderPathLen], _filename, fileNameLen);
	filePath[shaderPathLen + fileNameLen] = '\0';
	
	// printf(AC_YELLOW "%s\n" AC_RESET, filePath);
	
	FILE* file { nullptr };
	long fileSize { 0 };
	if (!getFileSize(filePath, &fileSize, &file)) return err_create(OPEN_FILE, "loaders.cc:loadShader");
	
	// printf(AC_YELLOW "fileSize: %ld\n" AC_RESET, fileSize);
	
	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);
	
	bgfx::ShaderHandle h = bgfx::createShader(mem);
	
	*_shaderHandle = h;
	
	return NONE;
}
