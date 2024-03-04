#include "targa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i) \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

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

static void print_header(struct Header* _h);
bool create_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height);

// comment this out if building by dds_test.bat
int main(void) { create_tga_image("tga_test_output", 511, 511); }

static void putnc(u8 c, size_t n, FILE* file) {
	for (size_t i = 0; i < n; i++) putc(c, file);
}

bool create_tga_image(const char* _fileName, const uint16_t _width, const uint16_t _height) {
	bool verify = true;
	
	size_t len = strlen(_fileName);
	char* fileName = malloc(sizeof(char) * len + sizeof(char) * 5);
	memcpy(fileName, _fileName, sizeof(char) * len);
	memcpy(fileName + len, ".tar", sizeof(char) * 5);
	
	FILE* file = fopen(fileName, "wb");
	if (file == NULL) {
		printf("[ERR] Failed to create/open file for write: %s\n", fileName);
		free(fileName);
		return false;
	}
	if (!verify) free(fileName);
	
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

	putc(0, file);				// u8		idLen;				// no image identification field
	putc(0, file);				// u8		colorMapType;		// indicates that (0) no color-map, (1) a color-map included
	putc(2, file);				// u8		imageType;			// uncompressed, True-color image
	putnc(0, 5, file);			// u16		cmFirstEntryIndex; 	// no color map, so no color map specification
								// u16		cmLen;
								// u8		cmEntrySize;
	putnc(0, 2, file);			// u16		originX;			// X-origin; the absolute horizontal coordinate for the lower left corner of the image
	putnc(0, 2, file);			// u16		originY;			// Y-origin; the absolute vertical coordinate for the lower left corner of the image
	putc(width.a, file);		// u16		width;				// width
	putc(width.b, file);
	putc(height.a, file);		// u16		height;				// height
	putc(height.b, file);
	putc(32, file);				// u8		pixelDepth;			// pixel depth
	putc(0, file);				// u8		imageDescriptor;	// image descriptor

	fputs("$END", file);
	
	fclose(file);
	
	if (verify) {
		FILE* f = fopen(fileName, "rb");
		
		size_t headerStructSize = sizeof(struct Header);
		printf("sizeof(struct Header): %lld byte\n", headerStructSize);
		int line = 0, prevLine = -1;
		for (int i = 0; i < (int)headerStructSize; i++) {
			printf(line % 2 == 0 ? AC_GREEN : AC_YELLOW);
			int c = getc(f);
			if (prevLine != line) {
				// printf("(%2d) %d ", line, c);
				printf("(%2d) " PRINTF_BINARY_PATTERN_INT8 " ", line, PRINTF_BYTE_TO_BINARY_INT8(c));
				prevLine = line;
			}
			else {
				// printf("%d ", c);
				printf(PRINTF_BINARY_PATTERN_INT8 " ", PRINTF_BYTE_TO_BINARY_INT8(c));
			}
			printf(AC_RESET);
			if (i == 0 || i == 1 || i == 2 || i == 4 || i == 6 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 16 || i >= 17) {
				line++;
				puts("");
			}
		}
		puts("--------------------");
		
		rewind(f);
		
		struct Header header;
		if (fread(&header, sizeof(header), 1, f) != 1) {
			puts("Read header failed.");
		}
		else print_header(&header);
		
		fclose(f);
	}
	
	return true;
}

// ~~~~~~~~~~

static void print_header(struct Header* _h) {
	u8* p = (u8*)_h;
	u16* p2;
	
	u8  l_idLen				= *p;	p++;
	u8  l_colorMapType		= *p;	p++;
	u8  l_imageType			= *p;	p++;	p2 = (u16*)p;
	u16 l_cmFirstEntryIndex	= *p2;	p2++;
	u16 l_cmLen				= *p2;	p2++;	p = (u8*)p2;
	u8  l_cmEntrySize		= *p;	p++;	p2 = (u16*)p;
	u16 l_originX			= *p2;	p2++;
	u16 l_originY			= *p2;	p2++;
	u16 l_width				= *p2;	p2++;
	u16 l_height			= *p2;	p2++;	p = (u8*)p2;
	u8  l_pixelDepth		= *p;	p++;
	u8  l_imageDescriptor	= *p;

	p = NULL;
	p2 = NULL;
	
	printf(
		AC_GREEN	"( 0) idLen: %18d (%d)\n"				AC_RESET
		AC_YELLOW	"( 1) colorMapType: %11d (%d)\n"		AC_RESET
		AC_GREEN	"( 2) imageType: %14d (%d)\n"			AC_RESET
		AC_YELLOW	"( 3) cmFirstEntryIndex: %6d (%d)\n"	AC_RESET
		AC_GREEN	"( 4) cmLen: %18d (%d)\n"				AC_RESET
		AC_YELLOW	"( 5) cmEntrySize: %12d (%d)\n"			AC_RESET
		AC_GREEN	"( 6) originX: %16d (%d)\n"				AC_RESET
		AC_YELLOW	"( 7) originY: %16d (%d)\n"				AC_RESET
		AC_GREEN	"( 8) width: %18d (%d)\n"				AC_RESET
		AC_YELLOW	"( 9) height: %17d (%d)\n"				AC_RESET
		AC_GREEN	"(10) pixelDepth: %13d (%d)\n"			AC_RESET
		AC_YELLOW	"(11) imageDescriptor: %8d (%d)\n\n"	AC_RESET,
		_h->idLen,				l_idLen,
		_h->colorMapType,		l_colorMapType,
		_h->imageType,			l_imageType,
		_h->cmFirstEntryIndex,	l_cmFirstEntryIndex,
		_h->cmLen,				l_cmLen,
		_h->cmEntrySize,		l_cmEntrySize,
		_h->originX,			l_originX,
		_h->originY,			l_originY,
		_h->width,				l_width,
		_h->height,				l_height,
		_h->pixelDepth,			l_pixelDepth,
		_h->imageDescriptor,	l_imageDescriptor);
	
	/* Output:
		( 0) idLen:                  0 (0)
		( 1) colorMapType:           0 (0)
		( 2) imageType:              2 (2)
		( 3) cmFirstEntryIndex:      0 (0)
		( 4) cmLen:                  0 (0)
		( 5) cmEntrySize:            0 (0)
		( 6) originX:                0 (0)
		( 7) originY:              511 (0)		<-- !?!?! (originY should be 0)
		( 8) width:                511 (511)	<-- !?!?!
		( 9) height:                32 (511)
		(10) pixelDepth:            36 (32)
		(11) imageDescriptor:       69 (0)		<-- looks like shifted, but this 0 is also missing
	*/
}
