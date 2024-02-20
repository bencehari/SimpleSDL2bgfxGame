#include "color.h"

union Color {
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} rgba;
	unsigned hex;
};

unsigned int rgbToHex(const float r, const float g, const float b) {
	Color c { (unsigned char)(255 * r), (unsigned char)(255 * g), (unsigned char)(255 * b), 255 };
	return c.hex;
}

unsigned int rgbaToHex(const float r, const float g, const float b, const float a) {
	Color c { (unsigned char)(255 * r), (unsigned char)(255 * g), (unsigned char)(255 * b), (unsigned char)(255 * a) };
	return c.hex;
}

unsigned int rgbToHex(const unsigned char r, const unsigned char g, const unsigned char b) {
	Color c { r, g, b, 255 };
	return c.hex;
}

unsigned int rgbaToHex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
	Color c { r, g, b, a };
	return c.hex;
}
