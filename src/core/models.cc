#include "models.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/consc.h"

// Model class

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
		vertices == NULL ? "false" : "true",
		indices == NULL ? "false" : "true",
		vertexBufferHnd.idx,
		indexBufferHnd.idx);
	
	if (_printVertices && vertices != NULL) {
		printf(AC_CYAN "  [VERTICES]\n" AC_RESET);
		for (int i = 0; i < verticesLen; i++) {
			printf("    %8.2f %8.2f %8.2f\t\t0x%08x\n",
				vertices[i].x,
				vertices[i].y,
				vertices[i].z,
				vertices[i].abgr);
		}
	}
	
	if (_printIndices && indices != NULL) {
		printf(AC_CYAN"  [INDICES]\n" AC_RESET);
		for (int i = 2; i < indicesLen; i += 3) {
			printf("%8d %8d %8d\n", indices[i - 2], indices[i - 1], indices[i]);
		}
	}
	printf(AC_CYAN "\n[MODEL INFO END]\n" AC_RESET);
}

// ModelManager

static bool initialized;

static int modelCount;
static int currentModelIndex;
static Model* models;

void initModelManager(int _maxModelCount) {
	if (initialized) {
		puts("Already initialized.");
		return;
	}
	
	modelCount = _maxModelCount;
	models = (Model*)malloc(sizeof(Model) * _maxModelCount);
	
	initialized = true;
}

void cleanupModelManager(void) {
	while (--currentModelIndex >= 0) {
		models[currentModelIndex].cleanup();
		free(&models[currentModelIndex]);
	}
	
	models = nullptr;
	currentModelIndex = 0;
	initialized = false;
}

Model* createModel(const Vertex_PosColor _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx::VertexLayout& _vertexLayout) {
	if (currentModelIndex >= modelCount) {
		puts(AC_YELLOW "Max model count reached." AC_RESET);
		return nullptr;
	}

	size_t verticesSize = sizeof(Vertex_PosColor) * _verticesLen;
	size_t indicesSize = sizeof(uint16_t) * _indicesLen;

	Vertex_PosColor* pvertices = (Vertex_PosColor*)malloc(verticesSize);
	memcpy(pvertices, _vertices, verticesSize);
	
	uint16_t* pindices = (uint16_t*)malloc(indicesSize);
	memcpy(pindices, _indices, indicesSize);
	
	models[currentModelIndex] = Model(
		currentModelIndex,
		pvertices,
		_verticesLen,
		pindices,
		_indicesLen,
		bgfx::createVertexBuffer(bgfx::makeRef(pvertices, verticesSize), _vertexLayout, BGFX_BUFFER_NONE),
		bgfx::createIndexBuffer(bgfx::makeRef(pindices, indicesSize), BGFX_BUFFER_NONE)
	);
	
	return &models[currentModelIndex++];
}
