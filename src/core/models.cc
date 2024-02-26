#include "models.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/consc.h"

// Model class

Model::Model(
		int _id,
		Vertex_Colored _vertices[],
		int _verticesLen,
		uint16_t _indices[],
		int _indicesLen,
		bgfx::VertexBufferHandle _vertexBufferHnd,
		bgfx::IndexBufferHandle _indexBufferHnd
	) :
		id(_id),
		vertices(_vertices),
		indices(_indices),
		verticesLen(_verticesLen),
		indicesLen(_indicesLen),
		vertexBufferHnd(_vertexBufferHnd),
		indexBufferHnd(_indexBufferHnd) {}

Model* Model::create(Vertex_Colored _vertices[], int _verticesLen, uint16_t _indices[], int _indicesLen, bgfx::VertexLayout& _vertexLayout) {
	return ModelManager::create(_vertices, _verticesLen, _indices, _indicesLen, _vertexLayout);
}

void Model::cleanup(void) {
	bgfx::destroy(vertexBufferHnd);
	bgfx::destroy(indexBufferHnd);
	
	free(vertices);
	free(indices);
}

void Model::print(bool _printVertices, bool _printIndices) {
	printf(AC_CYAN "[MODEL INFO]\n  [BASE]" AC_RESET "\n    id: %d, vertLen: %d, indLen: %d, vertex: %s, indices: %s, vBufHnd: %u, iBuffHnd: %u\n",
		id,
		verticesLen,
		indicesLen,
		vertices == nullptr ? "false" : "true",
		indices == nullptr ? "false" : "true",
		vertexBufferHnd.idx,
		indexBufferHnd.idx);
	
	if (_printVertices && vertices != nullptr) {
		printf(AC_CYAN "  [VERTICES]\n" AC_RESET);
		for (int i = 0; i < verticesLen; i++) {
			printf("    %8.2f %8.2f %8.2f\t\t0x%08x\n",
				vertices[i].x,
				vertices[i].y,
				vertices[i].z,
				vertices[i].abgr);
		}
	}
	
	if (_printIndices && indices != nullptr) {
		printf(AC_CYAN"  [INDICES]\n" AC_RESET);
		for (int i = 2; i < indicesLen; i += 3) {
			printf("%8d %8d %8d\n", indices[i - 2], indices[i - 1], indices[i]);
		}
	}
	printf(AC_CYAN "\n[MODEL INFO END]\n" AC_RESET);
}

namespace ModelManager {
	static bool initialized;

	static int maxModelCount;
	static int currentIndex;
	static Model* models;

	ErrorCode init(int _maxModelCount) {
		if (initialized) return err_create(ALREADY_INITED, "ModelManager::init");
		
		maxModelCount = _maxModelCount;
		models = (Model*)malloc(sizeof(Model) * _maxModelCount);
		if (models == NULL) return err_create(MEM_ALLOC, "ModelManager::init");
		
		initialized = true;
		return NONE;
	}

	void cleanup(void) {
		while (--currentIndex >= 0) {
			models[currentIndex].cleanup();
		}
		
		free(models);

		models = nullptr;
		currentIndex = 0;
		initialized = false;
	}

	Model* create(const Vertex_Colored _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx::VertexLayout& _vertexLayout) {
		if (currentIndex >= maxModelCount) {
			puts(AC_YELLOW "Max model count reached." AC_RESET);
			return nullptr;
		}

		size_t verticesSize { sizeof(Vertex_Colored) * _verticesLen };
		size_t indicesSize { sizeof(uint16_t) * _indicesLen };

		Vertex_Colored* pvertices { (Vertex_Colored*)malloc(verticesSize) };
		if (pvertices == NULL) {
			err_create(MEM_ALLOC, "Vertices");
			return nullptr;
		}
		
		memcpy(pvertices, _vertices, verticesSize);
		
		uint16_t* pindices { (uint16_t*)malloc(indicesSize) };
		if (pvertices == NULL) {
			err_create(MEM_ALLOC, "Indices");
			free(pvertices);
			return nullptr;
		}
		
		memcpy(pindices, _indices, indicesSize);
		
		models[currentIndex] = Model(
			currentIndex,
			pvertices,
			_verticesLen,
			pindices,
			_indicesLen,
			bgfx::createVertexBuffer(bgfx::makeRef(pvertices, verticesSize), _vertexLayout, BGFX_BUFFER_NONE),
			bgfx::createIndexBuffer(bgfx::makeRef(pindices, indicesSize), BGFX_BUFFER_NONE)
		);
		
		return &models[currentIndex++];
	}
}
