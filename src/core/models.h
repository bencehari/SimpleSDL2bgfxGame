#ifndef KE_MODELS_H
#define KE_MODELS_H

#include <stdbool.h>

#include <bgfx/c99/bgfx.h>

#include "vertex.h"

struct Model {
	int id;
	int verticesLen;
	int indicesLen;
	struct Vertex* vertices;
	uint16_t* indices;
	bgfx_vertex_buffer_handle_t vertexBufferHnd;
	bgfx_index_buffer_handle_t indexBufferHnd;
};

extern void models_init(int _maxModelCount);
extern void models_cleanup(void);

extern void model_print(const struct Model* _model, bool _printVertices, bool _printIndices);

extern struct Model* model_create(const struct Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx_vertex_layout_t* _vertexLayout);

#endif // KE_MODELS_H
