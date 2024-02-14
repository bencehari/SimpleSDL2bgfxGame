#ifndef KE_MODELS_H
#define KE_MODELS_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "vertex.h"

/**
 * @brief Model class contains all information fro bgfx to render a geometry.
*/
class Model {
private:
	unsigned int id;
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

/**
 * @brief Initializes the Model Manager
 *
 * It reservers enough space in memory for models.
 *
 * @param _maxModelCount Maximum number of models it can create.
*/
extern void initModelManager(int _maxModelCount);

/**
 * @brief Free up space reserved for models.
*/
extern void cleanupModelManager(void);

/**
 * @brief Creates the model.
 *
 * Creates and returns the model using bgfx. Indices must be in counter clockwise order.
 *
 * @param _vertices Vertex array.
 * @param _verticesLen Length of _vertices.
 * @param _indices uint16_t array of indices.
 * @param _indicesLen Length of _indices.
 * @param _vertexLayout bgfx::VertexLayout.
 *
 * @return Pointer to the created Model.
*/
extern Model* createModel(const Vertex _vertices[], int _verticesLen, const uint16_t _indices[], int _indicesLen, const bgfx::VertexLayout& _vertexLayout);

#endif // KE_MODELS_H
