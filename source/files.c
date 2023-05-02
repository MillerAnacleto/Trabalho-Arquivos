#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

void fileBinaryPrintAsCsv(char* file_name) {

    FILE* binary_file = binaryFileOpenRead(file_name);
    Bin_Header_t* header = binHeaderRead(binary_file);
    if (headerGetStatus(header) == '0') {
        fileClose(binary_file);
        errorFile();
    }

    Bin_Data_t* data;
    int num_structs = headerGetStructNum(header);
    if (num_structs != 0) {
        for (int i = 0; i < num_structs; i++) {
            data = dataBinaryRead(binary_file);
            if (dataGetRemoved(data) != '1') {
                dataPrintCsvStyle(data);
                printf("\n");
            }
            dataDestroy(data);
        }
    } else {
        free(header);
        fileClose(binary_file);
        errorReg();
    }

    free(header);

    fileClose(binary_file);
}

void fileReadCsvWriteBinary(char* csv_file_name, char* binary_file_name) {
    Bin_Data_t* data;

    int size_array[2];   // array to store the size of variable strings
    int accumulator = 0; // accumulator to calculate the offset byte
    int binary_write_checker;

    FILE* csv_file = csvFileOpen(csv_file_name);
    FILE* binary_file = binaryFileOpenWrite(binary_file_name);

    Bin_Header_t* header = headerCreate(); // Create the header file after opening, in case the
    // file doesn't exist, the header won't be created

    headerSetStatus(header, '0');

    binary_write_checker = binHeaderBinaryWrite(binary_file, header);
    if (binary_write_checker != 4) {
        fileClose(csv_file);
        fileClose(binary_file);
        errorFile();
    }

    int i = 0;
    while (!feof(csv_file)) {
        data = dataCsvRead(csv_file, size_array);
        if (data == NULL) { // if the attempt of reading a file is failed it
            // must be the end of file
            if (feof(csv_file)) {
                break;
            } else {
                fileClose(csv_file);
                fileClose(binary_file);
                errorFile(); // in case its not the end of file some error must
                // have ocurred
            }
        }

        binary_write_checker = dataBinaryWrite(binary_file, data, size_array);
        int write_checker = 4 + DATE_SIZE + BRAND_SIZE + size_array[0] + size_array[1];
        if (write_checker != binary_write_checker) {
            fileClose(csv_file);
            fileClose(binary_file);
            errorFile();
        }

        accumulator += size_array[0] + size_array[1];
        dataDestroy(data);
        i++;
    }

    fileClose(csv_file);
    headerSetStructNum(header, i);
    int64_t offset = 17 + accumulator; // offset caused by the header + the variable strings
    offset = ((long long int)(i) * (32)) + offset; // the struct fields summation is 32 bytes
    headerSetOffset(header, offset);
    
    headerSetStatus(header, '1');
    fseek(binary_file, 0, SEEK_SET); // reseting the pointer to the begginig in
    // order to rewrite the header

    binHeaderBinaryWrite(binary_file, header);
    free(header);

    fileClose(binary_file);
    binarioNaTela(binary_file_name);
}

int indexArrayPrint(Index_Node_t** index_array, int size){

    int acc = 0;
    Index_Data_t* data;

    for(int i = 0; i < size; i++){

        Index_Node_t* next = index_array[i];

        while(next != NULL){
            
            printf("i = %d\n", i);
            data = indexNodeGetData(next);
            int64_t offset = indexDataGetOffset(data);
            int int_key =  indexDataGetIntKey(data);
            char* char_key = indexDataGetStrKey(data);

            if(int_key != EMPTY_INT_FIELD){
                printf("%d ", int_key);
                //acc += fwrite(&int_key, sizeof(int), 1, index_file);
            }
            else if(char_key != NULL){
                printf("%c%c%c%c ", char_key[0], char_key[1], char_key[2], char_key[3]);
                //acc += fwrite(char_key, sizeof(char), STR_SIZE, index_file);
            }
            printf("%ld\n", offset);
            //acc += fwrite(&offset, sizeof(int64_t), 1, index_file);

            next = indexNodeGetNext(next);
        }
    }

    return acc;
}

void fileIndexCreate(char* binary_file_name, char* index_file_name, int parameter, int64_t *offset){

    FILE* read_file = binaryFileOpenRead(binary_file_name);

    Bin_Header_t* header = binHeaderRead(read_file);
    int size = headerGetStructNum(header);
    *offset += 17; //offset provocado pelo header
    //char* str = NULL;

    Index_Header_t* index_header = indexHeaderCreate();
    indexHeaderSetStatus(index_header, '0');
    //escrever o header

    Index_Node_t** index_array = indexArrayCreate(size);
    
    int non_empty = 0;

    Index_Data_t* data;
    //colocar o que está dentro do for numa função.
    for(int i = 0; i < size; i++){
            
        int64_t offset_temp = *offset;
        char exists = 0;
        data = readBinaryField(read_file, parameter, offset, &exists);
        indexDataSetOffset(data, offset_temp);
        
        if(exists){
            indexNodeSetData(index_array, non_empty, data);
            non_empty++;
        }
        else{
            free(data);
        }
    }
    fileClose(read_file);

    dataIndexArraySort(index_array, non_empty, parameter);

    //modularizar melhor? Talvez fazer numa função de write?
    FILE* index_file = binaryFileOpenWrite(index_file_name);
    
    indexHeaderWrite(index_file, index_header);
    dataIndexArrayWrite(index_file, index_array, non_empty);

    indexArrayDestroy(index_array, size, non_empty);

    indexHeaderSetStatus(index_header, '1');
    indexHeaderSetNum(index_header, non_empty);
    fseek(index_file,0, SEEK_SET);
    indexHeaderWrite(index_file, index_header);
    
    free(index_header);
    free(header);
    
    fileClose(index_file);

    binarioNaTela(index_file_name);
}

