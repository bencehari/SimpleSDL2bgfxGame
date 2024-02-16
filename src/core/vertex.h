#ifndef KE_VERTEX_H
#define KE_VERTEX_H

#include <stdint.h>

#include <bgfx/bgfx.h>

// TODO: this whole logic is wrong!
// Vertex_PosColor everywhere is a big no-no.
// Figure out something for generalize this!

/**
 * @brief Vertex with additional color data.
*/
struct Vertex_PosColor {
	float x, y, z;
	uint32_t abgr;

	Vertex_PosColor(float _x, float _y, float _z, uint32_t _abgr) : x(_x), y(_y), z(_z), abgr(_abgr) {}
	
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
