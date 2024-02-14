#include "object.h"

void Object::render() {
	Matrix4x4 t = M4x4_POS(transform.position.X, transform.position.Y, transform.position.Z);
	Matrix4x4 r = Q_TO_M4x4(transform.rotation);
	
	bgfx_set_transform((t * r).Elements, 1);
		
	bgfx_set_vertex_buffer(0, model->vertexBufferHnd, 0, model->verticesLen);
	bgfx_set_index_buffer(model->indexBufferHnd, 0, model->indicesLen);
	
	bgfx_submit(0, program, 0, BGFX_DISCARD_ALL);
}

void Object::encoder_render(bgfx_encoder_t& _encoder) {
	Matrix4x4 t = M4x4_POS(transform.position.X, transform.position.Y, transform.position.Z);
	Matrix4x4 r = Q_TO_M4x4(transform.rotation);
	
	bgfx_encoder_set_transform(&_encoder, (t * r).Elements, 1);
		
	bgfx_encoder_set_vertex_buffer(&_encoder, 0, model->vertexBufferHnd, 0, model->verticesLen);
	bgfx_encoder_set_index_buffer(&_encoder, model->indexBufferHnd, 0, model->indicesLen);
	
	bgfx_encoder_submit(&_encoder, 0, program, 0, BGFX_DISCARD_ALL);
}
