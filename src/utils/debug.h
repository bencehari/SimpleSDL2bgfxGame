#ifndef KE_DEBUG_H
#define KE_DEBUG_H

#include "../math/math_include.h"

extern void dbg_print_to_screen(const char* _format, ...);

extern void dbg_m4x4_to_screen(const Matrix4x4* _m, const char* _name);
extern void dbg_m4x4_to_console(const Matrix4x4* _m);

extern void dbg_space(int _count);

// must call on every frame's end
extern void dbg_reset(void);

#endif // KE_DEBUG_H
