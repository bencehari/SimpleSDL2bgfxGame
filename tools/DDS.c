#include "DDS.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
	
	int exitCode = EXIT_SUCCESS;
	
	if (argc == 2) {
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
		
		exitCode = print_dds_file_info(file);
	
		fclose(file);
	}
	else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
		// TODO: implement
	}
	else {
		int len = strlen(argv[0]);
		int i = len - 1;
		for (; i >= 0; i--) {
			if (argv[0][i] == '\\' || argv[0][i] == '/') break;
		}
		if (i != 0) i++;
		
		printf(
			"Usage:\n"
			"------\n"
			"%s <path-of-dds-image>.dds        Provide the file.\n"
			"%s -f <path-of-parent-folder>     Provide all dds files in folder.\n",
			argv[0] + i, argv[0] + i);
	}
	
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

int print_dds_file_info(FILE* _file) {
	struct DDS_HEADER header;
	enum DXTCompression cformat;
	struct DDS_HEADER_DXT10 headerDXT10;
	
	if (get_dds_file_info(_file, &header, &cformat, &headerDXT10) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}
	
	print_dds_header(&header);
	print_compression_format(cformat);
	
	return EXIT_SUCCESS;
}

int get_dds_file_info(FILE* _file, struct DDS_HEADER* _header, enum DXTCompression* _cformat, struct DDS_HEADER_DXT10* _headerDXT10) {
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
	
	if (fread(_header, sizeof(struct DDS_HEADER), 1, _file) != 1) {
		puts(AC_RED "[ERR]" AC_RESET " Reading header failed.");
		return EXIT_FAILURE;
	}
	if (_header->dwSize != 124) {
		puts(AC_RED "[ERR]" AC_RESET " header.dwSize is not 124.");
		return EXIT_FAILURE;
	}
	if (_header->ddspf.dwSize != 32) {
		puts(AC_RED "[ERR]" AC_RESET " header.ddspf.dwSize is not 32");
		return EXIT_FAILURE;
	}
	
	if (_header->dwHeight % 4 != 0 || _header->dwWidth % 4 != 0) {
		puts(AC_RED "[ERR]" AC_RESET " Image width or/and height is/are not multiple of 4!");
		return EXIT_FAILURE;
	}
	
	*_cformat = DXTC_NONE;
	
	if ((_header->ddspf.dwFlags & DDPF_FOURCC) != 0) {
		char fourCC[5];
		memcpy(fourCC, &_header->ddspf.dwFourCC, sizeof(char) * 4);
		fourCC[4] = '\0';
		
		if (strcmp(fourCC, "DXT1") == 0) *_cformat = DXTC_DXT1;
		else if (strcmp(fourCC, "DXT2") == 0) *_cformat = DXTC_DXT2;
		else if (strcmp(fourCC, "DXT3") == 0) *_cformat = DXTC_DXT3;
		else if (strcmp(fourCC, "DXT4") == 0) *_cformat = DXTC_DXT4;
		else if (strcmp(fourCC, "DXT5") == 0) *_cformat = DXTC_DXT5;
		else if (strcmp(fourCC, "DX10") == 0) *_cformat = DXTC_DX10;
		else {
			puts(AC_RED "[ERR]" AC_RESET " Unknown compression format.");
			return EXIT_FAILURE;
		}
	}
	
	if (*_cformat == DXTC_DXT2 || *_cformat == DXTC_DXT4) {
		printf(AC_RED "[ERR]" AC_RESET " '%s' compression format is not supported.", *_cformat == DXTC_DXT2 ? "DXT2" : "DXT4");
		return EXIT_FAILURE;
	}
	
	if (*_cformat == DXTC_DX10) {
		if (fread(&_headerDXT10, sizeof(_headerDXT10), 1, _file) != 1) {
			puts(AC_RED "[ERR]" AC_RESET " Reading header10 failed.");
			return EXIT_FAILURE;
		}
		
		puts(AC_YELLOW "[TODO]" AC_RESET " handle DX10!");
		return EXIT_SUCCESS;
	}
	else _headerDXT10 = NULL;
	
	return EXIT_SUCCESS;
}

int save_dds_file_to_targa(FILE* _file) {
	struct DDS_HEADER header;
	enum DXTCompression cformat;
	struct DDS_HEADER_DXT10 headerDXT10;
	
	if (get_dds_file_info(_file, &header, &cformat, &headerDXT10) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}
	
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
