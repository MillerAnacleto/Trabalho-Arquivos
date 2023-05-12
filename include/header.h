#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

/**
 * @brief writes the header struct fields in a binary file
 *
 * @param binary_file file in which the fields will be written
 * @param header struct from which the filds will be read
 * @return int controls the number of writes to verify if all fields were written
 */
int binHeaderBinaryWrite(FILE* binary_file, Bin_Header_t* header, int64_t offset);

/**
 * @brief it reads the Header of the binary file
 *
 * @param bin_file file that will be read
 *
 * @return returns the filled header struct
 */
Bin_Header_t* binHeaderRead(FILE* binary_file);

/**
 * @brief reads the index header
 * 
 * @param index 
 * @return Index_Header_t* 
 */
Index_Header_t* indexHeaderRead(FILE* index);

/**
 * @brief writes index header
 * 
 * @param binary_file 
 * @param header 
 * @return int 
 */
int indexHeaderWrite(FILE* binary_file, Index_Header_t* header, int64_t offset);

Index_Header_t* emptyIndexHeaderCreate();

int indexHeaderReWrite(FILE* file, Index_Header_t* header);

#endif // !HEADER_H_