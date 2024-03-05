#ifndef TARGA_H
#define TARGA_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ColorDataStart {
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	TOP_LEFT,
	TOP_RIGHT
};

/**
 * @brief Creates targa image file.
 *
 * @param _fileName Name of the output file without extension.
 * @param _width Width of image.
 * @param _height Height of image.
 * @param _start Starting corner of color data.
 * @param _colors Color data.
 * @param _verify Verifies the output file.
 * @return False on error, otherwise true.
*/
extern bool create_tga_image(
	const char* _fileName,
	const uint16_t _width,
	const uint16_t _height,
	const enum ColorDataStart _start,
	const uint8_t* _colors,
	const bool _verify);

#ifdef __cplusplus
}
#endif

#endif // TARGA_H
