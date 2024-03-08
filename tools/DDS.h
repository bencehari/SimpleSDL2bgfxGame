#ifndef DDS_TEST_H
#define DDS_TEST_H

#include <stdio.h>

typedef unsigned long	DWORD;
typedef unsigned int	UINT;

///////////////////////////////////////////////////////////////////////////////
// Pixelformat
///////////////////////////////////////////////////////////////////////////////
// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-pixelformat
enum DDS_PIXELFORMAT_DW_FLAGS {
	// Texture contains alpha data; dwRGBAlphaBitMask contains valid data.
	DDPF_ALPHAPIXELS		= 0x1,
	// Used in some older DDS files for alpha channel only uncompressed data
	// (dwRGBBitCount contains the alpha channel bitcount; dwABitMask contains valid data).
	DDPF_ALPHA				= 0x2,
	// Texture contains compressed RGB data; dwFourCC contains valid data.
	DDPF_FOURCC				= 0x4,
	// Texture contains uncompressed RGB data; dwRGBBitCount and the RGB masks
	// (dwRBitMask, dwGBitMask, dwBBitMask) contain valid data.
	DDPF_RGB				= 0x40,
	// Used in some older DDS files for YUV uncompressed data
	// (dwRGBBitCount contains the YUV bit count; dwRBitMask contains the Y mask,
	// dwGBitMask contains the U mask, dwBBitMask contains the V mask).
	DDPF_YUV				= 0x200,
	// Used in some older DDS files for single channel color uncompressed data
	// (dwRGBBitCount contains the luminance channel bit count;
	// dwRBitMask contains the channel mask).
	// Can be combined with DDPF_ALPHAPIXELS for a two channel DDS file.
	DDPF_LUMINANCE			= 0x20000
};

struct DDS_PIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};

///////////////////////////////////////////////////////////////////////////////
// Header
///////////////////////////////////////////////////////////////////////////////
// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-header
enum DDS_HEADER_DW_FLAGS {
	DDSD_CAPS			= 0x1,			// Required in every .dds file.
	DDSD_HEIGHT			= 0x2,			// Required in every .dds file.
	DDSD_WIDTH			= 0x4,			// Required in every .dds file.
	DDSD_PITCH			= 0x8,			// Required when pitch is provided for an uncompressed texture.
	DDSD_PIXELFORMAT	= 0x1000,		// Required in every .dds file.
	DDSD_MIPMAPCOUNT	= 0x20000,		// Required in a mipmapped texture.
	DDSD_LINEARSIZE		= 0x80000,		// Required when pitch is provided for a compressed texture.
	DDSD_DEPTH			= 0x800000		// Required in a depth texture.
};

struct DDS_HEADER {
	DWORD					dwSize;
	DWORD					dwFlags;
	DWORD					dwHeight;
	DWORD					dwWidth;
	DWORD					dwPitchOrLinearSize;
	DWORD					dwDepth;
	DWORD					dwMipMapCount;
	DWORD					dwReserved1[11];
	struct DDS_PIXELFORMAT	ddspf;
	DWORD					dwCaps;
	DWORD					dwCaps2;
	DWORD					dwCaps3;
	DWORD					dwCaps4;
	DWORD					dwReserved2;
};

/*
NOTE (LOL)
----------
When you write .dds files, you should set the DDSD_CAPS and DDSD_PIXELFORMAT flags,
and for mipmapped textures you should also set the DDSD_MIPMAPCOUNT flag.
However, when you read a .dds file, you should not rely on the DDSD_CAPS, DDSD_PIXELFORMAT,
and DDSD_MIPMAPCOUNT flags being set because some writers of such a file might not set these flags.
*/

const int DDS_HEADER_FLAGS_TEXTURE = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
const int DDS_HEADER_FLAGS_MIPMAP = DDSD_MIPMAPCOUNT;
const int DDS_HEADER_FLAGS_VOLUME = DDSD_DEPTH;
const int DDS_HEADER_FALGS_PITCH = DDSD_PITCH;
const int DDS_HEADER_FALGS_LINEARSIZE = DDSD_LINEARSIZE;

