#ifndef KE_COMPRESSION_FORMATS_H
#define KE_COMPRESSION_FORMATS_H

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

#ifdef __cplusplus
}
#endif

#endif // KE_COMPRESSION_FORMATS_H
