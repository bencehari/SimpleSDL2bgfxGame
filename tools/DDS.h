#ifndef KE_DDS_H
#define KE_DDS_H

#include <stdio.h>

#include "DDS_header.h"
#include "DXT.h"


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
 * @param _header DDS_HEADER* to set.
 * @param _cformat Compression format enum extracted from FourCC.
 * @param _headerDXT10 DDS_HEADER_DXT10* to set, NULL if does not DXT10.
 *
 * @return Error code.
*/
extern int get_dds_file_info(FILE* _file, struct DDS_HEADER* _header, enum DXTCompression* _cformat, struct DDS_HEADER_DXT10* _headerDXT10);

/**
 * @brief Saves highest MIP level to targa.
 *
 * @param _file FILE* to target DDS file.
 *
 * @return Error code.
*/
extern int save_dds_file_to_targa(FILE* _file);

extern void print_dds_pixelformat(struct DDS_PIXELFORMAT* _pf);
extern void print_dds_header(struct DDS_HEADER* _h);
extern void print_dds_header_dxt10(struct DDS_HEADER_DXT10* _h);

#endif // KE_DDS_H