enum DDS_HEADER_DW_CAPS {
	// Optional; must be used on any file that contains more than one surface
	// (a mipmap, a cubic environment map, or mipmapped volume texture).
	DDSCAPS_COMPLEX		= 0x8,
	// Optional; should be used for a mipmap.
	DDSCAPS_MIPMAP		= 0x400000,
	// Required
	DDSCAPS_TEXTURE		= 0x1000
};

const int DDS_SURFACE_FLAGS_MIPMAP = DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
const int DDS_SURFACE_FLAGS_TEXTURE = DDSCAPS_TEXTURE;
const int DDS_SURFACE_FLAGS_CUBEMAP = DDSCAPS_COMPLEX;

/*
NOTE (LOL)
----------
When you write .dds files, you should set the DDSCAPS_TEXTURE flag,
and for multiple surfaces you should also set the DDSCAPS_COMPLEX flag.
However, when you read a .dds file, you should not rely on the DDSCAPS_TEXTURE and
DDSCAPS_COMPLEX flags being set because some writers of such a file might not set these flags.
*/

enum DDS_HEADER_DW_CAPS2 {
	DDSCAPS2_CUBEMAP				= 0x200,		// Required for a cube map.
	DDSCAPS2_CUBEMAP_POSITIVEX		= 0x400,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_CUBEMAP_NEGATIVEX		= 0x800,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_CUBEMAP_POSITIVEY		= 0x1000,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_CUBEMAP_NEGATIVEY		= 0x2000,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_CUBEMAP_POSITIVEZ		= 0x4000,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_CUBEMAP_NEGATIVEZ		= 0x8000,		// Required when these surfaces are stored in a cube map.
	DDSCAPS2_VOLUME					= 0x200000		// Required for a volume texture.
};

const int DDS_CUBEMAP_POSITIVEX = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX;
const int DDS_CUBEMAP_NEGATIVEX = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX;
const int DDS_CUBEMAP_POSITIVEY = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY;
const int DDS_CUBEMAP_NEGATIVEY = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY;
const int DDS_CUBEMAP_POSITIVEZ = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ;
const int DDS_CUBEMAP_NEGATIVEZ = DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ;
const int DDS_CUBEMAP_ALLFACES =
	DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | DDS_CUBEMAP_POSITIVEY |
	DDS_CUBEMAP_NEGATIVEY | DDS_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ;
const int DDS_FLAGS_VOLUME = DDSCAPS2_VOLUME;

