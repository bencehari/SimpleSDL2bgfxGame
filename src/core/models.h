#ifndef KE_MODELS_H
#define KE_MODELS_H

#include <stdbool.h>

#include <bgfx/c99/bgfx.h>

#include "vertex.h"

class Model {
private:
	int id;
	Vertex* vertices;
	uint16_t* indices;
	
public:
	int verticesLen;
	int indicesLen;

	bgfx_vertex_buffer_handle_t vertexBufferHnd;
	bgfx_index_buffer_handle_t indexBufferHnd;

	void cleanup(void);
	void print(bool _printVertices, bool _printIndices);
	
	Model(
		int _id,
		Vertex _vertices[],
		int _verticesLen,
		uint16_t _indices[],
		int _indicesLen,
		bgfx_vertex_buffer_handle_t _vertexBufferHnd,
		bgfx_index_buffer_handle_t _indexBufferHnd)
		:
		id(_id),
		vertices(_vertices),
		indices(_indices),
		verticesLen(_verticesLen),
		indicesLen(_indicesLen),
		vertexBufferHnd(_vertexBufferHnd),
		indexBufferHnd(_indexBufferHnd) {}
};

extern void models_init(int _maxModelCount);
extern void models_cleanup(void);

extern Model* model_create(const Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx_vertex_layout_t* _vertexLayout);

#endif // KE_MODELS_H
