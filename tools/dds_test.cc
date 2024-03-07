#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dds_test.hh"
#include "targa.h"

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

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

enum CompressionFormat { NONE, DXT1, DXT2, DXT3, DXT4, DXT5, DX10 };

void printCompressinFormat(const CompressionFormat& cf) {
	printf("Compression format: ");
	switch (cf) {
		case CompressionFormat::NONE: puts("NONE"); break;
		case CompressionFormat::DXT1: puts("DXT1"); break;
		case CompressionFormat::DXT2: puts("DXT2"); break;
		case CompressionFormat::DXT3: puts("DXT3"); break;
		case CompressionFormat::DXT4: puts("DXT4"); break;
		case CompressionFormat::DXT5: puts("DXT5"); break;
		case CompressionFormat::DX10: puts("DX10"); break;
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
	
	CompressionFormat cformat { CompressionFormat::NONE };
	
	if ((header.ddspf.dwFlags & DDPF_FOURCC) != 0) {
		char fourCC[5];
		memcpy(fourCC, &header.ddspf.dwFourCC, sizeof(char) * 4);
		fourCC[4] = '\0';
		
		if (strcmp(fourCC, "DXT1") == 0) cformat = DXT1;
		else if (strcmp(fourCC, "DXT2") == 0) cformat = DXT2;
		else if (strcmp(fourCC, "DXT3") == 0) cformat = DXT3;
		else if (strcmp(fourCC, "DXT4") == 0) cformat = DXT4;
		else if (strcmp(fourCC, "DXT5") == 0) cformat = DXT5;
		else if (strcmp(fourCC, "DX10") == 0) cformat = DX10;
		else {
			puts(AC_RED "[ERR]" AC_RESET " Unknown compression format.");
			return EXIT_FAILURE;
		}
	}
	
	printCompressinFormat(cformat);
	
	DDS_HEADER_DXT10 header10;
	if (cformat == DX10) {
		if (fread(&header10, sizeof(header10), 1, _file) != 1) {
			puts(AC_RED "[ERR]" AC_RESET " Reading header10 failed.");
			return EXIT_FAILURE;
		}
		
		puts(AC_YELLOW "[TODO]" AC_RESET " handle DX10!");
		return EXIT_SUCCESS;
	}
	
	// TODO: create tga from image data
	
	return EXIT_SUCCESS;
}