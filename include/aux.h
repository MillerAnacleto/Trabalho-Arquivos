#ifndef AUX_H_
#define AUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

/**
 * @brief displays file error message and exits
 * with code 1
 *
 */
void errorFile();

/**
 * @brief displays register error message and exits
 * 
 */
void errorReg();

/**
 * @brief opens a file in text read mode
 * and treats the exception in case the file
 * doesn't exists
 *
 * @param file_name file name
 * @return FILE* returns the archieve pointer
 */
FILE* csvFileOpen(char* file_name);

/**
 * @brief opens a file in binary write mode
 * or creates a file in case it doesn't exist
 *
 * @param file_name file to be opened
 * @return FILE* returns the file to the pointer
 */
FILE* binaryFileOpenWrite(char* file_name);

/**
 * @brief opens a binary file in read mode
 *
 * @param file_name name of the file that will be read
 * @return the address to the file
 */
FILE* binaryFileOpenRead(char* file_name);

/**
 * @brief asserts if the file has ended in case return_value is 0
 *
 * @param return_value value returned from the functions that read files
 * (fread, fscanf, ...). In case this value is 0, either the file has
 * ended (in which case the return value is 1), or there was a reading error,
 * then the errorFile() function is invoked, displaying the error message.
 * Otherwise it returns 0
 *
 * @param file file being read
 * @return char returns 1 if the file has ended, 0 otherwise.
 */
char endFileAssert(int return_value, FILE* file);

/**
 * @brief verifies the end of file after a csv line has been read,
 * consuming '\n' and '\r'
 *
 * @param file file being verified
 * @return char if the file has ended returns 1, otherwise 0
 */
char endFileChecker(FILE* file);

/**
 * @brief consumes the first line of the csv file
 * 
 */
void consumeFirstLine(FILE* csv_file);

// closes a file
void fileClose(FILE* file);

/**
 * @brief verifies if the strings that have constant length are NULL or not
 *
 * @param str string that will be verified
 * @return 1 if it is NULL 0 if it is not
 */
int isConstStringNull(char* str);

int varStrTell(char* str);

int varStrSize(Data_t* data);

int searchParameter();

char* copyConstVarStr(char* str1);

// given function to produce an output based on the header that was written
void binarioNaTela(char* nomeArquivoBinario);

char* readQuote12();

#endif // !AUX_H_