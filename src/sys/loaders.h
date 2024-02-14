#ifndef LOADERS_H
#define LOADERS_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

#include "../core/models.h"
#include "../core/object.h"

enum IndicesOrder {
	INDICES_ORDER_AUTO,
	INDICES_ORDER_CLOCKWISE,
	INDICES_ORDER_COUNTERCLOCKWISE
};

extern bool load_external_obj_geometry(const char* _objPath, const bgfx::VertexLayout* _vertexLayout, Model** _model, enum IndicesOrder _order);

extern bool load_shader(const char* _filename, bgfx::ShaderHandle* _shaderHandle);

#endif // LOADERS_H
