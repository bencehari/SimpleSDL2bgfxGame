#include "targa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t		u8;
typedef uint16_t	u16;

// cm prefix stands for Color-map
struct Header {
	u8		idLen;
	u8		colorMapType;
	u8		imageType;
	u16		cmFirstEntryIndex;	// Index of the first color map entry.
	u16		cmLen;				// Total number of color map entries included.
	u8		cmEntrySize;		// Establishes the number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
	u16		originX;
	u16		originY;
	u16		width;
	u16		height;
	u8		pixelDepth;
	u8		imageDescriptor;
};

static bool fread_header(struct Header* _h, FILE* _file);
static void print_header(struct Header* _h);

// comment this out if building by dds_test.bat
int main(void) { create_tga_image("tga_test_output", 511, 511, true); }

static void putnc(u8 c, size_t n, FILE* file) {
	for (size_t i = 0; i < n; i++) putc(c, file);
}

bool create_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height, const bool _verify) {
	size_t len = strlen(_fileName);
	char* fileName = malloc(sizeof(char) * len + sizeof(char) * 5);
	memcpy(fileName, _fileName, sizeof(char) * len);
	memcpy(fileName + len, ".tga", sizeof(char) * 5);
	
	FILE* file = fopen(fileName, "wb");
	if (file == NULL) {
		printf("[ERR] Failed to create/open file for write: %s\n", fileName);
		free(fileName);
		return false;
	}
	if (!_verify) free(fileName);
	
	union Dimension {
		u16 i;
		struct {
			u8 a;
			u8 b;
		};
	};
	
	union Dimension width = { .i = _width };
	union Dimension height = { .i = _height };
	
	/* Image Type - Field 3
	0-127 reserved for use by Truevision.
	128-255 may be used for developer applications.
	---------------
	 0 - No image data included.
	 1 - Uncompressed, color-mapped image.
	 2 - Uncompressed, True-color image.
	 3 - Uncompressed, black-and-white image.
	 9 - Run-length encoded, Color-mapped image.
	10 - Run-length encoded, True-color image.
	11 - Run-length encoded, black-and-white image. */

	putc(0, file);			// no image identification field
	putc(0, file);			// indicates that (0) no color-map, (1) a color-map included
	putc(2, file);			// uncompressed, True-color image
	putnc(0, 5, file);		// no color map, so no color map specification
	putnc(0, 2, file);		// X-origin; the absolute horizontal coordinate for the lower left corner of the image
	putnc(0, 2, file);		// Y-origin; the absolute vertical coordinate for the lower left corner of the image
	putc(width.a, file);	// width
	putc(width.b, file);
	putc(height.a, file);	// height
	putc(height.b, file);
	putc(32, file);			// pixel depth
	putc(0, file);			// image descriptor

	fclose(file);
	
	if (_verify) {
		FILE* f = fopen(fileName, "rb");
		struct Header header;
		
		if (!fread_header(&header, f)) {
			puts("[ERR] Verify output file failed (reading header).");
			fclose(f);
			return false;
		}
		
		print_header(&header);
		fclose(f);
	}
	
	return true;
}

// ~~~~~~~~~~

static bool fread_header(struct Header* _h, FILE* _file) {
	int c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->idLen = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->colorMapType = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->imageType = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->cmFirstEntryIndex = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->cmLen = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->cmEntrySize = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->originX = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->originY = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->width = c;
	
	if (fread(&c, sizeof(u16), 1, _file) != 1) return false;
	_h->height = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->pixelDepth = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->imageDescriptor = c;
	
	return true;
}

static void print_header(struct Header* _h) {
	printf(
		"( 0) idLen: %18d\n"
		"( 1) colorMapType: %11d\n"
		"( 2) imageType: %14d\n"
		"( 3) cmFirstEntryIndex: %6d\n"
		"( 4) cmLen: %18d\n"
		"( 5) cmEntrySize: %12d\n"
		"( 6) originX: %16d\n"
		"( 7) originY: %16d\n"
		"( 8) width: %18d\n"
		"( 9) height: %17d\n"
		"(10) pixelDepth: %13d\n"
		"(11) imageDescriptor: %8d\n\n",
		_h->idLen,
		_h->colorMapType,
		_h->imageType,
		_h->cmFirstEntryIndex,
		_h->cmLen,
		_h->cmEntrySize,
		_h->originX,
		_h->originY,
		_h->width,
		_h->height,
		_h->pixelDepth,
		_h->imageDescriptor);
}
