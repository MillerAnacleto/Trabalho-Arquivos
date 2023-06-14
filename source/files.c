#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>

int cmpfunc(const void* a, const void* b){
    int* aa = (int*) a;
    int* bb = (int*) b;

    return (*aa) - (*bb);
}

void fileBinaryPrintAsCsv(char* file_name) {

    FILE* data_file = binaryFileOpenRead(file_name);
    Data_Header* header = dataHeaderRead(data_file);
    if (headerGetStatus(header) == '0') {
        fileClose(data_file);
        errorFile();
    }

    Data_Register* data;
    int num_structs = headerGetStructNum(header);
    if (num_structs != 0) {
        for (int i = 0; i < num_structs; i++) {
            data = dataBinaryRead(data_file);
            if (dataGetRemoved(data) != '1') {
                dataPrintCsvStyle(data);
                printf("\n");
            }
            dataDestroy(data);
        }
    } else {
        free(header);
        fileClose(data_file);
        errorReg();
    }

    free(header);

    fileClose(data_file);
}

void fileReadCsvWriteBinary(char* csv_file_name, char* data_file_name) {
    Data_Register* data;

    int size_array[2];   // array para armazenar o tamanho das strings variáveis
    int accumulator = 0; // acumulador para calcular o byte offset
    int binary_write_checker;

    FILE* csv_file = csvFileOpen(csv_file_name);
    FILE* data_file = binaryFileOpenWrite(data_file_name);

    Data_Header* header = headerCreate(); // Cria o cabeçalho do arquivo

    headerSetStatus(header, '0');

    binary_write_checker = dataHeaderWrite(data_file, header, -1);
    if (binary_write_checker != 4) {
        fileClose(csv_file);
        fileClose(data_file);
        errorFile();
    }

    int i = 0;
    while (!feof(csv_file)) {
        data = dataCsvRead(csv_file, size_array);
        if (data == NULL) { // se a tentativa de leitura falhou
            // deve ser o fim do arquivo
            if (feof(csv_file)) {
                break;
            } else {
                fileClose(csv_file);
                fileClose(data_file);
                errorFile(); // caso não seja o fim do arquvio, algum erro
                // deve ter ocorrido
            }
        }

        binary_write_checker = dataBinaryWrite(data_file, data, size_array, -1);
        int write_checker = 4 + DATE_SIZE + BRAND_SIZE + size_array[0] + size_array[1];
        if (write_checker != binary_write_checker) {
            fileClose(csv_file);
            fileClose(data_file);
            errorFile();
        }

        accumulator += size_array[0] + size_array[1];
        dataDestroy(data);
        i++;
    }

    

    fileClose(csv_file);
    headerSetStructNum(header, i);
    int64_t offset = BIN_HEADER_SIZE + accumulator; // offset causado pelo header + strings variáveis
    offset = ((long long int)(i) * DATA_BASE_SIZE) + offset; // a soma da parte fixa do dado é 32 bytes
    
    headerSetOffset(header, offset);
    headerSetStatus(header, '1');

    dataHeaderWrite(data_file, header, 0);
    free(header);

    fileClose(data_file);
    binarioNaTela(data_file_name);
}

void fileIndexCreate(char* data_file_name, char* index_file_name, int64_t *offset){

    FILE* read_file = binaryFileOpenRead(data_file_name);
    Data_Header* data_header = dataHeaderRead(read_file);
    
    //reescrevemos o data_header do arq de dados com status '0'
    headerSetStatus(data_header, '0');
    dataHeaderWrite(read_file, data_header, 0);
    
    int size = headerGetStructNum(data_header);
    *offset += BIN_HEADER_SIZE; //offset provocado pelo data_header do arq de dados
    
    //modificar: criar arvore B*
    //Index_Node** index_array = indexArrayCreate(size);
    
    //modificar: BT_Index_Key


    FILE* index_file = binaryFileOpenReadWriteC(index_file_name);
    indexBTreeCreate(index_file);
    //escrever data_header com status '0'
    
    fseek(read_file, *offset, SEEK_SET);
    for(int i = 0; i < size; i++){
        int64_t offsetemp = *offset;
        char exists = 0;
        Data_Register* data = dataBinaryRead(read_file);

        indexKeyInsert(index_file, dataGetId(data), offsetemp);

        *offset += dataGetSize(data);
    }

    //reesecrevemos o data_header do arq de dados com status '1'
    headerSetStatus(data_header, '1');
    dataHeaderWrite(read_file, data_header, 0);
    fileClose(read_file);

    //modificação: o arquivo da BTree já está aberto, a inserção ocorre no arquivo
    
    //escrevemos o data_header do arq de indice com status '1' e num de structs setado
    // indexHeaderSetStatus(index_header, '1');
    // indexHeaderSetNum(index_header, non_empty);
    // indexHeaderWrite(index_file, index_header, 0);
    
    // free(index_header);
    free(data_header);

    //rescrever header do índice com status '1'
    fileClose(index_file);

    
    binarioNaTela(index_file_name);
}

