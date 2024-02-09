#include "loaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/vertex.h"

#include "../utils/file.h"
#include "../utils/consc.h"

// more resource intensive loading of OBJ file
// TODO: pack all assets before build for runtime use!
bool load_external_obj_model(const char* _objPath, const bgfx_vertex_layout_t* _vertexLayout, struct Model** _model) {
	FILE* file = fopen(_objPath, "r");
	if (file == NULL) {
		printf(AC_RED "file is NULL: %s\n" AC_RESET, _objPath);
		return false;
	}
	
	bool result = true;
	
	char c;
	int vert = 0;
	int tris = 0;
	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			if (getc(file) == ' ') {
				vert++;
				while ((c = getc(file)) != '\n' && c != EOF) ;
			}
		}
		else if (c == 'f') {
			if (getc(file) == ' ') {
				int vCount = 1;
				while ((c = getc(file)) != '\n' && c != EOF) {
					if (c == ' ') vCount++;
				}
				if (vCount == 4) tris += 2;
				else if (vCount == 3) tris++;
			}
		}
	}
	
	// printf("\"%s\"\nvertex count: %d\ntri count: %d\n", _objPath, vert, tris);
	
	struct Vertex vertices[vert];
	uint16_t indices[tris * 3];
	
	int vIndex = 0;
	int iIndex = 0;
	
	rewind(file);
	
	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			if (getc(file) == ' ') {
				float x, y, z;
				int n = fscanf(file, "%f %f %f", &x, &y, &z);
				
				if (n != 3) {
					printf(AC_RED "Failed to match vertex data." AC_RESET);
					result = false;
					goto end;
				}
				else {
					vertices[vIndex] = VERTEX_NEW(x, y, z, vIndex % 2 == 0 ? 0xffffffff : (vIndex % 3 == 0 ? 0xff0000ff : 0xff00ff00));
				}
				vIndex++;
			}
		}
		else if (c == 'f') {
			if (getc(file) == ' ') {
				int
					v1 = -1, vt1 = -1, vn1 = -1,
					v2 = -1, vt2 = -1, vn2 = -1,
					v3 = -1, vt3 = -1, vn3 = -1,
					v4 = -1, vt4 = -1, vn4 = -1;

				int n = fscanf(file,
					"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
					&v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4);

				if (n == 12) {
					// Blender arranges indices clockwise,
					// while bgfx processes counterclockwise
					indices[iIndex++] = v3 - 1;
					indices[iIndex++] = v2 - 1;
					indices[iIndex++] = v1 - 1;
					
					indices[iIndex++] = v4 - 1;
					indices[iIndex++] = v3 - 1;
					indices[iIndex++] = v1 - 1;
				}
				else if (n == 9) {
					indices[iIndex++] = v3 - 1;
					indices[iIndex++] = v2 - 1;
					indices[iIndex++] = v1 - 1;
				}
				else {
					printf(AC_RED "Failed to match tri/quad data." AC_RESET);
					result = false;
					goto end;
				}
			}
		}
	}
	
	/*puts(AC_YELLOW "VERTICES" AC_RESET);
	for (int i = 0; i < vert; i++) printf("%d:\t%f %f %f\n", i + 1, vertices[i].x, vertices[i].y, vertices[i].z);
	puts(AC_YELLOW "INDICES" AC_RESET);
	for (int i = 1, j = 2; j < tris * 3; i++, j += 3) printf("%d:\t%d %d %d\n", i, indices[j - 2], indices[j - 1], indices[j]);*/
	
	*_model = model_create(vertices, vert, indices, tris * 3, _vertexLayout);
	
end:
	
	fclose(file);

	return result;
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
