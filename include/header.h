#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>

#define BIN_HEADER_SIZE 17
#define INDEX_HEADER_SIZE 5

/**
 * @brief escreve o cabeçalho em um arquivo de dados 
 *
 * @param data_file arquivo de dados 
 * @param header cabeçalho
 * @return número de itens escritos com sucesso
 */
int dataHeaderWrite(FILE* data_file, Data_Header* header, int64_t offset);

/**
 * @brief lê o cabeçalho de um arquivo de dados
 * @param data_file file that will be read
 * @return returns the filled header struct
 */
Data_Header* dataHeaderRead(FILE* data_file);

/**
 * @brief lê o cabeçalho do índice
 * 
 * @param index indice sendo lido
 * @return struct de cabeçalho de índice preenchida 
 */
Index_Header* indexHeaderRead(FILE* index);

/**
 * @brief escreve o índice de cabeçalho
 * 
 * @param data_file arquivo de índice
 * @param header struct de cabeçalho 
 * @return numero de itens escreitos com sucesso
 */
int indexHeaderWrite(FILE* index_file, Index_Header* header, int64_t offset);

/**
 * @brief cria uma struct de cabeçalho vazia 
 * 
 * @return a struct de cabeçalho vazia
 */
Index_Header* emptyIndexHeaderCreate();


#endif // !HEADER_H_