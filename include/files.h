#ifndef FILES_H_
#define FILES_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

/**
 * @brief The function reads the header, gets the number of registries in the file, 
 * reads and prints the registries one by one
 *
 * @param file_name file that will be opened
 */
void fileBinaryPrintAsCsv(char* file_name);

/**
 * @brief reads a csv file, using the data struct, stores the struct in RAM and
 * writes it to a binary file, one struct at time.
 *
 * @param csv_file_name csv file to be opened
 * @param binary_file_name binary file to be opened/created
 */
void fileReadCsvWriteBinary(char* csv_file_name, char* binary_file_name);

#endif // !FILES_H_