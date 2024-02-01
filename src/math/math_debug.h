#ifndef KE_MATH_DEBUG_H
#define KE_MATH_DEBUG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "HandmadeMath.h"
#pragma GCC diagnostic pop

#include "math_glue.h"

extern void MAT4_print_to_screen(const Matrix4x4* _m, int _startRow, const char* _name);
extern void MAT4_print_to_console(const Matrix4x4* _m);

#endif // KE_MATH_DEBUG_H
