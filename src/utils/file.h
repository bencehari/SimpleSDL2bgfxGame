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

/**
 * @brief Gets file name from path.
 *
 * Returns index of the char after the last '/'.
 * If the input is NULL, returns -1!
 * Does not working with '\'!!!
 *
 * @param _path Path char*.
 *
 * @return Start index of filename.
*/
extern int getFileName(const char* _path);

#endif // KE_FILE_H
