#include "vertex.h"

bgfx::VertexLayout vertexLayout;

void vertex_init(void) {
	vertexLayout
		.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, false)
		.end();
}
