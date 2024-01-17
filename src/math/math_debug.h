#ifndef KE_MATH_DEBUG_H
#define KE_MATH_DEBUG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#include "HandmadeMath.h"
#pragma GCC diagnostic pop

#include "math_glue.h"

extern void MAT4_PrintToScreen(const Mat4* _m, int _startRow);
extern void MAT4_PrintToConsole(const Mat4* _m);

#endif // KE_MATH_DEBUG_H