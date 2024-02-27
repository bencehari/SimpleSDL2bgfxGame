/**
 * @file wavefront_obj.hh
 *
 * @brief Loads raw Wavefront OBJ file runtime.
 *
 * - Detects indices order (CW or CCW), if requested.
 * - Process vertex data with colors (v x y z r g b), if RGB is missing,
 *   default color will be added.
 * - Process normals (vn x y z).
 * - Process faces (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 [v4/vt4/vn4]),
 *   tris and quads.
*/

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
 * @brief Creates a model from Wavefront OBJ data.
 *
 * @param _objPath Path to the .obj file.
 * @param _order Indices order for how to process the .obj data.
 *
 * @return Pointer to the created Model or nullptr on error.
*/
extern Model* wfobj_load(const char* _objPath, IndicesOrder _order = IndicesOrder::Auto);

#endif // KE_WAVEFRONT_OBJ
