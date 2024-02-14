#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <bgfx/bgfx.h>

static int row = 1;

void dbg_print_to_screen(const char* _format, ...) {
	char text[100];
	
	va_list args;
	va_start(args, _format);
	
	vsprintf(text, _format, args);
	bgfx::dbgTextPrintf(0, row++, 0x0f, text);
	
	va_end(args);
}

void dbg_m4x4_to_screen(const Matrix4x4* _m, const char* _name) {
	if (_name == NULL) bgfx::dbgTextPrintf(0, row++, 0x0f, "[TRANSFORM MATRIX]");
	else bgfx::dbgTextPrintf(0, row++, 0x0f, "[TRANSFORM MATRIX (%s)]", _name);
	bgfx::dbgTextPrintf(0, row++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][0], _m->Elements[1][0], _m->Elements[2][0], _m->Elements[3][0]);
	bgfx::dbgTextPrintf(0, row++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][1], _m->Elements[1][1], _m->Elements[2][1], _m->Elements[3][1]);
	bgfx::dbgTextPrintf(0, row++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][2], _m->Elements[1][2], _m->Elements[2][2], _m->Elements[3][2]);
	bgfx::dbgTextPrintf(0, row++, 0x0f, "%6.2f %6.2f %6.2f %6.2f", _m->Elements[0][3], _m->Elements[1][3], _m->Elements[2][3], _m->Elements[3][3]);
}

void dbg_m4x4_to_console(const Matrix4x4* _m) {
	printf("[TRANSFORM MATRIX]\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f %6.2f\n",
		_m->Elements[0][0], _m->Elements[1][0], _m->Elements[2][0], _m->Elements[3][0],
		_m->Elements[0][1], _m->Elements[1][1], _m->Elements[2][1], _m->Elements[3][1],
		_m->Elements[0][2], _m->Elements[1][2], _m->Elements[2][2], _m->Elements[3][2],
		_m->Elements[0][3], _m->Elements[1][3], _m->Elements[2][3], _m->Elements[3][3]);
}

void dbg_space(int _count) {
	row += _count;
}

void dbg_reset(void) {
	row = 1;
}
