#ifndef KE_VERTEX_H
#define KE_VERTEX_H

#include <stdint.h>

#include <bgfx/bgfx.h>

// TODO: this whole logic is wrong!
// Vertex_Colored everywhere is a big no-no.
// Figure out something for generalize this!

/**
 * @brief Vertex with additional color data.
*/
struct Vertex_Colored {
	float x, y, z;				//!< position coordinates
	uint32_t abgr;				//!< vertex color

	Vertex_Colored(float _x, float _y, float _z, uint32_t _abgr) : x(_x), y(_y), z(_z), abgr(_abgr) {}
	
	static bgfx::VertexLayout layout;
	static void init(void);
};

/**
 * @brief Vertex for texturing.
*/
struct Vertex_Textured {
	float x, y, z;				//!< position coordinates
	uint32_t normal, tangent;	//!< normal and tangent
	int16_t u, v;				//!< UV coordinates
	
	Vertex_Textured(float _x, float _y, float _z, uint32_t _normal, uint32_t _tangent, int16_t _u, int16_t _v) :
		x(_x), y(_y), z(_z),
		normal(_normal), tangent(_tangent),
		u(_u), v(_v) {}

	static bgfx::VertexLayout layout;
	static void init(void);
};

namespace VertexManager {
	/**
	 * @brief Initializes static Vertex datas (currently only that one).
	*/
	extern void init(void);
}

#endif // KE_VERTEX_H
