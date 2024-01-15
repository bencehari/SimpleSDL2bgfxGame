#include "models.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/consc.h"

static bool initialized;

static int modelCount;
static int currentModelIndex;
static struct Model* models;

void models_init(int _maxModelCount) {
	if (initialized) {
		models_cleanup();
		currentModelIndex = 0;
	}
	
	modelCount = _maxModelCount;
	models = malloc(sizeof(struct Model) * _maxModelCount);
	
	initialized = true;
}

void models_cleanup(void) {
	while (--currentModelIndex >= 0) {
		free(models[currentModelIndex].vertices);
		free(models[currentModelIndex].indices);
		free(&models[currentModelIndex]);
	}
	
	models = NULL;
	initialized = false;
}

void model_print(const struct Model* _model, bool _printVertices, bool _printIndices) {
	printf(AC_CYAN "[MODEL INFO]\n  [BASE]" AC_RESET "\n    id: %d, vertLen: %d, indLen: %d, vertex: %s, indices: %s, vBufHnd: %u, iBuffHnd: %u\n",
		_model->id,
		_model->verticesLen,
		_model->indicesLen,
		_model->vertices == NULL ? "false" : "true",
		_model->indices == NULL ? "false" : "true",
		_model->vertexBufferHnd.idx,
		_model->indexBufferHnd.idx);
	
	if (_printVertices && _model->vertices != NULL) {
		printf(AC_CYAN "  [VERTICES]\n" AC_RESET);
		for (int i = 0; i < _model->verticesLen; i++) {
			printf("    %8.2f %8.2f %8.2f\t\t0x%08x\n",
				_model->vertices[i].x,
				_model->vertices[i].y,
				_model->vertices[i].z,
				_model->vertices[i].abgr);
		}
	}
	
	if (_printIndices && _model->indices != NULL) {
		printf(AC_CYAN"  [INDICES]\n" AC_RESET);
		for (int i = 2; i < _model->indicesLen; i += 3) {
			printf("%8d %8d %8d\n", _model->indices[i - 2], _model->indices[i - 1], _model->indices[i]);
		}
	}
	printf(AC_CYAN "\n[MODEL INFO END]\n" AC_RESET);
}

struct Model* model_create(const struct Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx_vertex_layout_t* _vertexLayout) {
	if (currentModelIndex >= modelCount) {
		puts(AC_YELLOW "Max model count reached." AC_RESET);
		return NULL;
	}

	size_t verticesSize = sizeof(struct Vertex) * _verticesLen;
	size_t indicesSize = sizeof(uint16_t) * _indicesLen;

	struct Vertex* pvertices = malloc(verticesSize);
	memcpy(pvertices, _vertices, verticesSize);
	
	uint16_t* pindices = malloc(indicesSize);
	memcpy(pindices, _indices, indicesSize);
	
	models[currentModelIndex] = (struct Model) {
		.id = currentModelIndex,
		.verticesLen = _verticesLen,
		.indicesLen = _indicesLen,
		.vertices = pvertices,
		.indices = pindices,
		.vertexBufferHnd = bgfx_create_vertex_buffer(bgfx_make_ref(pvertices, verticesSize), _vertexLayout, BGFX_BUFFER_NONE),
		.indexBufferHnd = bgfx_create_index_buffer(bgfx_make_ref(pindices, indicesSize), BGFX_BUFFER_NONE)
	};
	
	return &models[currentModelIndex++];
}
