#ifndef KE_VERTEX_HH
#define KE_VERTEX_HH

#include <stdint.h>

#include <bgfx/bgfx.h>

/**
 * @brief Possible types of vertices.
*/
enum VertexType {
	Basic,			//!< Only positions
	Color,			//!< With vertex color data
	Texture			//!< With texture coordinates
};

/**
 * @brief Vertex with additional color data.
*/
struct Vertex_Colored {
	float x, y, z;				//!< position coordinates
	uint32_t abgr;				//!< vertex color

	Vertex_Colored(float _x, float _y, float _z, uint32_t _abgr);
	
	static bgfx::VertexLayout layout;
	static void init(void);
	
	void print();
};

/**
 * @brief Vertex for texturing.
*/
struct Vertex_Textured {
	float x, y, z;				//!< position coordinates
	uint32_t normal, tangent;	//!< normal and tangent
	int16_t u, v;				//!< UV coordinates
	
	Vertex_Textured(float _x, float _y, float _z, uint32_t _normal, uint32_t _tangent, int16_t _u, int16_t _v);

	static bgfx::VertexLayout layout;
	static void init(void);
};

namespace VertexManager {
	/**
	 * @brief Initializes static Vertex datas (currently only that one).
	*/
	extern void init(void);
}

#endif // KE_VERTEX_HH
