#ifndef KE_FILE_H
#define KE_FILE_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Gets the file's size.
 *
 * @param _filePath Path to the file.
 * @param _fileSize Pointer to long variable to set the file size.
 * @param _file Pointer (FILE*) to the file stream (SEEK_SET).
 *
 * @return False on error, otherwise true.
*/
extern bool getFileSize(const char* _filePath, long* _fileSize, FILE** _file);

#endif // KE_FILE_H
