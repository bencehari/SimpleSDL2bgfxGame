#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned long DWORD;

// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-pixelformat
struct DDS_PIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
	
	void print() {
		printf(
			"DDS_PIXELFORMAT\n"
			"---------------\n"
			"dwSize: %lu\n"
			"dwFlags: %lu\n"
			"dwFourCC: %lu\n"
			"dwRGBBitCount: %lu\n"
			"dwRBitMask: %lu\n"
			"dwGBitMask: %lu\n"
			"dwBBitMask: %lu\n"
			"dwABitMask: %lu\n\n",
			dwSize,
			dwFlags,
			dwFourCC,
			dwRGBBitCount,
			dwRBitMask,
			dwGBitMask,
			dwBBitMask,
			dwABitMask);
	}
};

// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-header
struct DDS_HEADER {
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwHeight;
	DWORD           dwWidth;
	DWORD           dwPitchOrLinearSize;
	DWORD           dwDepth;
	DWORD           dwMipMapCount;
	DWORD           dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	DWORD           dwCaps;
	DWORD           dwCaps2;
	DWORD           dwCaps3;
	DWORD           dwCaps4;
	DWORD           dwReserved2;
	
	void print() {
		printf(
			"DDS_HEADER\n"
			"----------\n"
			"dwSize: %lu\n"
			"dwFlags: %lu\n"
			"dwHeight: %lu\n"
			"dwWidth: %lu\n"
			"dwPitchOrLinearSize: %lu\n"
			"dwDepth: %lu\n"
			"dwMipMapCount: %lu\n"
			"dwCaps: %lu\n"
			"dwCaps2: %lu\n"
			"dwCaps3: %lu\n"
			"dwCaps4: %lu\n\n",
			dwSize,
			dwFlags,
			dwHeight,
			dwWidth,
			dwPitchOrLinearSize,
			dwDepth,
			dwMipMapCount,
			dwCaps,
			dwCaps2,
			dwCaps3,
			dwCaps4);
	}
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		puts("[ERR] No target file provided.");
		return EXIT_FAILURE;
	}
	
	char* filePath = argv[1];
	size_t filePathLen = strlen(filePath);
	{
		char* ext = filePath + filePathLen - 4;
		if (strcmp(ext, ".dds") != 0 && strcmp(ext, ".DDS") != 0) {
			puts("[ERR] Target is not a dds file!");
			return EXIT_FAILURE;
		}
	}
	
	FILE* file = fopen(filePath, "rb");
	if (file == nullptr) {
		printf("[ERR] Failed to open file: \"%s\"", filePath);
		return EXIT_FAILURE;
	}
	
	// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dx-graphics-dds-pguide
	
	if (getc(file) != 'D' || getc(file) != 'D' || getc(file) != 'S') {
		puts("[ERR] Parse error: does not started with DDS.");
		return EXIT_FAILURE;
	}
	getc(file); // skip the '20' part of '0x20534444'
	
	DDS_HEADER header;
	fread(&header, sizeof(DDS_HEADER), 1, file);
	header.print();
	header.ddspf.print();
	
	fclose(file);
	
	return EXIT_SUCCESS;
}