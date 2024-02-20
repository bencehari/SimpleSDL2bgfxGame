#ifndef KE_COLOR_H
#define KE_COLOR_H

/**
 * @brief Converts RGB to HEX.
 *
 * RGB is a normalized float (0.0f - 1.0f) and HEX contains alpha (0xffffffff).
 *
 * @param r Red (0.0f - 1.0f)
 * @param g Green (0.0f - 1.0f)
 * @param b Blue (0.0f - 1.0f)
 *
 * @return unsigned int HEX value.
*/
extern unsigned int rgbToHex(const float r, const float g, const float b);

/**
 * @brief Converts RGBA to HEX.
 *
 * RGB is a normalized float (0.0f - 1.0f) and HEX contains alpha (0xffffffff).
 *
 * @param r Red (0.0f - 1.0f)
 * @param g Green (0.0f - 1.0f)
 * @param b Blue (0.0f - 1.0f)
 * @param a Alpha (0.0f - 1.0f)
 *
 * @return unsigned int HEX value.
*/
extern unsigned int rgbaToHex(const float r, const float g, const float b, const float a);

/**
 * @brief Converts RGB to HEX.
 *
 * RGB is unsigned char (0 - 255) and HEX contains alpha (0xffffffff).
 *
 * @param r Red (0 - 255)
 * @param g Green (0 - 255)
 * @param b Blue (0 - 255)
 *
 * @return unsigned int HEX value.
*/
extern unsigned int rgbToHex(const unsigned char r, const unsigned char g, const unsigned char b);

/**
 * @brief Converts RGBA to HEX.
 *
 * RGB is unsigned char (0 - 255) and HEX contains alpha (0xffffffff).
 *
 * @param r Red (0 - 255)
 * @param g Green (0 - 255)
 * @param b Blue (0 - 255)
 * @param a Alpha (0 - 255)
 *
 * @return unsigned int HEX value.
*/
extern unsigned int rgbaToHex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

#endif // KE_COLOR_H
