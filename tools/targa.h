#ifndef TARGA_H
#define TARGA_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates targa image file.
 *
 * @param _fileName Name of the output file without extension.
 * @param _width Width of image.
 * @param _height Height of image.
 *
 * @return False on error, otherwise true.
*/
extern bool create_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height);

#ifdef __cplusplus
}
#endif

#endif // TARGA_H
