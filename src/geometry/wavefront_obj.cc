#include "wavefront_obj.hh"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../sys/err.h"

#include "../math/math.h"
#include "../core/vertex.hh"

#include "../utils/color.h"
#include "../utils/consc.h"
#include "../utils/file.h"

/**
 * @brief Enum flag for storing what data the .obj holds.
*/
enum ObjDataType {
	INVALID		= 0,
	POS			= 1 << 0,		//!< Position
	COLOR		= 1 << 1,		//!< Color
	NORM		= 1 << 2,		//!< Normal
	TEX			= 1 << 3,		//!< Texture coordinate
	BLENDER		= 1 << 4		//!< Is Blender produced
};

/**
 * @brief Prescanned data of .obj.
 *
 * What type of data the .obj holds and counts
 * for able to allocate memory.
*/
struct ObjData {
	int typeMask;			//!< ObjDataType "enum flag"
	int positionCount;		//!< Number of vertex positions
	int normalCount;		//!< Number of normals
	int texcoordCount;		//!< Number of texture coordinates
	int triCount;			//!< Number of triangles
	int objectCount;		//!< Number of objects
	IndicesOrder order;		//!< CW or CCW
	
	void print() {
		char dataType[35];
		char strs[][11] { "POS", " & COLOR", " & NORM", " & TEX", " & BLENDER" };
		char* p = dataType;
		
		for (int i = 0; i < 5; i++) {
			if ((typeMask & 1 << i) == 0) continue;

			size_t len = strlen(strs[i]);
			memcpy(p, strs[i], len);
			p += len;
		}
		*p = '\0';
		
		if (typeMask == INVALID) memcpy(dataType, "INVALID", 8);
		
		printf("v: %d, vn: %d, vt: %d, tris: %d, objects: %d, type: %s\n",
			positionCount,
			normalCount,
			texcoordCount,
			triCount,
			objectCount,
			dataType);
	}
};

struct Object {
	char* name;
	int startPositionIndex;
	int endPositionIndex;
	
	static Object create(const char* _name, int _startPosition) {
		size_t len = strlen(_name) + 1;
		char* n = (char*)malloc(sizeof(char) * len);
		if (n == NULL) {
			// TODO
			return { NULL, 0, 0 };
		}
		
		memcpy(n, _name, len);

		return { n, _startPosition, 0 };
	}
	
	void print() {
		printf("name: %s, start: %d, end: %d\n", name, startPositionIndex, endPositionIndex);
	}
	
	void cleanup() {
		free(name);
	}
};

struct Mesh {
	int positionCount;
	float* positions;
	int normalCount;
	float* normals;
	int texcoordCount;
	float* texcoords;
	float* colors;
	int objectCount;
	Object* objects;
	
	void cleanup() {
		free(positions);
		free(normals);
		free(texcoords);
		free(colors);
		free(objects);
	}
	
	static Mesh create() {
		return { 0, nullptr, 0, nullptr, 0, nullptr, nullptr, 0, nullptr };
	}
};

/**
 * @brief Opens file and reads header info.
 *
 * @param _path Path to the file.
 * @param _file nullptr on fail, otherwise FILE* pointer.
 * @param _data ObjData&.
 *
 * @return ErrorCode.
*/
static ErrorCode readFileAndHeader(const char* _path, FILE*& _file, ObjData& _data);

/**
 * @brief Scans file for obj data.
 *
 * Count all vertices, texcoords, normals, faces and save them in the ObjData struct.
 *
 * @param _file FILE* to the source file.
 * @param _data ObjData& to fill.
 * @param _mesh Mesh&.
 * @param _firstTriNormal A Vector3& for auto detect indices order.
 *
 * @return ErrorCode.
*/
static ErrorCode preprocess(FILE*& _file, ObjData& _data, Mesh& _mesh, Vector3& _firstTriNormal);

/**
 * @brief Creates model from obj.
 *
 * @param _file FILE* to the source file.
 * @param _data ObjData& (must init with preprocess).
 * @param _firstTriNormal Vector3& for auto detect indices order (must init with preprocess).
 *
 * @return nullptr on error, otherwise the Model*.
*/
static Model* process(FILE*& _file, ObjData& _data, const Vector3& _firstTriNormal);

