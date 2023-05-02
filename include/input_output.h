#ifndef INPUT_OUTPUT_H_
#define INPUT_OUTPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

/**
 * @brief reads a fixed size string from a file
 * (the return type is void because the fixed string
 * is already allocated, so we don't need to malloc this
 * string in the function)
 *
 * @param csv_file file in which the string is being read
 * @param str string being filled
 * @param strlen length of the string being read
 */
void readCsvConstString(FILE* csv_file, char* str, int strlen);

/**
 * @brief reads a variable sized string from a file
 *
 * @param csv_file file in which the string is being read
 * @param strlen size of the string
 * @return char* string that was read
 */
char* readCsvVarString(FILE* csv_file, int* strlen);

/**
 * @brief reads an integer from the CSV file, it the field is empty
 * then the integer is default to -1
 *
 * @param csv_file file being read
 * @return int integer that was read
 */
int readCsvInt(FILE* csv_file);

/**
 * @brief prints a fixed-size string
 *
 * @param str string that will be printed
 * @param strlen length of the string
 */
void printConstString(char* str, int strlen);

/**
 * @brief reads a string that has variable length, then identifies its end and changes the delimiter for a '\0'
 *
 * @param str
 * @return char* returns the modified string
 */
char* readBinaryVarString(FILE* bin_file);

char* readBinaryField(FILE* file, int parameter, int64_t *offset, int* key, char* exists);

void printField(char* str, int64_t offset, int key, int parameter);

void readFieldStdin(Index_Data_t* array_elem, int parameter);

#endif // !INPUT_OUTPUT_H_