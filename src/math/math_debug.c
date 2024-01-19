#include "math_debug.h"

#include <stdio.h>
#include <string.h>

#include <bgfx/c99/bgfx.h>

void MAT4_print_to_screen(const Mat4* _m, int _startRow, const char* _name) {
	if (_name == NULL) bgfx_dbg_text_printf(0, _startRow++, 0x0f, "[TRANSFORM MATRIX]");
	else bgfx_dbg_text_printf(0, _startRow++, 0x0f, "[TRANSFORM MATRIX (%s)]", _name);
	bgfx_dbg_text_printf(0, _startRow++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][0], _m->Elements[1][0], _m->Elements[2][0], _m->Elements[3][0]);
	bgfx_dbg_text_printf(0, _startRow++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][1], _m->Elements[1][1], _m->Elements[2][1], _m->Elements[3][1]);
	bgfx_dbg_text_printf(0, _startRow++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][2], _m->Elements[1][2], _m->Elements[2][2], _m->Elements[3][2]);
	bgfx_dbg_text_printf(0, _startRow  , 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][3], _m->Elements[1][3], _m->Elements[2][3], _m->Elements[3][3]);
}

void MAT4_print_to_console(const Mat4* _m) {
	printf("[TRANSFORM MATRIX]\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n",
		_m->Elements[0][0], _m->Elements[1][0], _m->Elements[2][0], _m->Elements[3][0],
		_m->Elements[0][1], _m->Elements[1][1], _m->Elements[2][1], _m->Elements[3][1],
		_m->Elements[0][2], _m->Elements[1][2], _m->Elements[2][2], _m->Elements[3][2],
		_m->Elements[0][3], _m->Elements[1][3], _m->Elements[2][3], _m->Elements[3][3]);
}
