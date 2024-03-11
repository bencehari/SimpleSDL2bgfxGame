#ifndef KE_DDS_H
#define KE_DDS_H

#include <stdio.h>

#include "DDS_header.h"
#include "compression_formats.h"

#ifdef __cplusplus
extern "C" {
#endif

enum DDS_DataMeta {
	ONLY_HEADER		= 0,
	DXT10_HEADER	= 1
};

/**
 * @brief All information extracted from DDS meta data.
*/
struct DDS_Data {
	int meta;
	enum CompressionFormat cformat;
	struct DDS_HEADER header;
	struct DDS_HEADER_DXT10 headerDXT10;
};

/**
 * @brief Prints DDS file header.
 *
 * @param _file FILE* to target DDS file.
 *
 * @return Error code.
*/
extern int print_dds_file_info(FILE* _file);

/**
 * @brief Sets provided DDS file headers.
 *
 * @param _file FILE* to target DDS file.
 * @param _data DDS_Data* to set.
 *
 * @return Error code.
*/
extern int get_dds_file_info(FILE* _file, struct DDS_Data* _data);

/**
 * @brief Saves highest MIP level to targa.
 *
 * @param _file FILE* to target DDS file.
 * @param _name Name of the output file.
 *
 * @return Error code.
*/
extern int save_dds_file_to_targa(FILE* _file, const char* _name);

extern void print_dds_pixelformat(struct DDS_PIXELFORMAT* _pf);
extern void print_dds_header(struct DDS_HEADER* _h);
extern void print_dds_header_dxt10(struct DDS_HEADER_DXT10* _h);
extern void print_dds_data(struct DDS_Data* _d);

#ifdef __cplusplus
}
#endif

#endif // KE_DDS_H
