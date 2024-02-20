#include "object.hh"

#include <stdio.h>

Object::Object(Model* _model, bgfx::ProgramHandle _program, Transform _transform) :
		model(_model),
		program(_program),
		transform(_transform) {}

Object* Object::create(Model* _model, bgfx::ProgramHandle _program, Transform _transform) {
	return ObjectManager::create(_model, _program, _transform);
}

void Object::render(void) {
	Matrix4x4 t M4x4_POS(transform.position.X, transform.position.Y, transform.position.Z);
	Matrix4x4 r Q_TO_M4x4(transform.rotation);
	
	bgfx::setTransform((t * r).Elements, 1);
		
	bgfx::setVertexBuffer(0, model->vertexBufferHnd, 0, model->verticesLen);
	bgfx::setIndexBuffer(model->indexBufferHnd, 0, model->indicesLen);
	
	bgfx::submit(0, program, 0, BGFX_DISCARD_ALL);
}

namespace ObjectManager {
	static bool initialized;

	static int maxObjectCount;
	static int currentIndex;
	static Object* objects;

	void init(int _maxObjectCount) {
		if (initialized) {
			puts("Already initialized.");
			return;
		}
		
		maxObjectCount = _maxObjectCount;
		objects = (Object*)malloc(sizeof(Object) * _maxObjectCount);
		
		initialized = true;
	}

	void cleanup(void) {
		free(objects);
		
		objects = nullptr;
		currentIndex = 0;
		initialized = false;
	}
	
	Object* create(Model* _model, bgfx::ProgramHandle _program, Transform _transform) {
		if (currentIndex >= maxObjectCount) return nullptr;
		
		objects[currentIndex] = Object(_model, _program, _transform);
		return &objects[currentIndex++];
	}
	
	Object* createTestCube(bgfx::ProgramHandle _programHandle, Transform _transform) {
		if (currentIndex >= maxObjectCount) {
			puts("Max Object count reached.");
			return nullptr;
		}
		
		Vertex_Colored vertices[] = {
			Vertex_Colored(-1.0f,  1.0f,  1.0f, 0xff000000),
			Vertex_Colored( 1.0f,  1.0f,  1.0f, 0xff0000ff),
			Vertex_Colored(-1.0f, -1.0f,  1.0f, 0xff00ff00),
			Vertex_Colored( 1.0f, -1.0f,  1.0f, 0xff00ffff),
			Vertex_Colored(-1.0f,  1.0f, -1.0f, 0xffff0000),
			Vertex_Colored( 1.0f,  1.0f, -1.0f, 0xffff00ff),
			Vertex_Colored(-1.0f, -1.0f, -1.0f, 0xffffff00),
			Vertex_Colored( 1.0f, -1.0f, -1.0f, 0xffffffff),
		};
		uint16_t indices[] = {
			0, 1, 2,
			1, 3, 2,
			4, 6, 5,
			5, 6, 7,
			0, 2, 4,
			4, 2, 6,
			1, 5, 3,
			5, 7, 3,
			0, 4, 1,
			4, 5, 1,
			2, 3, 6,
			6, 3, 7,
		};
		
		Model* pCubeModel { ModelManager::create(vertices, 8, indices, 36, Vertex_Colored::layout) };
		
		// pCubeModel.print(true, true);
		
		return create(pCubeModel, _programHandle, _transform);
	}

	void render(void) {
		for (int i = 0; i < currentIndex; i++) objects[i].render();
	}
}
