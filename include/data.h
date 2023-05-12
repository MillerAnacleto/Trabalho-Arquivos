#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

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
int dataBinaryWrite(FILE* binary_file, Bin_Data_t* data, int* size_array);

/**
 * @brief Receives a registry and prints its value following the given rules
 *
 * @param data data that will be printed
 */
void dataPrintCsvStyle(Bin_Data_t* data);

/**
 * @brief sorts an index array based on key and offset
 * 
 * @param index_array array to be sorted
 * @param size size of the array
 * @param parameter defines if the comparions will be between int or string
 */
void dataIndexArraySort(Index_Node_t** index_array, int size, int parameter);

/**
 * @brief writes an index array in a binary index file
 * 
 * @param index_file index file
 * @param index_array array to be written (index)
 * @param size size of the array
 * @param parameter defines if int or string will be written
 * @return the number of itens successfully written
 */
int dataIndexArrayWrite(FILE* index_file, Index_Node_t** index_array, int size, int parameter);

/**
 * @brief reads an index integer index array from a file 
 * 
 * @param index index file to be read
 * @param array array to recieve the index information
 * @param size number of structs to be read in the index file
 * @param node_num number of different keys read 
 * @param diff_node_num number of valid keys read (non-empty)
 */
void dataIndexArrayIntRead(FILE* index, Index_Node_t** array, int size, int* node_num, int* diff_node_num);

/**
 * @brief reads an index string index array from a file 
 * 
 * @param index index file to be read
 * @param array array to recieve the index information
 * @param size number of structs to be read in the index file
 * @param node_num number of different keys read 
 * @param diff_node_num number of valid keys read (non-empty)
 * 
*/
void dataIndexArrayStrRead(FILE* index, Index_Node_t** array, int size, int* node_num, int* diff_node_num);

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

/**
 * @brief search an index int array using binary search
 * 
 * @param index index array 
 * @param beg beggining of current binary search
 * @param end end of current binary search
 * @param field_val value being searched
 * @return position of the searched item if found, -1 otherwise 
 */
int binarySearchIndexInt(Index_Node_t** index, int beg, int end, int field_val);

/**
 * @brief search an string int array using binary search
 * 
 * @param index index array 
 * @param beg beggining of current binary search
 * @param end end of current binary search
 * @param field_val value being searched
 * @return position of the searched item if found, -1 otherwise 
 */
int binarySearchIndexStr(Index_Node_t** index, int beg, int end, char* str);

/**
 * @brief compares a list of index nodes to check if the other fields match all the 
 * search parameters (the list contains all keys collide with the indexation)
 * 
 * @param node first node in the list
 * @param array parameter array, contains all the search parameter fields
 * @param offset_array offset array that will be filled and returned, with all
 * the matching binary structs locations.
 * @param binary_file file being read to compare all the other parameters not 
 * contained in the index file
 * @param parameter_num number of parameters being compared
 * @param print case 1 prints all matching structs
 * @return returns the offset array
 */
int nodeListCompare(Index_Node_t* node, Index_Data_t** array,
    FILE* binary_file, int parameter_num, void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data));

/**
 * @brief performs a binary search in the index array and returns the offset array
 * containing the position of all matching structs in the binary file
 * 
 * @param index_file 
 * @param binary_file file being read (to compare all other search parameters 
 * that are not in the index file) 
 * @param array parameter array to hold an int or string
 * @param parameter_num number of search parameters
 * @param parameter_index binary parameter index in the parameter array
 * @return offset array
 */
int binarySearchIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index, void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data));

/**
 * @brief performs linear search in a binary file
 * 
 * @param file file in which the search occurs
 * @param array parameter array
 * @param array_size number of parameters
 * @param print if 1 prints any matching structs 
 * @return offset array
 */
int linearSearchBinaryFile(FILE* file, Index_Data_t** array, int array_size,
    void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data));


void ptrBinDataPrint(FILE* bin_file, int64_t offset, Bin_Data_t* bin_data);

void ptrBinDataDelete(FILE* bin_file, int64_t offset, Bin_Data_t* bin_data);
#endif // !DATA_H_