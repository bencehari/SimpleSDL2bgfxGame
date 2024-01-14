#include "vertex.h"

bgfx_vertex_layout_t vertexLayout;

void vertex_init(void) {
	bgfx_vertex_layout_begin(&vertexLayout, BGFX_RENDERER_TYPE_COUNT);
	bgfx_vertex_layout_add(&vertexLayout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
	bgfx_vertex_layout_add(&vertexLayout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true, false);
	bgfx_vertex_layout_end(&vertexLayout);
}
