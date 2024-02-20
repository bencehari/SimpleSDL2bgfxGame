#ifndef KE_WAVEFRONT_OBJ
#define KE_WAVEFRONT_OBJ

#include <bgfx/bgfx.h>

#include "../core/models.hh"

/**
 * @brief Modes for how to process the provided geometry data.
*/
enum IndicesOrder {
	Auto,	//!< Not provided, should auto-detect.
	CW,		//!< Clockwise
	CCW		//!< Counter clockwise
};

/**
 * @brief Creates a model (colored by vertex) from Wavefront OBJ data.
 *
 * Obj should contain vertex color data (defaults to #ff007f [pink]).
 * In Blender: add Color Attribute in Object Data Properties
 * and export with Color checked in Geometry.
 *
 * @param _objPath Path to the .obj file.
 * @param _vertexLayout bgfx::VertexLayout of the Model.
 * @param _order Indices order for how to process the .obj data.
 *
 * @return Pointer to the created Model or nullptr on error.
*/
extern Model* wfobj_loadColored(const char* _objPath, IndicesOrder _order = IndicesOrder::Auto);

/**
 * @brief Creates a textured model from Wavefront OBJ data.
 *
 * @param _objPath Path to the .obj file.
 * @param _vertexLayout bgfx::VertexLayout of the Model.
 * @param _order Indices order for how to process the .obj data.
 *
 * @return Pointer to the created Model or nullptr on error.
*/
extern Model* wfobj_loadTextured(const char* _objPath, IndicesOrder _order = IndicesOrder::Auto);

#endif // KE_WAVEFRONT_OBJ
