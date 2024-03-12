#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "targa.h"

static uint8_t* createChessTable(uint8_t blockSize);

int main(int argc, char* argv[]) {
	if (argc == 1) {
		puts("No param provided, creating test chess table tga (40x40) file.");
		
		int blockSize = 5;
		uint8_t* chessTable = createChessTable(blockSize);
		
		if (chessTable != NULL) {
			def_tga_image("tga_test_output", blockSize * 8, blockSize * 8, chessTable);
			free(chessTable);
		}
		else return EXIT_FAILURE;
	}
	else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
		size_t len = strlen(argv[2]);
		if (len < 5) {
			puts("[ERR] Invalid file path.");
			return EXIT_FAILURE;
		}
		
		char* ext = argv[2] + len - 4;
		if (strcmp(ext, ".tga") != 0 && strcmp(ext, ".TGA") != 0) {
			puts("[ERR] File must have .tga or .TGA extension.");
			return EXIT_FAILURE;
		}
		
		if (!verify_tga_image(argv[2])) return EXIT_FAILURE;
	}
	else {
		puts(
			"MANUAL\n"
			"------\n"
			"EMPTY  -> creating test image.\n"
			"-i     -> prints info about, and verify tga image. Must provide path to tga file (\\w ext) as the next param.");
	}
	
	return EXIT_SUCCESS;
}

static uint8_t* createChessTable(uint8_t blockSize) {
	uint8_t* ret = malloc(sizeof(uint8_t) * blockSize * blockSize * 8 * 8 * 3);
	if (ret == NULL) {
		puts("[ERR] Allocate memory for chess table failed.");
		return NULL;
	}
	
	int rowLen = blockSize * 8 * 3;
	for (int row = 0; row < 8; row++) {
		uint8_t color = row % 2 == 0 ? 0x00 : 0xFF;

		for (int col = 0; col < 8; col++) {
			color = color == 0xFF ? 0x00 : 0xFF;

			int r0 = row * blockSize * rowLen;
			for (int r = r0; r < r0 + blockSize * rowLen; r += rowLen) {
				int c0 = col * blockSize * 3;
				for (int c = c0; c < c0 + blockSize * 3; c += 3) {
					for (int rgb = 0; rgb < 3; rgb++) {
						ret[r + c + rgb] = color;
					}
				}
			}
		}
	}
	
	return ret;
}
