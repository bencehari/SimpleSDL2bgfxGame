#include "file.h"
#include "consc.h"

bool getFileSize(const char* _filePath, long* _fileSize, FILE** _file) {
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
