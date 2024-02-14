#ifndef KE_MODELS_H
#define KE_MODELS_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "vertex.h"

class Model {
private:
	int id;
	Vertex* vertices;
	uint16_t* indices;
	
public:
	int verticesLen;
	int indicesLen;

	bgfx::VertexBufferHandle vertexBufferHnd;
	bgfx::IndexBufferHandle indexBufferHnd;

	void cleanup(void);
	void print(bool _printVertices, bool _printIndices);
	
	Model(
		int _id,
		Vertex _vertices[],
		int _verticesLen,
		uint16_t _indices[],
		int _indicesLen,
		bgfx::VertexBufferHandle _vertexBufferHnd,
		bgfx::IndexBufferHandle _indexBufferHnd)
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

extern Model* model_create(const Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx::VertexLayout& _vertexLayout);

#endif // KE_MODELS_H
