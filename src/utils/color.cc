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

unsigned int rgbaToHex(const float r, const float g, const float b, const float a) {
	Color c;
	c.rgba.r = 255 * r;
	c.rgba.g = 255 * g;
	c.rgba.b = 255 * b;
	c.rgba.a = 255 * a;
	return c.hex;
}

unsigned int rgbaToHex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
	Color c;
	c.rgba.r = r;
	c.rgba.g = g;
	c.rgba.b = b;
	c.rgba.a = a;
	return c.hex;
}