Model* wfobj_load(const char* _objPath, IndicesOrder _order) {
	printf(AC_YELLOW "[wfobj_load] %s\n" AC_RESET, _objPath);

	FILE* file { nullptr };
	
	ObjData data { POS, 0, 0, 0, 0, 0, _order };
	
	if (readFileAndHeader(_objPath, file, data) != NONE) {
		if (file != nullptr) fclose(file);
		return nullptr;
	}

	Mesh mesh { Mesh::create() };
	Vector3 firstTriNormal V3_ZERO;

	if (preprocess(file, data, mesh, firstTriNormal) != NONE) {
		fclose(file);
		return nullptr;
	}
	
	// TEST
	for (int i = 0; i < mesh.objectCount; i++) mesh.objects[i].print();
	
	Model* model = process(file, data, firstTriNormal);

	mesh.cleanup();
	fclose(file);

	return model;
}

static ErrorCode readFileAndHeader(const char* _path, FILE*& _file, ObjData& _data) {
	const char* ext = _path + strlen(_path) - 4;
	if (strcmp(ext, ".obj") != 0 && strcmp(ext, ".OBJ") != 0) {
		return err_create(EXT_MISMATCH, "%s is not Wavefront OBJ.\n", getFileName(_path));
	}
	
	_file = fopen(_path, "r");
	if (_file == nullptr) {
		return err_create(OPEN_FILE, "file is NULL: %s\n", getFileName(_path));
	}
	
	char buf[10];
	char* line = fgets(buf, 10, _file);
	if (line == nullptr) {
		return err_create(NO_CONTENT, NULL);
	}

	if (strcmp(line, "# Blender") == 0) _data.typeMask |= BLENDER;
	
	return NONE;
}

