#ifndef TARGA_H
#define TARGA_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ColorDataDirection {
	RIGHT_UP,		//!< Starts from bottom left corner
	LEFT_UP,		//!< Starts from bottom right corner
	RIGHT_DOWN,		//!< Starts from top left corner
	LEFT_DOWN		//!< Starts from top right corner
};

/**
 * @brief Creates targa image file.
 *
 * Color data direction is defaulted to RIGHT_DOWN and verification to false.
 *
 * @param _fileName Name of the output file without extension.
 * @param _width Width of image.
 * @param _height Height of image.
 * @param _colors Color data.
 * @return False on error, otherwise true.
*/
extern bool def_tga_image(
	const char* _fileName,
	const uint16_t _width,
	const uint16_t _height,
	const uint8_t* _colors);

/**
 * @brief Creates targa image file.
 *
 * @param _fileName Name of the output file without extension.
 * @param _width Width of image.
 * @param _height Height of image.
 * @param _dir Color data direction.
 * @param _colors Color data.
 * @param _verify Verifies the output file.
 * @return False on error, otherwise true.
*/
extern bool create_tga_image(
	const char* _fileName,
	const uint16_t _width,
	const uint16_t _height,
	const enum ColorDataDirection _dir,
	const uint8_t* _colors,
	const bool _verify);

/**
 * @brief Verifies targa content.
 *
 * @param _filePath Path to the targa file.
 *
 * @return True, if everything looks good, otherwise false.
*/
extern bool verify_tga_image(const char* _filePath);

#ifdef __cplusplus
}
#endif

#endif // TARGA_H
