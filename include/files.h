#ifndef FILES_H_
#define FILES_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

#define EMPTY_INT_FIELD -1

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

/**
 * @brief creates an index file
 * 
 * @param binary_file_name binary file from which the index file is created
 * @param index_file_name name of the index file that will be created
 * @param parameter search parameter tha will be indexed in the index file
 * @param offset long int that holds the number of bytes for each valid struct read
 */
void fileIndexCreate(char* binary_file_name, char* index_file_name, int parameter, int64_t *offset);

/**
 * @brief reads an index file and creates a linked list array to handle collisions.
 * Each equal key is placed in a linked list above the previous key of that type.
 * 
 * @param index_filename name of index file to be opened 
 * @param parameter diferentiates the read of int to string
 * @return returns the index node array
 */
Index_Node_t** fileIndexRead(char* index_filename, int parameter);

int SearchBinaryFile(char* filename, char* index_file_name, int index_parameter);
int SearchDeleteBinaryFile(char* filename, char* index_file_name, int index_parameter);
int SearchUpdateBinaryFile(char* filename, char* index_file_name, int index_parameter);

int linearSearchBinaryFile(FILE* file, Bin_Header_t* header, Index_Data_t** array, int array_size, char print);
int linearDeleteBinaryFile(FILE* file, Bin_Header_t* header, Index_Data_t** array, int array_size);

int binarySearchIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index);
int binarySearchDeleteIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index);

char insertIntoBinaryFile(char* filename, char* index_file_name, int index_parameter);


#endif // !FILES_H_