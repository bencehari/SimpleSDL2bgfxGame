#include "wavefront_obj.hh"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../math/math.h"
#include "../core/vertex.hh"

#include "../utils/color.h"
#include "../utils/consc.h"

struct ObjData {
	int positionCount;
	int texcoordCount;
	int normalCount;
	int triCount;
	IndicesOrder order;
	
	void print() {
		printf("v: %d, vt: %d, vn: %d, tris: %d\n", positionCount, texcoordCount, normalCount, triCount);
	}
};

static bool readFileAndHeader(const char* _path, FILE*& _file, bool& _flipX) {
	_file = fopen(_path, "r");
	if (_file == nullptr) {
		printf(AC_RED "file is NULL: %s\n" AC_RESET, _path);
		return false;
	}
	
	char buf[10];
	char* line = fgets(buf, 10, _file);
	if (line == nullptr) {
		fclose(_file);
		return false;
	}

	if (strcmp(line, "# Blender") == 0) _flipX = true;
	
	return true;
}

static bool preprocess(FILE*& _file, ObjData& _data, Vector3& _normal) {
	char c;
	bool shouldDetectOrder = _data.order == IndicesOrder::Auto;
	
	// TODO: handle face layouts, e.g.:
	// "f v1 v2 v3", "f v1/vt1 v2/vt2 v3/vt3",
	// "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3", "f v1//vn1 v2//vn2 v3//vn3"
	
	while ((c = getc(_file)) != EOF) {
		if (c == 'v') {
			// vertex
			if ((c = getc(_file)) == ' ') {
				_data.positionCount++;
				while ((c = getc(_file)) != '\n' && c != EOF) ;
			}
			// vertex normal
			else if (c == 'n') {
				if (shouldDetectOrder) {
					int n = fscanf(_file, "%f %f %f", &_normal.X, &_normal.Y, &_normal.Z);

					if (n != 3) {
						printf(AC_RED "Failed to match vertex normal data." AC_RESET);

						fclose(_file);
						return false;
					}
					shouldDetectOrder = false;
				}
				_data.normalCount++;
			}
		}
		else if (c == 'f') {
			// face
			if (getc(_file) == ' ') {
				int vCount { 1 };
				while ((c = getc(_file)) != '\n' && c != EOF) {
					if (c == ' ') vCount++;
				}
				// quad
				if (vCount == 4) _data.triCount += 2;
				// tri
				else if (vCount == 3) _data.triCount++;
			}
		}
	}
	
	if (_data.order == IndicesOrder::Auto) {
		if (_normal == V3_ZERO) {
			printf(AC_RED "Failed to retrive vertex normal." AC_RESET);

			fclose(_file);
			return false;
		}
	}
	
	// _data.print();
	
	return true;
}

