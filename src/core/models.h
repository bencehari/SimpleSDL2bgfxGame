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

extern void model_hnd_initialize(int _maxModelCount);
extern void model_hnd_deinitialize(void);

extern void model_print(struct Model* _model, bool _printVertices, bool _printIndices);

extern int model_create(const struct Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx_vertex_layout_t* _vertexLayout);
extern struct Model* model_get_by_idx(int _idx);

#endif // KE_MODELS_H