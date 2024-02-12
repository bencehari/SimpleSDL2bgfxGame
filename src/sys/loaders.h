#ifndef LOADERS_H
#define LOADERS_H

#include <stdbool.h>

#include <bgfx/c99/bgfx.h>

#include "../core/models.h"
#include "../core/object.h"

enum IndicesOrder {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

extern bool load_external_obj_model(const char* _objPath, const bgfx_vertex_layout_t* _vertexLayout, struct Model** _model, enum IndicesOrder _order);

extern bool load_shader(const char* _filename, bgfx_shader_handle_t* _shaderHandle);

#endif // LOADERS_H