///////////////////////////////////////////////////////////////////////////////
// Header DXT10
///////////////////////////////////////////////////////////////////////////////
// -- DXGI format
// -- https://learn.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format
enum DXGI_FORMAT {
	DXGI_FORMAT_UNKNOWN										= 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS						= 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT							= 2,
	DXGI_FORMAT_R32G32B32A32_UINT							= 3,
	DXGI_FORMAT_R32G32B32A32_SINT							= 4,
	DXGI_FORMAT_R32G32B32_TYPELESS							= 5,
	DXGI_FORMAT_R32G32B32_FLOAT								= 6,
	DXGI_FORMAT_R32G32B32_UINT								= 7,
	DXGI_FORMAT_R32G32B32_SINT								= 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS						= 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT							= 10,
	DXGI_FORMAT_R16G16B16A16_UNORM							= 11,
	DXGI_FORMAT_R16G16B16A16_UINT							= 12,
	DXGI_FORMAT_R16G16B16A16_SNORM							= 13,
	DXGI_FORMAT_R16G16B16A16_SINT							= 14,
	DXGI_FORMAT_R32G32_TYPELESS								= 15,
	DXGI_FORMAT_R32G32_FLOAT								= 16,
	DXGI_FORMAT_R32G32_UINT									= 17,
	DXGI_FORMAT_R32G32_SINT									= 18,
	DXGI_FORMAT_R32G8X24_TYPELESS							= 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT						= 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS					= 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT						= 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS						= 23,
	DXGI_FORMAT_R10G10B10A2_UNORM							= 24,
	DXGI_FORMAT_R10G10B10A2_UINT							= 25,
	DXGI_FORMAT_R11G11B10_FLOAT								= 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS							= 27,
	DXGI_FORMAT_R8G8B8A8_UNORM								= 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB							= 29,
	DXGI_FORMAT_R8G8B8A8_UINT								= 30,
	DXGI_FORMAT_R8G8B8A8_SNORM								= 31,
	DXGI_FORMAT_R8G8B8A8_SINT								= 32,
	DXGI_FORMAT_R16G16_TYPELESS								= 33,
	DXGI_FORMAT_R16G16_FLOAT								= 34,
	DXGI_FORMAT_R16G16_UNORM								= 35,
	DXGI_FORMAT_R16G16_UINT									= 36,
	DXGI_FORMAT_R16G16_SNORM								= 37,
	DXGI_FORMAT_R16G16_SINT									= 38,
	DXGI_FORMAT_R32_TYPELESS								= 39,
	DXGI_FORMAT_D32_FLOAT									= 40,
	DXGI_FORMAT_R32_FLOAT									= 41,
	DXGI_FORMAT_R32_UINT									= 42,
	DXGI_FORMAT_R32_SINT									= 43,
	DXGI_FORMAT_R24G8_TYPELESS								= 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT							= 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS						= 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT						= 47,
	DXGI_FORMAT_R8G8_TYPELESS								= 48,
	DXGI_FORMAT_R8G8_UNORM									= 49,
	DXGI_FORMAT_R8G8_UINT									= 50,
	DXGI_FORMAT_R8G8_SNORM									= 51,
	DXGI_FORMAT_R8G8_SINT									= 52,
	DXGI_FORMAT_R16_TYPELESS								= 53,
	DXGI_FORMAT_R16_FLOAT									= 54,
	DXGI_FORMAT_D16_UNORM									= 55,
	DXGI_FORMAT_R16_UNORM									= 56,
	DXGI_FORMAT_R16_UINT									= 57,
	DXGI_FORMAT_R16_SNORM									= 58,
	DXGI_FORMAT_R16_SINT									= 59,
	DXGI_FORMAT_R8_TYPELESS									= 60,
	DXGI_FORMAT_R8_UNORM									= 61,
	DXGI_FORMAT_R8_UINT										= 62,
	DXGI_FORMAT_R8_SNORM									= 63,
	DXGI_FORMAT_R8_SINT										= 64,
	DXGI_FORMAT_A8_UNORM									= 65,
	DXGI_FORMAT_R1_UNORM									= 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP							= 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM								= 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM								= 69,
	DXGI_FORMAT_BC1_TYPELESS								= 70,
	DXGI_FORMAT_BC1_UNORM									= 71,
	DXGI_FORMAT_BC1_UNORM_SRGB								= 72,
	DXGI_FORMAT_BC2_TYPELESS								= 73,
	DXGI_FORMAT_BC2_UNORM									= 74,
	DXGI_FORMAT_BC2_UNORM_SRGB								= 75,
	DXGI_FORMAT_BC3_TYPELESS								= 76,
	DXGI_FORMAT_BC3_UNORM									= 77,
	DXGI_FORMAT_BC3_UNORM_SRGB								= 78,
	DXGI_FORMAT_BC4_TYPELESS								= 79,
	DXGI_FORMAT_BC4_UNORM									= 80,
	DXGI_FORMAT_BC4_SNORM									= 81,
	DXGI_FORMAT_BC5_TYPELESS								= 82,
	DXGI_FORMAT_BC5_UNORM									= 83,
	DXGI_FORMAT_BC5_SNORM									= 84,
	DXGI_FORMAT_B5G6R5_UNORM								= 85,
	DXGI_FORMAT_B5G5R5A1_UNORM								= 86,
	DXGI_FORMAT_B8G8R8A8_UNORM								= 87,
	DXGI_FORMAT_B8G8R8X8_UNORM								= 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM					= 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS							= 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB							= 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS							= 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB							= 93,
	DXGI_FORMAT_BC6H_TYPELESS								= 94,
	DXGI_FORMAT_BC6H_UF16									= 95,
	DXGI_FORMAT_BC6H_SF16									= 96,
	DXGI_FORMAT_BC7_TYPELESS								= 97,
	DXGI_FORMAT_BC7_UNORM									= 98,
	DXGI_FORMAT_BC7_UNORM_SRGB								= 99,
	DXGI_FORMAT_AYUV										= 100,
	DXGI_FORMAT_Y410										= 101,
	DXGI_FORMAT_Y416										= 102,
	DXGI_FORMAT_NV12										= 103,
	DXGI_FORMAT_P010										= 104,
	DXGI_FORMAT_P016										= 105,
	DXGI_FORMAT_420_OPAQUE									= 106,
	DXGI_FORMAT_YUY2										= 107,
	DXGI_FORMAT_Y210										= 108,
	DXGI_FORMAT_Y216										= 109,
	DXGI_FORMAT_NV11										= 110,
	DXGI_FORMAT_AI44										= 111,
	DXGI_FORMAT_IA44										= 112,
	DXGI_FORMAT_P8											= 113,
	DXGI_FORMAT_A8P8										= 114,
	DXGI_FORMAT_B4G4R4A4_UNORM								= 115,
	DXGI_FORMAT_P208										= 130,
	DXGI_FORMAT_V208										= 131,
	DXGI_FORMAT_V408										= 132,
	DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE,
	DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE,
	DXGI_FORMAT_FORCE_UINT									= 0xffffffff
};

