#ifndef KE_VERTEX_H
#define KE_VERTEX_H

#include <stdint.h>

#include <bgfx/c99/bgfx.h>

struct Vertex {
	float x, y, z;
	uint32_t abgr;
};

extern bgfx_vertex_layout_t vertexLayout;

#define VERTEX_NEW(_x, _y, _z, _c) ((struct Vertex) { (_x), (_y), (_z), (_c) })

extern void vertex_init(void);

#endif // KE_VERTEX_H
