#ifndef KE_MODELS_HH
#define KE_MODELS_HH

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../sys/err.h"
#include "vertex.hh"

/**
 * @brief Model class contains all information fro bgfx to render a geometry.
*/
class Model {
private:
	unsigned int id;
	void* vertices;
	uint16_t* indices;
	
public:
	int verticesLen;
	int indicesLen;

	bgfx::VertexBufferHandle vertexBufferHnd;
	bgfx::IndexBufferHandle indexBufferHnd;

	/**
	 * @brief DO NOT call this directly!
	*/
	Model(
		int _id,
		void* _vertices,
		int _verticesLen,
		uint16_t _indices[],
		int _indicesLen,
		bgfx::VertexBufferHandle _vertexBufferHnd,
		bgfx::IndexBufferHandle _indexBufferHnd);

	/**
	 * @brief Creates the model.
	 *
	 * Creates and returns the model using bgfx. Indices must be in counter clockwise order.
	 *
	 * @param _vertexType VertexType enum will define which Vertex layout will be used.
	 * @param _vertices Vertex array as void* (contents must aligned with specified _vertexType).
	 * @param _verticesLen Length of _vertices.
	 * @param _indices uint16_t array of indices.
	 * @param _indicesLen Length of _indices.
	 *
	 * @return Pointer to the created Model, otherwise nullptr.
	*/
	static Model* create(const VertexType _vertexType, void* _vertices, int _verticesLen, uint16_t _indices[], int _indicesLen);

	void cleanup(void);
	
	void print(bool _printVertices, bool _printIndices);
};

namespace ModelManager {
	/**
	 * @brief Initializes the Model Manager
	 *
	 * It reservers enough space in memory for models.
	 *
	 * @param _maxModelCount Maximum number of models it can create.
	 *
	 * @return ErrorCode.
	*/
	extern ErrorCode init(int _maxModelCount);

	/**
	 * @brief Free up space reserved for models.
	*/
	extern void cleanup(void);

	/**
	 * @brief Creates the model.
	 *
	 * Creates and returns the model using bgfx. Indices must be in counter clockwise order.
	 *
	 * @param _vertexType VertexType enum will define which Vertex layout will be used.
	 * @param _vertices Vertex array as void* (contents must aligned with specified _vertexType).
	 * @param _verticesLen Length of _vertices.
	 * @param _indices uint16_t array of indices.
	 * @param _indicesLen Length of _indices.
	 *
	 * @return Pointer to the created Model, otherwise nullptr.
	*/
	extern Model* create(const VertexType _vertexType, const void* _vertices, int _verticesLen, const uint16_t _indices[], int _indicesLen);
}

#endif // KE_MODELS_HH
