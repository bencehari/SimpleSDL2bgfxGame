#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "dds_test.hh"
#include "DXT.h"
#include "targa.h"

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

typedef uint8_t BYTE;

int processDDSFile(FILE*& _file);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		puts(AC_RED "[ERR]" AC_RESET " No target file provided.");
		return EXIT_FAILURE;
	}
	
	char* filePath = argv[1];
	size_t filePathLen = strlen(filePath);
	{
		char* ext = filePath + filePathLen - 4;
		if (strcmp(ext, ".dds") != 0 && strcmp(ext, ".DDS") != 0) {
			puts(AC_RED "[ERR]" AC_RESET " Target is not a DDS file!");
			return EXIT_FAILURE;
		}
	}
	
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		printf(AC_RED "[ERR]" AC_RESET " Failed to open file: \"%s\"", filePath);
		fclose(file);
		return EXIT_FAILURE;
	}
	
	int exitCode = processDDSFile(file);
	
	fclose(file);
	
	return exitCode;
}



void printCompressionFormat(const DXTCompression& cf) {
	printf("Compression format: ");
	switch (cf) {
		case DXTC_NONE: puts("NONE"); break;
		case DXTC_DXT1: puts("DXT1"); break;
		case DXTC_DXT2: puts("DXT2"); break;
		case DXTC_DXT3: puts("DXT3"); break;
		case DXTC_DXT4: puts("DXT4"); break;
		case DXTC_DXT5: puts("DXT5"); break;
		case DXTC_DX10: puts("DX10"); break;
	}
}

int processDDSFile(FILE*& _file) {
	// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dx-graphics-dds-pguide
	
	{
		char dwMagic[5];
		if (fread(dwMagic, sizeof(char), 4, _file) != 4) {
			puts(AC_RED "[ERR]" AC_RESET " Rread dwMagic failed.");
			return EXIT_FAILURE;
		}
		
		dwMagic[4] = '\0';
		if (strcmp(dwMagic, "DDS ") != 0) {
			puts(AC_RED "[ERR]" AC_RESET " dwMagic is not 'DDS '.");
			return EXIT_FAILURE;
		}
	}
	
	DDS_HEADER header;
	if (fread(&header, sizeof(DDS_HEADER), 1, _file) != 1) {
		puts(AC_RED "[ERR]" AC_RESET " Reading header failed.");
		return EXIT_FAILURE;
	}
	if (header.dwSize != 124) {
		puts(AC_RED "[ERR]" AC_RESET " header.dwSize is not 124.");
		return EXIT_FAILURE;
	}
	if (header.ddspf.dwSize != 32) {
		puts(AC_RED "[ERR]" AC_RESET " header.ddspf.dwSize is not 32");
		return EXIT_FAILURE;
	}
	
	header.print();
	
	if (header.dwHeight % 4 != 0 || header.dwWidth % 4 != 0) {
		puts(AC_RED "[ERR]" AC_RESET " Image width or/and height is/are not multiple of 4!");
		return EXIT_FAILURE;
	}
	
	DXTCompression cformat { DXTC_NONE };
	
	if ((header.ddspf.dwFlags & DDPF_FOURCC) != 0) {
		char fourCC[5];
		memcpy(fourCC, &header.ddspf.dwFourCC, sizeof(char) * 4);
		fourCC[4] = '\0';
		
		if (strcmp(fourCC, "DXT1") == 0) cformat = DXTC_DXT1;
		else if (strcmp(fourCC, "DXT2") == 0) cformat = DXTC_DXT2;
		else if (strcmp(fourCC, "DXT3") == 0) cformat = DXTC_DXT3;
		else if (strcmp(fourCC, "DXT4") == 0) cformat = DXTC_DXT4;
		else if (strcmp(fourCC, "DXT5") == 0) cformat = DXTC_DXT5;
		else if (strcmp(fourCC, "DX10") == 0) cformat = DXTC_DX10;
		else {
			puts(AC_RED "[ERR]" AC_RESET " Unknown compression format.");
			return EXIT_FAILURE;
		}
	}
	
	if (cformat == DXTC_DXT2 || cformat == DXTC_DXT4) {
		printf(AC_RED "[ERR]" AC_RESET " '%s' compression format is not supported.", cformat == DXTC_DXT2 ? "DXT2" : "DXT4");
		return EXIT_FAILURE;
	}
	
	printCompressionFormat(cformat);
	
	DDS_HEADER_DXT10 header10;
	if (cformat == DXTC_DX10) {
		if (fread(&header10, sizeof(header10), 1, _file) != 1) {
			puts(AC_RED "[ERR]" AC_RESET " Reading header10 failed.");
			return EXIT_FAILURE;
		}
		
		puts(AC_YELLOW "[TODO]" AC_RESET " handle DX10!");
		return EXIT_SUCCESS;
	}
	
	// TEXTURE
	// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-file-layout-for-textures
	
	// DDSD_* <- header.dwFlags
	// DDPF_* <- header.ddspf.dwFlags
	
	// for an uncompressed texture, use the DDSD_PITCH and DDPF_RGB flags.
	
	// for a compressed texture, use the DDSD_LINEARSIZE and DDPF_FOURCC flags.
	
	// for a mipmapped texture, use the DDSD_MIPMAPCOUNT, DDSCAPS_MIPMAP, and DDSCAPS_COMPLEX flags also as well as the mipmap count member.
	// if mipmaps are generated, all levels down to 1-by-1 are usually written.
	// for a compressed texture, the size of each mipmap level image is typically one-fourth the size of the previous,
	// with a minimum of 8 (DXT1) or 16 (DXT2-5) bytes (for square textures).
	// formula to calculate the size of each level for a non-square texture:
	// max(1, ( (width + 3) / 4 ) ) x max(1, ( (height + 3) / 4 ) ) x 8(DXT1) or 16(DXT2-5)
	
	return EXIT_SUCCESS;
}