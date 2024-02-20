#include "vertex.hh"

bgfx::VertexLayout Vertex_Colored::layout;
bgfx::VertexLayout Vertex_Textured::layout;

Vertex_Colored::Vertex_Colored(float _x, float _y, float _z, uint32_t _abgr) : x(_x), y(_y), z(_z), abgr(_abgr) {}

void Vertex_Colored::init(void) {
	Vertex_Colored::layout
		.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, false)
		.end();
}

Vertex_Textured::Vertex_Textured(float _x, float _y, float _z, uint32_t _normal, uint32_t _tangent, int16_t _u, int16_t _v) :
		x(_x), y(_y), z(_z),
		normal(_normal), tangent(_tangent),
		u(_u), v(_v) {}

void Vertex_Textured::init(void) {
	Vertex_Textured::layout
		.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false)
			.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
		.end();
}

namespace VertexManager {
	void init(void) {
		Vertex_Colored::init();
		Vertex_Textured::init();
	}
}
