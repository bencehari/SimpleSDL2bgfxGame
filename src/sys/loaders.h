#ifndef LOADERS_H
#define LOADERS_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../core/models.h"
#include "../core/object.h"

/**
 * @brief Modes for how to process the provided geometry data.
*/
enum IndicesOrder {
	INDICES_ORDER_AUTO,					//!< Not provided, should auto-detect.
	INDICES_ORDER_CLOCKWISE,			//!< Clockwise
	INDICES_ORDER_COUNTERCLOCKWISE		//!< Counter clockwise
};

/**
 * @brief Creates a model from Wavefront OBJ data.
 *
 * @param _objPath Path to the .obj file.
 * @param _vertexLayout bgfx::VertexLayout of the Model.
 * @param _order Indices order for how to process the .obj data.
 *
 * @return Pointer to the created Model or nullptr on error.
*/
extern Model* loadExternalGeometry_OBJ(const char* _objPath, const bgfx::VertexLayout* _vertexLayout, enum IndicesOrder _order);

/**
 * @brief Loads shader.
 *
 * Loads binary data (shipped with bgfx examples) and
 * feeds it to bgfx.
 *
 * @param _filename Path to the shader file.
 * @param _shaderHandle bgfx::ShaderHandle pointer to the created shader.
 *
 * @return False on error, otherwise true.
*/
extern bool loadShader(const char* _filename, bgfx::ShaderHandle* _shaderHandle);

#endif // LOADERS_H
