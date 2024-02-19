#ifndef KE_FILE_H
#define KE_FILE_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Gets the file's size.
 *
 * @param _filePath Paath to the file.
 * @param _fileSize long variable to set the file size.
 * @param _file Pointer (FILE*) to the file stream (SEEK_SET).
 *
 * @return False on error, otherwise true.
*/
extern bool getFileSize(const char* _filePath, long& _fileSize, FILE*& _file);

#endif // KE_FILE_H
