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
 * @brief Additional data for processing .obj file.
*/
struct ObjData {
	bool mirrorX;			//!< mirror X axis
	IndicesOrder order;		//!< CW or CCW
	
	void print() {
		printf("mirror x: %s, indices order: %s\n",
			mirrorX ? "true" : "false",
			order == IndicesOrder::Auto ? "Auto" : (order == IndicesOrder::CW ? "Clockwise" : "Counter clockwise"));
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
	bool hasColorData;
	float* colors;
	int normalCount;
	float* normals;
	int texcoordCount;
	float* texcoords;
	
	int triCount;
	uint16_t* indices;
	
	int objectCount;
	Object* objects;
	
	void cleanup() {
		positionCount = 0;
		free(positions);
		hasColorData = false;
		free(colors);
		normalCount = 0;
		free(normals);
		texcoordCount = 0;
		free(texcoords);
		
		triCount = 0;
		free(indices);
		
		objectCount = 0;
		free(objects);
	}
	
	static Mesh create() {
		return {
			0, nullptr,			// position (inited to 0, as it is a must)
			false, nullptr,		// color
			0, nullptr,			// normal
			0, nullptr,			// texcoord
			0, nullptr,			// triCount, indices
			0, nullptr			// object
		};
	}
	
	void print() {
		printf("v: %d, c: %s, vn: %d, vt: %d, tri: %d, o: %d\n",
			positionCount,
			hasColorData ? "true" : "false",
			normalCount,
			texcoordCount,
			triCount,
			objectCount);
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
 * @param _mesh Mesh& to fill.
 * @param _firstTriNormal Vector3& for auto detect indices order (must init with preprocess).
 *
 * @return ErrorCode.
*/
static ErrorCode process(FILE*& _file, ObjData& _data, Mesh& _mesh, const Vector3& _firstTriNormal);

/**
 * @brief Creates a model from mesh data.
 *
 * @param _mesh Mesh&.
 * @param _model Model*& to fill.
 *
 * @return ErrorCode.
*/
static ErrorCode createModel(Mesh& _mesh, Model*& _model);

Model* wfobj_load(const char* _objPath, IndicesOrder _order) {
	printf(AC_YELLOW "[wfobj_load] %s\n" AC_RESET, _objPath);

	FILE* file { nullptr };
	
	ObjData data { false, _order };
	
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
	// for (int i = 0; i < mesh.objectCount; i++) mesh.objects[i].print();
	
	if (process(file, data, mesh, firstTriNormal) != NONE) {
		fclose(file);
		return nullptr;
	}
	
	fclose(file);
	
	Model* model { nullptr };
	if (createModel(mesh, model) != NONE) return nullptr;
	
	mesh.cleanup();

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

	if (strcmp(line, "# Blender") == 0) _data.mirrorX = true;
	
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
				_mesh.positionCount++;
				
				if (checkColor) {
					float a;
					int n = fscanf(_file, "%f %f %f %f %f %f", &a, &a, &a, &a, &a, &a);

					if (n == 6) {
						_mesh.hasColorData = true;
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
				_mesh.normalCount++;
			}
			// texture coordinates
			else if (c == 't') {
				_mesh.texcoordCount++;
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
				if (vCount == 4) _mesh.triCount += 2;
				// tri
				else if (vCount == 3) _mesh.triCount++;
			}
		}
		else if (c == 'o') {
			// object
			if (getc(_file) == ' ') {
				if (objI > 0) {
					objO[objI - 1].endPositionIndex = _mesh.positionCount - 1;
				}
				
				char objectName[WFOBJ_MAX_OBJECT_NAME_LEN];
				int idx = 0;
				while ((c = getc(_file)) != '\n' && c != EOF) objectName[idx++] = c;
				objectName[idx] = '\0';
				
				objO[objI++] = Object::create(objectName, _mesh.positionCount);
				_mesh.objectCount++;
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
			_mesh.objects[objI - 1].endPositionIndex = _mesh.positionCount;
		}
	}
	else _mesh.objectCount = -1;

	if (_mesh.normalCount == 0) _mesh.normalCount = -1;
	if (_mesh.texcoordCount == 0) _mesh.texcoordCount = -1;
	
	_mesh.print();
	
	return NONE;
}

static ErrorCode process(FILE*& _file, ObjData& _data, Mesh& _mesh, const Vector3& _firstTriNormal) {
	_mesh.positions = (float*)malloc(sizeof(float) * _mesh.positionCount * 3);
	if (_mesh.positions == NULL) {
		return err_create(MEM_ALLOC, "Positions");
	}
	
	if (_mesh.hasColorData) {
		_mesh.colors = (float*)malloc(sizeof(float) * _mesh.positionCount * 3);
		if (_mesh.positions == NULL) {
			return err_create(MEM_ALLOC, "Colors");
		}
	}
	
	_mesh.indices = (uint16_t*)malloc(sizeof(uint16_t) * _mesh.triCount * 3);
	if (_mesh.indices == NULL) {
		free(_mesh.positions);
		return err_create(MEM_ALLOC, "Indices");
	}
	
	int vIndex { 0 };
	int iIndex { 0 };
	
	rewind(_file);

	char c;
	while ((c = getc(_file)) != EOF) {
		if (c == 'v') {
			// vertex
			if (getc(_file) == ' ') {
				if (_mesh.hasColorData) {
					float x, y, z;
					float r, g, b;
					int n = fscanf(_file, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
					
					if (n == 6) {
						_mesh.positions[vIndex * 3] = _data.mirrorX ? -x : x;
						_mesh.colors[vIndex * 3] = r;
						_mesh.positions[vIndex * 3 + 1] = y;
						_mesh.colors[vIndex * 3 + 1] = g;
						_mesh.positions[vIndex * 3 + 2] = z;
						_mesh.colors[vIndex * 3 + 2] = b;
					}
					else return err_create(PARSE_FAILED, "Failed to match vertex data.");
				}
				else {
					float x, y, z;
					int n = fscanf(_file, "%f %f %f", &x, &y, &z);

					if (n == 3) {
						_mesh.positions[vIndex * 3] = _data.mirrorX ? -x : x;
						_mesh.positions[vIndex * 3 + 1] = y;
						_mesh.positions[vIndex * 3 + 2] = z;
					}
					else return err_create(PARSE_FAILED, "Failed to match vertex data.");
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
				
				v1 += v1 < 0 ? _mesh.positionCount : -1;
				vt1 += vt1 < 0 ? _mesh.texcoordCount : -1;
				vn1 += vn1 < 0 ? _mesh.normalCount : -1;
				v2 += v2 < 0 ? _mesh.positionCount : -1;
				vt2 += vt2 < 0 ? _mesh.texcoordCount : -1;
				vn2 += vn2 < 0 ? _mesh.normalCount : -1;
				v3 += v3 < 0 ? _mesh.positionCount : -1;
				vt3 += vt3 < 0 ? _mesh.texcoordCount : -1;
				vn3 += vn3 < 0 ? _mesh.normalCount : -1;
				v4 += v4 < 0 ? _mesh.positionCount : -1;
				vt4 += vt4 < 0 ? _mesh.texcoordCount : -1;
				vn4 += vn4 < 0 ? _mesh.normalCount : -1;

				if (_data.order == IndicesOrder::Auto) {
					Vector3 a V3_NEW(_mesh.positions[v1 * 3], _mesh.positions[v1 * 3 + 1], _mesh.positions[v1 * 3 + 2]);
					Vector3 b V3_NEW(_mesh.positions[v2 * 3], _mesh.positions[v2 * 3 + 1], _mesh.positions[v2 * 3 + 2]);
					Vector3 c V3_NEW(_mesh.positions[v3 * 3], _mesh.positions[v3 * 3 + 1], _mesh.positions[v3 * 3 + 2]);
					
					// calculate normal clockwise
					Vector3 normCalculated V3_NORM(V3_CROSS(b - a, c - a));
					// validate if calculated clockwise normal points the same direction as the provided normal
					_data.order = V3_DOT(_firstTriNormal, normCalculated) >= 0.95f ? IndicesOrder::CW : IndicesOrder::CCW;
				}

				if (n == 12) {
					switch (_data.order) {
						case IndicesOrder::CW:
							_mesh.indices[iIndex++] = v3;
							_mesh.indices[iIndex++] = v2;
							_mesh.indices[iIndex++] = v1;
							
							_mesh.indices[iIndex++] = v4;
							_mesh.indices[iIndex++] = v3;
							_mesh.indices[iIndex++] = v1;
							break;
						case IndicesOrder::CCW:
							_mesh.indices[iIndex++] = v1;
							_mesh.indices[iIndex++] = v2;
							_mesh.indices[iIndex++] = v3;
							
							_mesh.indices[iIndex++] = v1;
							_mesh.indices[iIndex++] = v3;
							_mesh.indices[iIndex++] = v4;
							break;
						default: break;
					}
					
				}
				else if (n == 9) {
					switch (_data.order) {
						case IndicesOrder::CW:
							_mesh.indices[iIndex++] = v3;
							_mesh.indices[iIndex++] = v2;
							_mesh.indices[iIndex++] = v1;
							break;
						case IndicesOrder::CCW:
							_mesh.indices[iIndex++] = v1;
							_mesh.indices[iIndex++] = v2;
							_mesh.indices[iIndex++] = v3;
							break;
						default: break;
					}
				}
				else {
					return err_create(PARSE_FAILED, "Failed to match tri/quad data.");
				}
			}
		}
	}

	return NONE;
}

static ErrorCode createModel(Mesh& _mesh, Model*& _model) {
	void* vertices { nullptr };
	VertexType vertexType { VertexType::Basic };
	
	if (_mesh.hasColorData) {
		vertexType = VertexType::Color;
		
		vertices = malloc(sizeof(Vertex_Colored) * _mesh.positionCount);
		if (vertices == NULL) return err_create(MEM_ALLOC, "Vertices");

		Vertex_Colored* p = (Vertex_Colored*)vertices;

		for (int i = 0; i < _mesh.positionCount; i++) {
			p[i] = Vertex_Colored(
				_mesh.positions[i * 3], _mesh.positions[i * 3 + 1], _mesh.positions[i * 3 + 2],
				rgbToHex(_mesh.colors[i * 3], _mesh.colors[i * 3 + 1], _mesh.colors[i * 3 + 2]));
		}
	}
	// fallback to pink
	else {
		vertexType = VertexType::Color;
		
		vertices = malloc(sizeof(Vertex_Colored) * _mesh.positionCount);
		if (vertices == NULL) return err_create(MEM_ALLOC, "Vertices");

		Vertex_Colored* p = (Vertex_Colored*)vertices;

		for (int i = 0; i < _mesh.positionCount; i++) {
			p[i] = Vertex_Colored(_mesh.positions[i * 3], _mesh.positions[i * 3 + 1], _mesh.positions[i * 3 + 2], 0xff7f00ff);
		}
	}
	
	_model = Model::create(vertexType, vertices, _mesh.positionCount, _mesh.indices, _mesh.triCount * 3);
	if (_model == nullptr) {
		// TODO
		return (ErrorCode)404;
	}
	
	return NONE;
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
