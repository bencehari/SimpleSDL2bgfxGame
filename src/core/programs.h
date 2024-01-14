#ifndef KE_PROGRAMS_H
#define KE_PROGRAMS_H

#include <stdbool.h>

#include <bgfx/C99/bgfx.h>

extern bgfx_program_handle_t program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders);

#endif // KE_PROGRAMS_H
