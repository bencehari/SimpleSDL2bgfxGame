#include "err.h"

#include <stdio.h>
#include <stdarg.h>

#include "../utils/consc.h"

enum ErrorCode err_create(enum ErrorCode _errorCode, const char* _format, ...) {
	if (_format == NULL) {
		printf(AC_RED "[%s]\n" AC_RESET, err_code_str(_errorCode));
		return _errorCode;
	}

	char text[100];

	va_list args;
	va_start(args, _format);

	vsprintf_s(text, 100, _format, args);
	printf(AC_RED "[%s] %s\n" AC_RESET, err_code_str(_errorCode), text);

	va_end(args);

	return _errorCode;
}

const char* err_code_str(enum ErrorCode _errorCode) {
	switch (_errorCode) {
		case NONE: return "None"; break;
		case MEM_ALLOC: return "Couldn't allocate memory (malloc)"; break;
		case OPEN_FILE: return "Couldn't open file"; break;
		case NO_CONTENT: return "Empty buffer, NULL return"; break;
		default: break;
	}
	return "[err.h:err_code_str] Shouldn't happen.";
}
