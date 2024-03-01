#include "file.h"

#include <string.h>

#include "consc.h"

bool get_file_size(const char* _filePath, long* _fileSize, FILE** _file) {
	*_file = fopen(_filePath, "rb");
	if (_file == NULL) {
		printf(AC_YELLOW "file is NULL: %s\n" AC_RESET, _filePath);
		return false;
	}
	
	fseek(*_file, 0, SEEK_END);
	*_fileSize = ftell(*_file);
	fseek(*_file, 0, SEEK_SET);
	
	return true;
}

int get_file_name(const char* _path) {
	if (_path == NULL) return -1;
	
	int len = (int)strlen(_path);
	for (; len >= 0; len--) {
		if (_path[len] == '/') break;
	}
	return ++len;
}
