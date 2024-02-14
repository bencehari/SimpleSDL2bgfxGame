#ifndef KE_PROGRAMS_H
#define KE_PROGRAMS_H

#include <stdbool.h>

#include <bgfx/bgfx.h>

extern void programs_init(int _maxProgramCount);
extern void programs_cleanup(void);

extern bgfx::ProgramHandle program_create(const char* _vertexShader, const char* _fragmentShader, bool _destroyShaders);

#endif // KE_PROGRAMS_H
