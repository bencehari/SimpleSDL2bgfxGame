#ifndef KE_COMPRESSION_FORMATS_H
#define KE_COMPRESSION_FORMATS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

enum CompressionFormat {
	CF_NONE,
	CF_DXT1,
	CF_DXT2,
	CF_DXT3,
	CF_DXT4,
	CF_DXT5,
	CF_DX10
};

static inline void print_compression_format(const enum CompressionFormat cf) {
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

#ifdef __cplusplus
}
#endif

#endif // KE_COMPRESSION_FORMATS_H
