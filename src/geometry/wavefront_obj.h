#ifndef KE_WAVEFRONT_OBJ
#define KE_WAVEFRONT_OBJ

#include <bgfx/bgfx.h>

#include "../core/models.h"

/**
 * @brief Modes for how to process the provided geometry data.
*/
enum IndicesOrder {
	Auto,	//!< Not provided, should auto-detect.
	CW,		//!< Clockwise
	CCW		//!< Counter clockwise
};

namespace WavefrontObj {
	/**
	 * @brief Creates a model from Wavefront OBJ data.
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
	extern Model* loadColored(const char* _objPath, enum IndicesOrder _order = IndicesOrder::Auto);
}

#endif // KE_WAVEFRONT_OBJ