static ErrorCode preprocess(FILE*& _file, ObjData& _data, Mesh& _mesh, Vector3& _firstTriNormal) {
	char c;
	bool detectOrder = _data.order == IndicesOrder::Auto;
	bool checkColor = true;
	
	int objI = 0;
	Object objO[WFOBJ_MAX_OBJECT_COUNT];
	
	while ((c = getc(_file)) != EOF) {
		if (c == 'v') {
			// vertex
			if ((c = getc(_file)) == ' ') {
				_data.positionCount++;
				
				if (checkColor) {
					float a;
					int n = fscanf(_file, "%f %f %f %f %f %f", &a, &a, &a, &a, &a, &a);

					if (n == 6) {
						_data.typeMask |= COLOR;
					}
					else if (n != 3) {
						fclose(_file);
						return err_create(PARSE_FAILED, "Failed to match vertex data.");
					}
					checkColor = false;
					// fscanf moved the file pointer, so we don't need the while
					continue;
				}
				
				while ((c = getc(_file)) != '\n' && c != EOF) ;
			}
			// vertex normal
			else if (c == 'n') {
				if (detectOrder) {
					int n = fscanf(_file, "%f %f %f", &_firstTriNormal.X, &_firstTriNormal.Y, &_firstTriNormal.Z);

					if (n != 3) {
						fclose(_file);
						return err_create(PARSE_FAILED, "Failed to match vertex normal data.");
					}
					detectOrder = false;
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
		else if (c == 'o') {
			// object
			if (getc(_file) == ' ') {
				if (objI > 0) {
					objO[objI - 1].endPositionIndex = _data.positionCount - 1;
				}
				
				char objectName[WFOBJ_MAX_OBJECT_NAME_LEN];
				int idx = 0;
				while ((c = getc(_file)) != '\n' && c != EOF) objectName[idx++] = c;
				objectName[idx] = '\0';
				
				objO[objI++] = Object::create(objectName, _data.positionCount);
				_data.objectCount++;
			}
		}
	}
	
	if (_data.order == IndicesOrder::Auto) {
		if (_firstTriNormal == V3_ZERO) {
			return err_create(PARSE_FAILED, "Failed to retrive vertex normal.");
		}
	}

	if (objI > 0) {
		_mesh.objectCount = objI;
		_mesh.objects = (Object*)malloc(sizeof(Object) * objI);
		if (_mesh.objects == NULL) {
			printf(AC_RED "Failed to allocate memory for objects." AC_RESET);
			// TODO: now what?
		}
		else {
			for (int i = 0; i < objI; i++) _mesh.objects[i] = objO[i];
			_mesh.objects[objI - 1].endPositionIndex = _data.positionCount;
		}
	}

	if (_data.normalCount != 0) _data.typeMask |= NORM;
	if (_data.texcoordCount != 0) _data.typeMask |= TEX;
	
	return NONE;
}

static Model* process(FILE*& _file, ObjData& _data, const Vector3& _firstTriNormal) {
	Model* model { nullptr };
	
	Vertex_Colored* vertices { (Vertex_Colored*)malloc(sizeof(Vertex_Colored) * _data.positionCount) };
	if (vertices == NULL) {
		puts(AC_RED "Failed to allocate memory." AC_RESET);
		return nullptr;
	}
	
	uint16_t* indices { (uint16_t*)malloc(sizeof(uint16_t) * _data.triCount * 3) };
	if (indices == NULL) {
		puts(AC_RED "Failed to allocate memory." AC_RESET);
		free(vertices);
		return nullptr;
	}
	
	int vIndex { 0 };
	int iIndex { 0 };
	
	rewind(_file);

	char c;
	while ((c = getc(_file)) != EOF) {
		if (c == 'v') {
			// vertex
			if (getc(_file) == ' ') {
				if ((_data.typeMask & COLOR) != 0) {
					float x, y, z;
					float r, g, b;
					int n = fscanf(_file, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
					
					if (n == 6) {
						vertices[vIndex] = Vertex_Colored((_data.typeMask & BLENDER) == 0 ? x : -x, y, z, rgbToHex(r, g, b));
					}
					else {
						printf(AC_RED "Failed to match vertex data." AC_RESET);
						goto end;
					}
				}
				else {
					float x, y, z;
					int n = fscanf(_file, "%f %f %f", &x, &y, &z);

					if (n == 3) {
						vertices[vIndex] = Vertex_Colored((_data.typeMask & BLENDER) == 0 ? x : -x, y, z, 0xff7f00ff);
					}
					else {
						printf(AC_RED "Failed to match vertex data." AC_RESET);
						goto end;
					}
				}
				
				vIndex++;
			}
		}
		else if (c == 'f') {
			// face
			if (getc(_file) == ' ') {
				int
					v1 { 0 }, vt1 { 0 }, vn1 { 0 },
					v2 { 0 }, vt2 { 0 }, vn2 { 0 },
					v3 { 0 }, vt3 { 0 }, vn3 { 0 },
					v4 { 0 }, vt4 { 0 }, vn4 { 0 };

				int n = fscanf(_file,
					"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
					&v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4);
				
				v1 += v1 < 0 ? _data.positionCount : -1;
				vt1 += vt1 < 0 ? _data.texcoordCount : -1;
				vn1 += vn1 < 0 ? _data.normalCount : -1;
				v2 += v2 < 0 ? _data.positionCount : -1;
				vt2 += vt2 < 0 ? _data.texcoordCount : -1;
				vn2 += vn2 < 0 ? _data.normalCount : -1;
				v3 += v3 < 0 ? _data.positionCount : -1;
				vt3 += vt3 < 0 ? _data.texcoordCount : -1;
				vn3 += vn3 < 0 ? _data.normalCount : -1;
				v4 += v4 < 0 ? _data.positionCount : -1;
				vt4 += vt4 < 0 ? _data.texcoordCount : -1;
				vn4 += vn4 < 0 ? _data.normalCount : -1;

				if (_data.order == IndicesOrder::Auto) {
					Vector3 a V3_NEW(vertices[v1].x, vertices[v1].y, vertices[v1].z);
					Vector3 b V3_NEW(vertices[v2].x, vertices[v2].y, vertices[v2].z);
					Vector3 c V3_NEW(vertices[v3].x, vertices[v3].y, vertices[v3].z);
					
					// calculate normal clockwise
					Vector3 normCalculated V3_NORM(V3_CROSS(b - a, c - a));
					// validate if calculated clockwise normal points the same direction as the provided normal
					_data.order = V3_DOT(_firstTriNormal, normCalculated) >= 0.95f ? IndicesOrder::CW : IndicesOrder::CCW;
				}

				if (n == 12) {
					switch (_data.order) {
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
					switch (_data.order) {
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

	model = ModelManager::create(vertices, _data.positionCount, indices, _data.triCount * 3, Vertex_Colored::layout);

end:
	free(vertices);
	free(indices);

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
