#include "object.h"

void Object::render(void) {
	Matrix4x4 t = M4x4_POS(transform.position.X, transform.position.Y, transform.position.Z);
	Matrix4x4 r = Q_TO_M4x4(transform.rotation);
	
	bgfx::setTransform((t * r).Elements, 1);
		
	bgfx::setVertexBuffer(0, model->vertexBufferHnd, 0, model->verticesLen);
	bgfx::setIndexBuffer(model->indexBufferHnd, 0, model->indicesLen);
	
	bgfx::submit(0, program, 0, BGFX_DISCARD_ALL);
}
