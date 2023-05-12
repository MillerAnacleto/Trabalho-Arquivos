#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

#define DATA_BASE_SIZE 32
#define HEADER_SIZE 17

/**
 * @brief reads a csv line and saves it into a data struct
 *
 * @param csv_file file being read
 * @param size_array keeps the variable strings size, to be summated
 * for calculating the offset byte
 *
 * @return Bin_Data_t* returns the filled data struct
 */
Bin_Data_t* dataCsvRead(FILE* csv_file, int* size_array);
Bin_Data_t* dataRead(int* size_array);

/**
 * @brief reads one registry of the file
 *
 * @param bin_file file that will be read
 * @return Bin_Data_t* the data read from the file
 */
Bin_Data_t* dataBinaryRead(FILE* bin_file);

/**
 * @brief writes the data struct fields into a file in binary
 * write mode
 *
 * @param binary_file file opened in binary write
 * @param data struct from which the filds will be read
 * @param size_array array that keeps the variable strings size, to write
 * exactly the strings size.
 *
 * @return int controls the number of writes to verify if all fields were written
 */
int dataBinaryWrite(FILE* binary_file, Bin_Data_t* data, int* size_array, int64_t offset);

/**
 * @brief Receives a registry and prints its value following the given rules
 *
 * @param data data that will be printed
 */
void dataPrintCsvStyle(Bin_Data_t* data);

void dataIndexArraySort(Index_Node_t** index_array, int unique_node_num, int parameter);

int dataIndexArrayWrite(FILE* index_file, Index_Node_t** index_array, int unique_node_num);

void dataIndexArrayIntRead(FILE* index, Index_Node_t** array, int size, int* unique_node_num);

void dataIndexArrayStrRead(FILE* index, Index_Node_t** array, int size, int* unique_node_num);

/**
 * @brief compares a binary data struct with all search parameters given
 * 
 * @param bin_data binary data candidate to be the search answer
 * @param parameter_array holds each field of the search parameters
 * @param parameter_num number of search parameters given
 * @return 1 case the binary data has all search parameters, 0 otherwise 
 */
char dataParamCompare(Bin_Data_t* bin_data, Index_Data_t** parameter_array, int parameter_num);

/**
 * @brief returns the int field specified by parameter, id case param == 0
 * or article number, case param == 1 
 * 
 * @param data binary data from wich the field is returned
 * @param param specifier to determine which field is returned
 * @return id (param == 0) or article number (param == 1)
 */
int dataGetIntField(Bin_Data_t* data, int param);

/**
 * @brief returns the string field specified by parameter, date case param == 2
 * description case param == 3, place case param == 4, brand case param == 5.
 * 
 * @param data binary data from wich the field is returned
 * @param param  specifier to determine which field is returned
 * @return  
 */
char* dataGetStrField(Bin_Data_t* data, int param);

void dataMarkDeleted(FILE* binary_file, int64_t offset);

#endif // !DATA_H_