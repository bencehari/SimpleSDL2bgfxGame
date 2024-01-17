#include "object.h"

void obj_render(const struct Object* _o) {
	bgfx_set_transform(_o->transform.Elements, 1);
		
	bgfx_set_vertex_buffer(0, _o->model->vertexBufferHnd, 0, _o->model->verticesLen);
	bgfx_set_index_buffer(_o->model->indexBufferHnd, 0, _o->model->indicesLen);
	
	bgfx_submit(0, _o->program, 0, BGFX_DISCARD_ALL);
}

void obj_encoder_render(bgfx_encoder_t* _encoder, const struct Object* _o) {
	bgfx_encoder_set_transform(_encoder, _o->transform.Elements, 1);
		
	bgfx_encoder_set_vertex_buffer(_encoder, 0, _o->model->vertexBufferHnd, 0, _o->model->verticesLen);
	bgfx_encoder_set_index_buffer(_encoder, _o->model->indexBufferHnd, 0, _o->model->indicesLen);
	
	bgfx_encoder_submit(_encoder, 0, _o->program, 0, BGFX_DISCARD_ALL);
}
