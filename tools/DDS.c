#include "DDS.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "compression_formats.h"
#include "targa.h"

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

void print_compression_format(const enum CompressionFormat cf) {
	printf("Compression format: ");
	switch (cf) {
		case CF_NONE: puts("NONE"); break;
		case CF_DXT1: puts("DXT1"); break;
		case CF_DXT2: puts("DXT2"); break;
		case CF_DXT3: puts("DXT3"); break;
		case CF_DXT4: puts("DXT4"); break;
		case CF_DXT5: puts("DXT5"); break;
		case CF_DX10: puts("DX10"); break;
	}
}

int print_dds_file_info(FILE* _file) {
	struct DDS_Data data;
	if (get_dds_file_info(_file, &data) != EXIT_SUCCESS) return EXIT_FAILURE;
	
	print_dds_data(&data);
	
	return EXIT_SUCCESS;
}

int get_dds_file_info(FILE* _file, struct DDS_Data* _data) {
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
	
	if (fread(&_data->header, sizeof(struct DDS_HEADER), 1, _file) != 1) {
		puts(AC_RED "[ERR]" AC_RESET " Reading header failed.");
		return EXIT_FAILURE;
	}
	if (_data->header.dwSize != 124) {
		puts(AC_RED "[ERR]" AC_RESET " header.dwSize is not 124.");
		return EXIT_FAILURE;
	}
	if (_data->header.ddspf.dwSize != 32) {
		puts(AC_RED "[ERR]" AC_RESET " header.ddspf.dwSize is not 32");
		return EXIT_FAILURE;
	}
	
	if (_data->header.dwHeight % 4 != 0 || _data->header.dwWidth % 4 != 0) {
		puts(AC_RED "[ERR]" AC_RESET " Image width or/and height is/are not multiple of 4!");
		return EXIT_FAILURE;
	}
	
	_data->cformat = CF_NONE;
	
	if ((_data->header.ddspf.dwFlags & DDPF_FOURCC) != 0) {
		char fourCC[5];
		memcpy(fourCC, &_data->header.ddspf.dwFourCC, sizeof(char) * 4);
		fourCC[4] = '\0';
		
		if (strcmp(fourCC, "DXT1") == 0) _data->cformat = CF_DXT1;
		else if (strcmp(fourCC, "DXT2") == 0) _data->cformat = CF_DXT2;
		else if (strcmp(fourCC, "DXT3") == 0) _data->cformat = CF_DXT3;
		else if (strcmp(fourCC, "DXT4") == 0) _data->cformat = CF_DXT4;
		else if (strcmp(fourCC, "DXT5") == 0) _data->cformat = CF_DXT5;
		else if (strcmp(fourCC, "DX10") == 0) _data->cformat = CF_DX10;
		else {
			char fourCC0[5];
			memcpy(fourCC0, fourCC, 4);
			fourCC0[4] = '\0';
			printf(AC_RED "[ERR]" AC_RESET " Unknown compression format: %s.\n", fourCC0);
			return EXIT_FAILURE;
		}
	}
	
	if (_data->cformat == CF_DXT2 || _data->cformat == CF_DXT4) {
		printf(AC_RED "[ERR]" AC_RESET " '%s' compression format is not supported.", _data->cformat == CF_DXT2 ? "DXT2" : "DXT4");
		return EXIT_FAILURE;
	}
	
	if (_data->cformat == CF_DX10) {
		if (fread(&_data->headerDXT10, sizeof(struct DDS_HEADER_DXT10), 1, _file) != 1) {
			puts(AC_RED "[ERR]" AC_RESET " Reading header10 failed.");
			return EXIT_FAILURE;
		}
		
		_data->meta = DXT10_HEADER;
		
		puts(AC_YELLOW "[TODO]" AC_RESET " handle DX10!");
		return EXIT_SUCCESS;
	}
	
	return EXIT_SUCCESS;
}

int save_dds_file_to_targa(FILE* _file) {
	struct DDS_Data data;
	if (get_dds_file_info(_file, &data) != EXIT_SUCCESS) return EXIT_FAILURE;
	
	// TODO: implement
	
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
		"dwSize: %u\n"
		"dwFlags: %u\n"
		"dwFourCC: %u\n"
		"dwRGBBitCount: %u\n"
		"dwRBitMask: %u\n"
		"dwGBitMask: %u\n"
		"dwBBitMask: %u\n"
		"dwABitMask: %u\n",
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
		"dwSize: %u\n"
		"dwFlags: %u\n"
		"dwHeight: %u\n"
		"dwWidth: %u\n"
		"dwPitchOrLinearSize: %u\n"
		"dwDepth: %u\n"
		"dwMipMapCount: %u\n"
		"dwCaps: %u\n"
		"dwCaps2: %u\n"
		"dwCaps3: %u\n"
		"dwCaps4: %u\n",
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

void print_dds_data(struct DDS_Data* _d) {
	print_dds_header(&_d->header);
	print_compression_format(_d->cformat);
	if (_d->meta == DXT10_HEADER) {
		puts("");
		print_dds_header_dxt10(&_d->headerDXT10);
	}
}
