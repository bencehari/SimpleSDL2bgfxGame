#ifndef TARGA_H
#define TARGA_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates targa image file.
 *
 * @param _fileName Nem of the output file without extension.
 *
 * @return False on error, otherwise true.
*/
extern bool create_tga_image(const char* _fileName);

#ifdef __cplusplus
}
#endif

#endif // TARGA_H
