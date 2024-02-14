#include "loaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/vertex.h"

#include "../utils/file.h"
#include "../utils/consc.h"

// resource intensive loading of .obj file
// TODO: pack all assets before build for runtime use!
bool load_external_obj_geometry(const char* _objPath, const bgfx::VertexLayout* _vertexLayout, Model** _model, enum IndicesOrder _order) {
	// TODO: handle multi-object .obj
	
	FILE* file = fopen(_objPath, "r");
	if (file == nullptr) {
		printf(AC_RED "file is NULL: %s\n" AC_RESET, _objPath);
		return false;
	}
	
	bool result = true;
	
	char c;
	int vert = 0;
	int tris = 0;
	
	bool detectOrder = _order == INDICES_ORDER_AUTO ? true : false;
	Vector3 normal = V3_ZERO;
	
	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			if ((c = getc(file)) == ' ') {
				vert++;
				while ((c = getc(file)) != '\n' && c != EOF) ;
			}
			else if (detectOrder && c == 'n') {
				int n = fscanf(file, "%f %f %f", &normal.X, &normal.Y, &normal.Z);
				
				if (n != 3) {
					printf(AC_RED "Failed to match vertex normal data." AC_RESET);
					result = false;
					goto close;
				}
				detectOrder = false;
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
	
	{
		Vertex_PosColor* vertices = (Vertex_PosColor*)malloc(sizeof(struct Vertex_PosColor) * vert);
		uint16_t* indices = (uint16_t*)malloc(sizeof(uint16_t) * tris * 3);
		
		int vIndex = 0;
		int iIndex = 0;
		
		rewind(file);
		
		if (_order == INDICES_ORDER_AUTO) {
			if (normal == V3_ZERO) {
				printf(AC_RED "Failed to retrive vertex normal." AC_RESET);
				result = false;
				goto cleanup;
			}
		}
		
		while ((c = getc(file)) != EOF) {
			if (c == 'v') {
				if (getc(file) == ' ') {
					float x, y, z;
					int n = fscanf(file, "%f %f %f", &x, &y, &z);
					
					if (n != 3) {
						printf(AC_RED "Failed to match vertex data." AC_RESET);
						result = false;
						goto cleanup;
					}
					else {
						vertices[vIndex] = Vertex_PosColor(x, y, z, vIndex % 2 == 0 ? 0xffffffff : (vIndex % 3 == 0 ? 0xff0000ff : 0xff00ff00));
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
					
					// we store indexes 0 based
					v1--; vt1--; vn1--;
					v2--; vt2--; vn2--;
					v3--; vt3--; vn3--;
					v4--; vt4--; vn4--;

					if (_order == INDICES_ORDER_AUTO) {
						Vector3 a = V3_NEW(vertices[v1].x, vertices[v1].y, vertices[v1].z);
						Vector3 b = V3_NEW(vertices[v2].x, vertices[v2].y, vertices[v2].z);
						Vector3 c = V3_NEW(vertices[v3].x, vertices[v3].y, vertices[v3].z);
						
						// calculate normal clockwise
						Vector3 normCalculated = V3_NORM(V3_CROSS(b - a, c - a));
						
						// hopefully that will be enough.
						// EDIT: changed to 0.95 from 0.99
						// theoretically dot > 0.0f would be enough, but it is strange that even 0.975 happened, maybe should investigate this
						_order = V3_DOT(normal, normCalculated) >= 0.95f ? INDICES_ORDER_CLOCKWISE : INDICES_ORDER_COUNTERCLOCKWISE;
						
						/*printf(AC_YELLOW "determine indices order for \"%s\"\n" AC_RESET
							"           normal: %f %f %f\nnormal calculated: %f %f %f\nDOT: %f\n",
							_objPath, normal.X, normal.Y, normal.Z, normCalculated.X, normCalculated.Y, normCalculated.Z, V3_DOT(normal, normCalculated));*/
					}

					if (n == 12) {
						// Blender arranges indices clockwise, while bgfx processes counterclockwise
						if (_order == INDICES_ORDER_CLOCKWISE) {
							indices[iIndex++] = v3;
							indices[iIndex++] = v2;
							indices[iIndex++] = v1;
							
							indices[iIndex++] = v4;
							indices[iIndex++] = v3;
							indices[iIndex++] = v1;
						}
						else {
							indices[iIndex++] = v1;
							indices[iIndex++] = v2;
							indices[iIndex++] = v3;
							
							indices[iIndex++] = v1;
							indices[iIndex++] = v3;
							indices[iIndex++] = v4;
						}
						
					}
					else if (n == 9) {
						if (_order == INDICES_ORDER_CLOCKWISE) {
							indices[iIndex++] = v3;
							indices[iIndex++] = v2;
							indices[iIndex++] = v1;
						}
						else {
							indices[iIndex++] = v1;
							indices[iIndex++] = v2;
							indices[iIndex++] = v3;
						}
					}
					else {
						printf(AC_RED "Failed to match tri/quad data." AC_RESET);
						result = false;
						goto cleanup;
					}
				}
			}
		}
		
		/*puts(AC_YELLOW "VERTICES" AC_RESET);
		for (int i = 0; i < vert; i++) printf("%d:\t%f %f %f\n", i + 1, vertices[i].x, vertices[i].y, vertices[i].z);
		puts(AC_YELLOW "INDICES" AC_RESET);
		for (int i = 1, j = 2; j < tris * 3; i++, j += 3) printf("%d:\t%d %d %d\n", i, indices[j - 2], indices[j - 1], indices[j]);*/
		
		*_model = createModel(vertices, vert, indices, tris * 3, *_vertexLayout);

cleanup:
		free(vertices);
		free(indices);
	}
	
close:
	fclose(file);

	return result;
}

bool load_shader(const char* _filename, bgfx::ShaderHandle* _shaderHandle) {
	const char* shaderPath;
	
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
	
	size_t shaderPathLen = strlen(shaderPath);
	size_t fileNameLen = strlen(_filename);
	
	char* filePath = (char*)malloc(shaderPathLen + fileNameLen + 1);
	memcpy(filePath, shaderPath, shaderPathLen);
	memcpy(&filePath[shaderPathLen], _filename, fileNameLen);
	filePath[shaderPathLen + fileNameLen] = '\0';
	
	// printf(AC_YELLOW "%s\n" AC_RESET, filePath);
	
	FILE* file = nullptr;
	long fileSize = 0;
	if (!file_get_size(filePath, &fileSize, &file)) return false;
	
	// printf(AC_YELLOW "fileSize: %ld\n" AC_RESET, fileSize);
	
	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);
	
	bgfx::ShaderHandle h = bgfx::createShader(mem);
	
	*_shaderHandle = h;
	
	return true;
}
