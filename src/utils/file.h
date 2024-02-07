#ifndef KE_FILE_H
#define KE_FILE_H

#include <stdio.h>
#include <stdbool.h>

extern bool file_get_size(const char* _filePath, long* _fileSize, FILE** _file);

#endif // KE_FILE_H
