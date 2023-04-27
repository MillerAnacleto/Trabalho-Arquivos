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
int binHeaderBinaryWrite(FILE* binary_file, Bin_Header_t* header);

/**
 * @brief it reads the Header of the binary file
 *
 * @param bin_file file that will be read
 *
 * @return returns the filled header struct
 */
Bin_Header_t* binHeaderBinaryRead(FILE* binary_file);

#endif // !HEADER_H_