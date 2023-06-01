#ifndef INDEX_H_
#define INDEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <data.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>


/**
 * @brief ordena um vetor de ponteiros para registros de índice
 * 
 * @param index_array vetor a ser ordenado
 * @param size tamaanho do vetor
 * @param parameter parametro que determina se a comparação ocorre entre 
 * strings ou ints
 */
void dataIndexArraySort(Index_Node** index_array, int size, int parameter);

/**
 * @brief escreve um vetor de registros de índice num arquivo binário 
 * 
 * @param index_file arquivo a ser preenchido
 * @param index_array vetor de registros
 * @param size tamanho do vetor
 * @param parameter paramtero indexado para diferenciar string de int
 * @return número de elementos escritos com sucesso
 */
int dataIndexArrayWrite(FILE* index_file, Index_Node** index_array, int size, int parameter);

/**
 * @brief lê um vetor de índice de um arquivo para memória primária indexado com string 
 * 
 * @param index arquivo de índice sendo lido
 * @param array vetor recebendo os registros de indice
 * @param size número de registros a serem lidos 
 * @param diff_node_num número de chaves diferentes lidas (tamanho horizontal do vetor)
 */
void dataIndexArrayIntRead(FILE* index, Index_Node** array, int size, int* diff_node_num);

/**
 * @brief lê um arquivo de índice de um arquivo para memória primária indexado com string 
 * 
 * @param index arquivo de índice sendo lido
 * @param array vetor de arquivos recebendo a string
 * @param size número de registros a serem lidos 
 * @param diff_node_num número de chaves diferentes lidas (tamanho horizontal do vetor)
 * 
*/
void dataIndexArrayStrRead(FILE* index, Index_Node** array, int size, int* diff_node_num);

#endif