int SearchBinaryFile(char* filename, char* index_file_name, int index_parameter, 
    Parameter_Hold** (*fnt)(fntptr)){

    int found = 0;
    FILE* data_file = binaryFileOpenRead(filename);
    FILE* index_file = binaryFileOpenRead(index_file_name);
    int parameter = 0;
    char index_flag = 0;
    int parameter_index = 0;
    int parameter_num = 0;

    scanf("%d", &parameter_num);
    
    Parameter_Hold** array = parameterArrayRead(parameter_num, index_parameter, &index_flag, &parameter_index);
    
    if(index_flag){
       found = searchBT(index_file, data_file, array, parameter_num);
    }
    else{
        found = linearSearchBinaryFile(data_file, array, parameter_num, fnt);
    }
    
    parameterArrayDestroy(array, parameter_num);
    
    fileClose(data_file);
    fileClose(index_file);

    return found;
}

void insertIntoBinaryFile(char* filename, char* index_file_name, int index_parameter){

    // abre arquivos
    FILE* data_file = binaryFileOpenReadWrite(filename);
    FILE* index_file = binaryFileOpenReadWrite(index_file_name);

    int size_array[2];
    Data_Register* data;
    Data_Header* header;
    Index_Header* index_header;
    int64_t offset;
    int index_str_num;
    
    int index_pos = 0;

    // lê o cabeçalho do arquivo de dados
    header = dataHeaderRead(data_file);
    headerSetStatus(header, '0');
    dataHeaderWrite(data_file, header, 0);

    // lê o cabeçalho do arquivo de indice
    index_header = indexHeaderRead(index_file);
    indexHeaderSetStatus(index_header, '0');
    indexHeaderWrite(index_file, index_header, 0);

    // le novo data
    data = dataRead(size_array);

    offset = headerGetOffset(header);
    index_str_num = indexHeaderGetNumKeys(index_header);

    // escreve novo data
    dataBinaryWrite(data_file, data, size_array, offset);


    // atualiza bin header
    headerSetStructNum(header, headerGetStructNum(header) + 1);
    headerSetOffset(header, headerGetOffset(header) + dataGetSize(data));
    headerSetStatus(header, '1');
    dataHeaderWrite(data_file, header, 0);


    //modificar: chamar a função para inserir a chave de BTree 

    // // cria o data do novo nó
    // Index_Data* new_node_data = indexDataCreate();
    // indexDataSetOffset(new_node_data, offset);

    // // array da leituro do indice 
    // Index_Node** node_array = indexArrayCreate(index_str_num);
    // int unique_node_num;
    // char index_checker = 1;

    // // lê o indice e o coloca no array de nodes
    // dataIndexArrayIntRead(index_file, node_array, index_str_num, &unique_node_num);

    // // verifica se o valor lido do parametro do indíce não é nulo
    // int field_val = dataGetIntField(data, index_parameter);
    // if(field_val == -1){
    //     index_checker = 0;
    // }
    // indexDataSetIntKey(new_node_data, field_val);

    // // busca se esse valor já existe no indice
    // index_pos = binarySearchIndexInt(node_array, 0, unique_node_num - 1, field_val);

    
    
    // // cria um novo nó de indice a partir dos dados recebidos
    // Index_Node* new_node = indexNodeCreate(new_node_data);

    // // atualiza e escreve o index header
    // indexHeaderSetNum(index_header, indexHeaderGetNum(index_header) + 1);
    // indexHeaderSetStatus(index_header, '1');
    // indexHeaderWrite(index_file, index_header, 0);

    // // se o valor ainda não existe no indice criamos ele
    // if(index_pos == -1){
    //     // se há espaço no vetor apenas colocamos ele lá
    //     if(unique_node_num < index_str_num){
    //         node_array[unique_node_num] = new_node;
    //         unique_node_num++;
    //     }
    //     // senão realocamos o vetor aumentando seu espaço
    //     else{
    //         node_array = realloc(node_array, (++index_str_num) * sizeof(Index_Node *));
    //         if(node_array == NULL){
    //             indexArrayDestroy(node_array, index_str_num, unique_node_num);
    //             dataDestroy(data);
    //             free(header);
    //             free(index_header);
    //             fileClose(data_file);
    //             fileClose(index_file);
    //         }
    //         unique_node_num++;
    //         node_array[index_str_num - 1] = new_node;
    //     }

    // }
    // // se valor existe no indice colocamos ele no fim da lista ligada 
    // else{
    //     Index_Node* node = node_array[index_pos];

    //     while(indexNodeGetNext(node) != NULL){
    //         node = indexNodeGetNext(node);
    //     }
        
    //     indexNodeStackData(node, new_node);
    // }

    // // por fim ordenamos e escrevemos o indice
    // dataIndexArraySort(node_array, unique_node_num, index_parameter);
    // dataIndexArrayWrite(index_file, node_array, unique_node_num, index_parameter);
    

    // indexArrayDestroy(node_array, index_str_num, unique_node_num);
    
    dataDestroy(data);
    free(index_header);
    free(header);
    fileClose(data_file);
    fileClose(index_file);
}