// https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dds-header-dxt10
enum D3D11_RESOURCE_DIMENSION {
	D3D11_RESOURCE_DIMENSION_UNKNOWN		= 0,	// Resource is of unknown type.
	D3D11_RESOURCE_DIMENSION_BUFFER			= 1,	// Resource is a buffer.
	D3D11_RESOURCE_DIMENSION_TEXTURE1D		= 2,	// Resource is a 1D texture.
	D3D11_RESOURCE_DIMENSION_TEXTURE2D		= 3,	// Resource is a 2D texture.
	D3D11_RESOURCE_DIMENSION_TEXTURE3D		= 4		// Resource is a 3D texture.
};

struct DDS_HEADER_DXT10 {
	enum DXGI_FORMAT				dxgiFormat;
	enum D3D11_RESOURCE_DIMENSION	resourceDimension;
	UINT							miscFlag;
	UINT							arraySize;
	UINT							miscFlags2;
};

enum DDS_HEADER_DXT10_MISCFLAG {
	// Indicates a 2D texture is a cube-map texture.
	DDS_RESOURCE_MISC_TEXTURECUBE		= 0x4
};

enum DDS_HEADER_DXT10_MISCFLAGS2 {
	// Alpha channel content is unknown. This is the value for legacy files,
	// which typically is assumed to be 'straight' alpha.
	DDS_ALPHA_MODE_UNKNOWN				= 0x0,
	// Any alpha channel content is presumed to use straight alpha.
	DDS_ALPHA_MODE_STRAIGHT				= 0x1,
	// Any alpha channel content is using premultiplied alpha.
	// The only legacy file formats that indicate this information are 'DX2' and 'DX4'.
	DDS_ALPHA_MODE_PREMULTIPLIED		= 0x2,
	// Any alpha channel content is all set to fully opaque.
	DDS_ALPHA_MODE_OPAQUE				= 0x3,
	// Any alpha channel content is being used as a 4th channel and is not intended
	// to represent transparency (straight or premultiplied).
	DDS_ALPHA_MODE_CUSTOM				= 0x4
};

extern int process_dds_file(FILE* _file);

extern void print_dds_pixelformat(struct DDS_PIXELFORMAT* _pf);
extern void print_dds_header(struct DDS_HEADER* _h);
extern void print_dds_header_dxt10(struct DDS_HEADER_DXT10* _h);

#endif // DDS_TEST_H
