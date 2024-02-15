#include "vertex.h"

bgfx::VertexLayout Vertex_PosColor::layout;

void Vertex_PosColor::init(void) {
	Vertex_PosColor::layout
		.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, false)
		.end();
}

void initVertexVariants(void) {
	Vertex_PosColor::init();
}