Index_Node_t** fileIndexRead(char* index_filename, int parameter ){
    
    int node_num = 0; 
    int diff_node_num = 0;
    
    FILE* index = binaryFileOpenRead(index_filename);
    Index_Header_t* header = indexHeaderRead(index);
    int size = indexHeaderGetNum(header);

    if(header == NULL) return NULL;

    Index_Node_t** array = indexArrayCreate(size);

    if(parameter <= 1){
        dataIndexArrayIntRead(index, array, size, &node_num, &diff_node_num);
    }
    else{
        dataIndexArrayStrRead(index, array, size, &node_num, &diff_node_num);
    }   

    //indexArrayPrint(array, diff_node_num);
    indexArrayDestroy(array, node_num, diff_node_num);

    free(header);
    fileClose(index);
    return array;
}

char SearchBinaryFile(char* filename, int index_parameter){

    char found = 0;
    FILE* binary_file = binaryFileOpenRead(filename);
    int parameter_num = 0;
    int parameter = 0;
    //char binary_flag = 0;
    scanf("%d", &parameter_num);

    Index_Data_t** array = malloc(parameter_num*sizeof(Index_Data_t*));
    for(int kj = 0; kj < parameter_num; kj++){
        array[kj] = indexDataCreate();
    }

    for(int j = 0; j < parameter_num; j++){
        
        parameter = searchParameter();
        if(parameter == index_parameter){
            //binary_flag = 1;
        }
        readFieldStdin(array[j], parameter);
    }

    
    // if(binary_flag){
    //     //binarySearchIndexArray(Param_array, index_file, binary_file);
    // }
    //else{
    found =  linearSearchBinaryFile(binary_file, array, parameter_num);
    //}

    // for(int m = 0; m < parameter_num; m++){
        
    //     if(indexDataGetIntKey(array[m]) == EMPTY_INT_FIELD){
    //         char* str = indexDataGetStrKey(array[m]);
    //         if(str != NULL){
    //             for(int k = 0; k < 12; k++){
    //                 printf("%c", str[k]);
    //             }
    //             printf("\n");
    //         }
    //     }
    //     else printf("%d\n", indexDataGetIntKey(array[m]));
    //     printf("param = %ld\n", indexDataGetOffset(array[m]));
    // }
    // printf("\n\n");

    

    for(int kj = 0; kj < parameter_num; kj++){
        indexDataDestroy(array[kj]);
    }
    free(array);

    fileClose(binary_file);

    return found;
}

char linearSearchBinaryFile(FILE* file, Index_Data_t** array, int array_size){
    char found = 0;
    Bin_Header_t* header;
    header = binHeaderRead(file);
    int struct_num = headerGetStructNum(header);
    //char exists;

    //int param = (int)indexDataGetOffset(array[0]);

    for(int i = 0; i < struct_num; i++){
        Bin_Data_t* bin_data = dataBinaryRead(file);
        if(dataGetRemoved(bin_data) == '1'){
            dataDestroy(bin_data);
            continue;
        }

        //if(cmp == array[i]) printDataCsvStyle(data);

        //comparar todos os campos do array com data (Bin_data)
        //para isso, criar uma função que retorna o campo dado o parâmetro

        //fazer uma função que compara:
        //bin_data e array (param está no offset)
        //cada iteração deve comparar um campo que está no array[i]
        //e algum dos campos do bin_data, para isso fazer uma função
        //que retorna os campos de acordo com o valor do parâmetro fornecido.
        //ou seja, a função allParamCompare deve ser algo como:

        if(dataParamCompare(bin_data, array, array_size)){
            found = 1;
            dataPrintCsvStyle(bin_data);
            printf("\n");
        }

        dataDestroy(bin_data);
        // for(int i = 0; i < arr_size; i++){
        //      compare(bin_data, array[i]);
        // onde compare bin_data, array[i] => 
        //          int param = array[i]->offset;
        //          switch (param):
        //          case 0: 
        //              if(bin_data->id == array[i]->int_key) ...
        //              return 1; ou return 0;
        //          case n:
        //              str1 = bin_data->lugar;
        //              str2 = array[i]->str_key; 
        //              break;
        //          compare(str1, str2) return 1 if equal, 0 if diff.
          
        
    }

    free(header);

    return found;
}