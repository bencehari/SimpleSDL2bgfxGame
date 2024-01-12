#ifndef KE_PROGRAMS_H
#define KE_PROGRAMS_H

#include <stdbool.h>

#include <bgfx/C99/bgfx.h>

extern void programs_initialize(int _maxProgramsCount);
extern void programs_deinitialize();

extern int program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders);
extern bgfx_program_handle_t* program_get_by_idx(int _idx);

#endif // KE_PROGRAMS_H
