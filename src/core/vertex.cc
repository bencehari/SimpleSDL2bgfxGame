#include "vertex.hh"

bgfx::VertexLayout Vertex_Colored::layout;
bgfx::VertexLayout Vertex_Textured::layout;

void Vertex_Colored::init(void) {
	Vertex_Colored::layout
		.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, false)
		.end();
}

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
