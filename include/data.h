#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>

#define fntptr FILE* file, int64_t offset, Data_Register* bin_data, Data_Header* bin_header , Parameter_Hold** param_array, int* param_num

#define DATA_BASE_SIZE 32
typedef char bool;

/**
 * @brief lê um registro de dados de um arquivo csv
 *
 * @param csv_file arquivo sendo lido
 * @param size_array array com os tamanhos das strings variáveis (descrição e lugar)
 * para calcular o byte offset 
 *
 * @return a struct Bin_Data preenchida 
 */
Data_Register* dataCsvRead(FILE* csv_file, int* size_array);

/**
 * @brief lê um registro de dados da entrada padrão
 * 
 * @param size_array 
 * @return a struct Bin_Data preenchida 
 */
Data_Register* dataRead(int* size_array);

/**
 * @brief lê um registro de dados de um arquivo binário
 *
 * @param data_file o arquivo cuja função é lida
 * @return a struct Bin_Data preenchida 
 */
Data_Register* dataBinaryRead(FILE* data_file);

/**
 * @brief escreve o registro de dados em um arquivo
 *
 * @param data_file arquivo em modo de escrita binária
 * @param data struct que tem os arquivos lidos
 * @param size_array vetor que mantém o tamanho das strings variáveis
 *
 * @return número de itens escritos corretamente
 */
int dataBinaryWrite(FILE* data_file, Data_Register* data, int* size_array, int64_t offset);

/**
 * @brief imprime um registro de dados no formato csv
 *
 * @param data registro a ser impresso
 */
void dataPrintCsvStyle(Data_Register* data);



/**
 * @brief compara um registro binário com um vetor de parâmetos (cada elemento
 * do vetor de parâmetros possui um campo).
 * 
 * @param bin_data registro binário a ser comparado
 * @param parameter_array vetor de parâmetros que foram passados da entrada
 * @param parameter_num numero de parâmetros (tamanho do vetor)
 * @return TRUE caso o arquivo binário possua todos os campos, FALSE c.c. 
 */
bool dataParamCompare(Data_Register* bin_data, Parameter_Hold** parameter_array, int parameter_num);

/**
 * @brief retorna o campo inteiro de um registro de dados
 * 
 * @param data registro de dados a ter o campo retornado
 * @param param especificador que determina qual parâmetro é retornado
 * @return id (param == 0) ou número do artigo (param == 1)
 */
int dataGetIntField(Data_Register* data, int param);

/**
 * @brief retorna uma string de um registro de dados
 * 
 * @param data registro de dados a ter o campo retornado
 * @param param  specificador que determina qual parâmetro é retornado
 * @return  data caso param == 2, descrição caso param == 3, 
 * lugar caso param == 4, marca caso param == 5.
 */
char* dataGetStrField(Data_Register* data, int param);

/**
 * @brief seta um campo inteiro de um registro de dados 
 * 
 * @param data registro de dados a ter o campo setado
 * @param field valor do campo
 * @param param especificador que determina o campo (mesmo das funções acima)
 */
void dataSetIntField(Data_Register* data, int field, int param);

/**
 * @brief seta um campo de string de um registro de dados 
 * 
 * @param data registro de dados a ter o campo setado
 * @param str valor do campo
 * @param param especificador que determina o campo (mesmo das funções acima)
 */
void dataSetStrField(Data_Register* data, char* str, int param);


/**
 * @brief busca em um vetor de índices por um campo inteiro 
 * 
 * @param index vetor de índices indexado num campo inteiro 
 * @param beg início da busca binária atual
 * @param end fim da busca binária atual
 * @param field_val valor sendo buscado
 * @return posição do valor buscado, ou -1 caso não exista
 */
int binarySearchIndexInt(Index_Node** index, int beg, int end, int field_val);

/**
 * @brief busca em um vetor de índices por um campo de string 
 * 
 * @param index vetor de índices indexado num campo de string
 * @param beg início da busca binária atual
 * @param end fim da busca binária atual
 * @param str valor sendo buscado
 * 
 * @return posição do valor buscado, ou -1 caso não exista
*/
int binarySearchIndexStr(Index_Node** index, int beg, int end, char* str);

/**
 * @brief compara uma lista ligada de nós com um array de parametros
 * 
 * @param node primeiro nó na lista
 * @param array array de parâmetros que contem os offsets a serem buscados
 * @param data_file arquivo de dados sendo lido, para que a comparação ocorra por completo
 * é necessário ir até a posição do offset e recuperar o registro de dados inteiro
 * @param parameter_num número de parâmetros sendo comparado
 * @param fnt ponteiro para a função que executa quando um registro é encontrado
 * @return numero de itens encontrados
 */
int nodeListCompare(Index_Node* node, Parameter_Hold** array,
    FILE* data_file, int parameter_num, Parameter_Hold** (*fnt)(fntptr));

/**
 * @brief busca binária em um array de índices, encapsula as funções que lidam com 
 * int e string
 * 
 * @param index_file arquivo de índice sendo lido para gerar o array de índice a ser
 * buscado
 * @param data_file arquivo de dados a ser lido para a comparação completa entre os parâmetros 
 * @param array array de parâmetros
 * @param parameter_num número de parâmetros de busca
 * @param parameter_index indice do parâmetro que contém o campo indexado no arquivo de índice
 * @return número de registros encontrados com sucesso
 */
int binarySearchIndexArray(FILE* index_file, FILE* data_file, Parameter_Hold** array,  
    int parameter_num, int parameter_index, Parameter_Hold** (*fnt)(fntptr));

/**
 * @brief faz busca linear num arquivo de dados binário
 * 
 * @param file arquivo de dados
 * @param array vetor de parâmetros passados
 * @param array_size número de parâmetros (tamanho do vetor)
 * @param fnt ponteiro de função para o que deve ser executado quando a busca tem sucessp
 * @return numero de itens encontrados
 */
int linearSearchBinaryFile(FILE* file, Parameter_Hold** array, int array_size,
    Parameter_Hold** (*fnt) (fntptr));

/**
 * @brief função para imprimir na tela o registro de dados 
 * 
 * @return o retorno não é usado nessa função (apenas mantido para que as funções tenham mesma sintaxe)
 */
Parameter_Hold** ptrBinDataPrint(fntptr);

/**
 * @brief função para deletar um registro de dados
 * 
 * @return o retorno não é usado nessa função (apenas mantido para que as funções tenham mesma sintaxe) 
 */
Parameter_Hold** ptrBinDataDelete(fntptr);

/**
 * @brief função para dar update num regisrto de dados
 * 
 * @return vetor de parâmetros para a próxima correspondência dentro da mesma busca
 */
Parameter_Hold** ptrUpdateField(fntptr);

/**
 * @brief preenche um vetor de parâmetros com dados da entrada padrão
 * 
 * @param parameter_num número de parâmetros a ser passado (tamanho do vetor)
 * @param index_parameter valor indexado
 * @param binary_flag caso o valor indexado tenha o mesmo tipo que o passado como parâmetro
 * a flag de busca binária de torna 1
 * @param parameter_index indice do vetor de parâmetros que contém o valor indexado
 * @return array de parâmetros preenchido
 */
Parameter_Hold** parameterArrayRead(int parameter_num, int index_parameter, char* binary_flag,
    int* parameter_index);

#endif // !DATA_H_