#include "DDS.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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
	if (file == NULL) {
		printf(AC_RED "[ERR]" AC_RESET " Failed to open file: \"%s\"", filePath);
		fclose(file);
		return EXIT_FAILURE;
	}
	
	int exitCode = process_dds_file(file);
	
	fclose(file);
	
	return exitCode;
}



void print_compression_format(const enum DXTCompression cf) {
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

int process_dds_file(FILE* _file) {
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
	
	struct DDS_HEADER header;
	if (fread(&header, sizeof(struct DDS_HEADER), 1, _file) != 1) {
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
	
	print_dds_header(&header);
	
	if (header.dwHeight % 4 != 0 || header.dwWidth % 4 != 0) {
		puts(AC_RED "[ERR]" AC_RESET " Image width or/and height is/are not multiple of 4!");
		return EXIT_FAILURE;
	}
	
	enum DXTCompression cformat = DXTC_NONE;
	
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
	
	print_compression_format(cformat);
	
	struct DDS_HEADER_DXT10 header10;
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

void print_dds_pixelformat(struct DDS_PIXELFORMAT* _pf) {
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
		"dwABitMask: %lu\n",
		_pf->dwSize,
		_pf->dwFlags,
		_pf->dwFourCC,
		_pf->dwRGBBitCount,
		_pf->dwRBitMask,
		_pf->dwGBitMask,
		_pf->dwBBitMask,
		_pf->dwABitMask);

	printf(
		"----------\n"
		"FLAGS:\n"
		"DDPF_ALPHAPIXELS: %d\n"
		"DDPF_ALPHA: %d\n"
		"DDPF_FOURCC: %d\n"
		"DDPF_RGB: %d\n"
		"DDPF_YUV: %d\n"
		"DDPF_LUMINANCE: %d\n\n",
		(_pf->dwFlags & DDPF_ALPHAPIXELS) != 0 ? 1 : 0,
		(_pf->dwFlags & DDPF_ALPHA) != 0 ? 1 : 0,
		(_pf->dwFlags & DDPF_FOURCC) != 0 ? 1 : 0,
		(_pf->dwFlags & DDPF_RGB) != 0 ? 1 : 0,
		(_pf->dwFlags & DDPF_YUV) != 0 ? 1 : 0,
		(_pf->dwFlags & DDPF_LUMINANCE) != 0 ? 1 : 0);
}

void print_dds_header(struct DDS_HEADER* _h) {
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
		"dwCaps4: %lu\n",
		_h->dwSize,
		_h->dwFlags,
		_h->dwHeight,
		_h->dwWidth,
		_h->dwPitchOrLinearSize,
		_h->dwDepth,
		_h->dwMipMapCount,
		_h->dwCaps,
		_h->dwCaps2,
		_h->dwCaps3,
		_h->dwCaps4);
	
	printf(
		"----------\n"
		"FLAGS:\n"
		"DDSD_CAPS: %d\n"
		"DDSD_HEIGHT: %d\n"
		"DDSD_WIDTH: %d\n"
		"DDSD_PITCH: %d\n"
		"DDSD_PIXELFORMAT: %d\n"
		"DDSD_MIPMAPCOUNT: %d\n"
		"DDSD_LINEARSIZE: %d\n"
		"DDSD_DEPTH: %d\n\n",
		(_h->dwFlags & DDSD_CAPS) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_HEIGHT) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_WIDTH) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_PITCH) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_PIXELFORMAT) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_MIPMAPCOUNT) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_LINEARSIZE) != 0 ? 1 : 0,
		(_h->dwFlags & DDSD_DEPTH) != 0 ? 1 : 0);
	
	print_dds_pixelformat(&_h->ddspf);
}

void print_dds_header_dxt10(struct DDS_HEADER_DXT10* _h) {
	printf(
		"DDS_HEADER_DXT10\n"
		"----------------\n"
		"dxgiFormat: %d\n"
		"resourceDimension: %d\n"
		"miscFlag: %d\n"
		"arraySize: %d\n"
		"miscFlags2: %d\n\n",
		_h->dxgiFormat,
		_h->resourceDimension,
		_h->miscFlag,
		_h->arraySize,
		_h->miscFlags2);
}