Model* wfobj_load(const char* _objPath, IndicesOrder _order) {
	FILE* file { nullptr };
	bool flipX { false };
	
	if (!readFileAndHeader(_objPath, file, flipX)) return nullptr;

	ObjData data { 0, 0, 0, 0, _order };
	Vector3 normal V3_ZERO;

	if (!preprocess(file, data, normal)) return nullptr;
	
	Model* model { nullptr };
	
	Vertex_Colored* vertices { (Vertex_Colored*)malloc(sizeof(Vertex_Colored) * data.positionCount) };
	uint16_t* indices { (uint16_t*)malloc(sizeof(uint16_t) * data.triCount * 3) };
	
	int vIndex { 0 };
	int iIndex { 0 };
	
	rewind(file);

	char c;
	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			// vertex
			if (getc(file) == ' ') {
				float x, y, z;
				float r, g, b;
				int n = fscanf(file, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				
				if (n == 3) {
					vertices[vIndex] = Vertex_Colored(!flipX ? x : -x, y, z, 0xff7f00ff);
				}
				else if (n == 6) {
					vertices[vIndex] = Vertex_Colored(!flipX ? x : -x, y, z, rgbToHex(r, g, b));
				}
				else {
					printf(AC_RED "Failed to match vertex data." AC_RESET);
					goto end;
				}
				vIndex++;
			}
		}
		else if (c == 'f') {
			// face
			if (getc(file) == ' ') {
				int
					v1 { -1 }, vt1 { -1 }, vn1 { -1 },
					v2 { -1 }, vt2 { -1 }, vn2 { -1 },
					v3 { -1 }, vt3 { -1 }, vn3 { -1 },
					v4 { -1 }, vt4 { -1 }, vn4 { -1 };

				int n = fscanf(file,
					"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
					&v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4);
				
				// TODO: handle negative vertex index
				
				// we store indexes 0 based
				v1--; vt1--; vn1--;
				v2--; vt2--; vn2--;
				v3--; vt3--; vn3--;
				v4--; vt4--; vn4--;

				if (_order == IndicesOrder::Auto) {
					Vector3 a V3_NEW(vertices[v1].x, vertices[v1].y, vertices[v1].z);
					Vector3 b V3_NEW(vertices[v2].x, vertices[v2].y, vertices[v2].z);
					Vector3 c V3_NEW(vertices[v3].x, vertices[v3].y, vertices[v3].z);
					
					// calculate normal clockwise
					Vector3 normCalculated V3_NORM(V3_CROSS(b - a, c - a));
					
					// hopefully that will be enough.
					// EDIT: changed to 0.95 from 0.99
					// theoretically dot > 0.0f would be enough, but it is strange that even 0.975 happened, maybe should investigate this
					data.order = _order = V3_DOT(normal, normCalculated) >= 0.95f ? IndicesOrder::CW : IndicesOrder::CCW;
					
					/*printf(AC_YELLOW "determine indices order for \"%s\"\n" AC_RESET
						"           normal: %f %f %f\nnormal calculated: %f %f %f\nDOT: %f\n",
						_objPath, normal.X, normal.Y, normal.Z, normCalculated.X, normCalculated.Y, normCalculated.Z, V3_DOT(normal, normCalculated));*/
				}

				if (n == 12) {
					// Blender arranges indices clockwise, while bgfx processes counterclockwise
					switch (_order) {
						case IndicesOrder::CW:
							indices[iIndex++] = v3;
							indices[iIndex++] = v2;
							indices[iIndex++] = v1;
							
							indices[iIndex++] = v4;
							indices[iIndex++] = v3;
							indices[iIndex++] = v1;
							break;
						case IndicesOrder::CCW:
							indices[iIndex++] = v1;
							indices[iIndex++] = v2;
							indices[iIndex++] = v3;
							
							indices[iIndex++] = v1;
							indices[iIndex++] = v3;
							indices[iIndex++] = v4;
							break;
						default: break;
					}
					
				}
				else if (n == 9) {
					switch (_order) {
						case IndicesOrder::CW:
							indices[iIndex++] = v3;
							indices[iIndex++] = v2;
							indices[iIndex++] = v1;
							break;
						case IndicesOrder::CCW:
							indices[iIndex++] = v1;
							indices[iIndex++] = v2;
							indices[iIndex++] = v3;
							break;
						default: break;
					}
				}
				else {
					printf(AC_RED "Failed to match tri/quad data." AC_RESET);
					goto end;
				}
			}
		}
	}
	
	/*puts(AC_YELLOW "VERTICES" AC_RESET);
	for (int i = 0; i < vert; i++) printf("%d:\t%f %f %f\n", i + 1, vertices[i].x, vertices[i].y, vertices[i].z);
	puts(AC_YELLOW "INDICES" AC_RESET);
	for (int i = 1, j = 2; j < tris * 3; i++, j += 3) printf("%d:\t%d %d %d\n", i, indices[j - 2], indices[j - 1], indices[j]);*/
	
	model = ModelManager::create(vertices, data.positionCount, indices, data.triCount * 3, Vertex_Colored::layout);

end:
	free(vertices);
	free(indices);

	fclose(file);

	return model;
}

/*
// quickly copied from bgfx_utils.h to make wfobj_loadTextured work
// recursively started from https://github.com/bkaradzic/bgfx/blob/c75b9cb57d8943a12dbd5399b1ace5c27c6a6c67/examples/common/bgfx_utils.h#L56

#define TRUNC_F(_f) (float(int((_f))))
#define FRACT_F(_f) ((_f) - TRUNC_F((_f)))
#define FLOOR_F(_f) ( (_f) < 0.0f ? -FRACT_F(-(_f)) - float(0.0f != FRACT_F(-(_f))) : (_f) - FRACT_F((_f)) )
#define ROUND_F(_f) (FLOOR_F((_f) + 0.5f))
#define MIN_F(_fa, _fb) ((_fa) < (_fb) ? (_fa) : (_fb))
#define MAX_F(_fa, _fb) ((_fa) > (_fb) ? (_fa) : (_fb))
#define CLAMP_F(_f, _fmin, _fmax) (MAX_F(MIN_F((_f), (_fmax)), (_fmin)))
#define TO_UNORM(_fvalue, _fscale) (uint32_t(ROUND_F(CLAMP_F((_fvalue), 0.0f, 1.0f) * (_fscale))))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void packRGBA8(void* _dst, const float* _src) {
	uint8_t* dst = (uint8_t*)_dst;
	dst[0] = uint8_t(TO_UNORM(_src[0], 255.0f));
	dst[1] = uint8_t(TO_UNORM(_src[1], 255.0f));
	dst[2] = uint8_t(TO_UNORM(_src[2], 255.0f));
	dst[3] = uint8_t(TO_UNORM(_src[3], 255.0f));
}

static uint32_t encodeNormalRGBA8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f) {
	const float src[] = {
		_x * 0.5f + 0.5f,
		_y * 0.5f + 0.5f,
		_z * 0.5f + 0.5f,
		_w * 0.5f + 0.5f
	};
	uint32_t dst;
	packRGBA8(&dst, src);
	return dst;
}

#pragma GCC diagnostic pop
*/
