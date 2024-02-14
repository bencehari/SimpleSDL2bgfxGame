#ifndef KE_VERTEX_H
#define KE_VERTEX_H

#include <stdint.h>

#include <bgfx/bgfx.h>

struct Vertex {
	float x, y, z;
	uint32_t abgr;
	
	Vertex(float _x, float _y, float _z, uint32_t _abgr) : x(_x), y(_y), z(_z), abgr(_abgr) {}
};

extern bgfx::VertexLayout vertexLayout;

extern void vertex_init(void);

#endif // KE_VERTEX_H
