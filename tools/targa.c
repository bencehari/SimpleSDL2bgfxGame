#include "targa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
	((byte) & 0x80 ? '1' : '0'), \
	((byte) & 0x40 ? '1' : '0'), \
	((byte) & 0x20 ? '1' : '0'), \
	((byte) & 0x10 ? '1' : '0'), \
	((byte) & 0x08 ? '1' : '0'), \
	((byte) & 0x04 ? '1' : '0'), \
	((byte) & 0x02 ? '1' : '0'), \
	((byte) & 0x01 ? '1' : '0') 

typedef uint8_t		ui8;
typedef uint16_t	ui16;

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
	
// cm prefix stands for Color-map
struct Header {
	ui8		idLen;
	ui8		colorMapType;
	ui8		imageType;
	ui8		cmEntrySize;		// Establishes the number of bits per entry. Typically 15, 16, 24 or 32-bit values are used.
								// (moved from after cmLen to avoid padding, as writing and reading are not done by the type)
	ui16	cmFirstEntryIndex;	// Index of the first color map entry.
	ui16	cmLen;				// Total number of color map entries included.
	ui16	originX;
	ui16	originY;
	ui16	width;
	ui16	height;
	ui8		pixelDepth;
	ui8		imageDescriptor;
};

static bool fread_header(struct Header* _h, FILE* _file);
static void print_header(struct Header* _h);

bool def_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height, const uint8_t* _colors) {
	return create_tga_image(_fileName, _width, _height, RIGHT_DOWN, _colors, false);
}

static void putnc(ui8 c, size_t n, FILE* file) {
	for (size_t i = 0; i < n; i++) putc(c, file);
}

bool create_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height,
					  const enum ColorDataDirection _dir, const uint8_t* _colors, const bool _verify) {
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

	ui8 dir = 0; // default: RIGHT_UP
	switch (_dir) {
		case LEFT_UP:		dir = 1 << 4; break;
		case RIGHT_DOWN:	dir = 1 << 5; break;
		case LEFT_DOWN:		dir = (1 << 5) | (1 << 4); break;
		default: break;
	}

	putc(0, file);								// no image identification field
	putc(0, file);								// indicates that (0) no color-map, (1) a color-map included
	putc(2, file);								// uncompressed, True-color image
	putnc(0, 5, file);							// no color map, so no color map specification
	putnc(0, 2, file);							// X-origin; the absolute horizontal coordinate for the lower left corner of the image
	putnc(0, 2, file);							// Y-origin; the absolute vertical coordinate for the lower left corner of the image
	fwrite(&_width, sizeof(ui16), 1, file);		// width
	fwrite(&_height, sizeof(ui16), 1, file);	// height
	putc(24, file);								// pixel depth
	putc(dir, file);							// image descriptor (5th bit set: data order starts from top left)
	
	// Image ID would be here, if the very first field would be 1
	// Color Map Data, same as Image ID with second field
	
	// Image Data (True-color)
	for (size_t i = 0; i < _width * _height * 3; i++) {
		putc(_colors[i], file);
	}
	
	// footer
	putnc(0, 8, file);										// 0-3: Extension Area Offset, 4-7: Developer Directory Offset
	fwrite("TRUEVISION-XFILE", sizeof(char), 16, file);		// The Signature
	putc('.', file);										// .
	putc('\0', file);										// Binary zero string terminator

	fclose(file);

	if (_verify && !verify_tga_image(fileName)) return false;
	
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
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
	_h->cmFirstEntryIndex = c;
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
	_h->cmLen = c;
	
	c = fgetc(_file); if (c < 0 || feof(_file)) return false;
	_h->cmEntrySize = c;
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
	_h->originX = c;
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
	_h->originY = c;
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
	_h->width = c;
	
	if (fread(&c, sizeof(ui16), 1, _file) != 1) return false;
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

bool verify_tga_image(const char* _filePath) {
	FILE* f = fopen(_filePath, "rb");
	struct Header header;
	
	if (!fread_header(&header, f)) {
		puts("[ERR] Verify output file failed (reading header).");
		fclose(f);
		return false;
	}
	
	print_header(&header);
	
	printf(
		"0 - 3 These bits specify the number of attribute bits per pixel.\n"
		"5 & 4 These bits are used to indicate the order in which pixel data is transferred from the file to the screen.\n"
		"      Bit 4 is for left-to-right ordering and bit 5 is for top-to-bottom ordering as shown below.\n"
		"6 - 7 Must be zero! (to insure future compatibility)\n"
		"76543210\n"
		"--------\n"
		BYTE_TO_BINARY_PATTERN "\n",
		BYTE_TO_BINARY(header.imageDescriptor));

	fseek(f, -18, SEEK_END);
	int c[16];
	for (int i = 0; i < 16; i++) c[i] = getc(f);
	char truevision[] = "TRUEVISION-XFILE";
	for (int i = 0; i < 16; i++) {
		if (c[i] != truevision[i]) {
			puts("[ERR] Original TGA not supported yet.");
			fclose(f);
			return false;
		}
	}
	
	fseek(f, 0, SEEK_END);
	long imageDataSize = ftell(f) - 18 - 26; // total - header - footer
	long targetSize = header.width * header.height * 3;
	if (imageDataSize != targetSize) {
		printf("[ERR] Image data size doesn't match: %ld (expected: %ld)\n", imageDataSize, targetSize);
		fclose(f);
		return false;
	}
	
	printf("File '%s' LOOKS GOOD\n", _filePath);
	
	fclose(f);
	return true;
}
