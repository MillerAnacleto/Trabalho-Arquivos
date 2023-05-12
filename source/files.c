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

    binary_write_checker = binHeaderBinaryWrite(binary_file, header, -1);
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

    binHeaderBinaryWrite(binary_file, header, 0);
    free(header);

    fileClose(binary_file);
    binarioNaTela(binary_file_name);
}

void fileIndexCreate(char* binary_file_name, char* index_file_name, int parameter, int64_t *offset){

    FILE* read_file = binaryFileOpenRead(binary_file_name);

    Bin_Header_t* header = binHeaderRead(read_file);
    
    //reescrevemos o header do arq de dados com status '0'
    headerSetStatus(header, '0');
    binHeaderBinaryWrite(read_file, header, 0);
    
    int size = headerGetStructNum(header);
    *offset += 17; //offset provocado pelo header do arq de dados
    
    Index_Node_t** index_array = indexArrayCreate(size);
    
    int non_empty = 0;

    Index_Data_t* data;
    //colocar o que está dentro do for numa função.
    for(int i = 0; i < size; i++){
            
        int64_t offset_temp = *offset;
        char exists = 0;
        data = readBinaryField(read_file, parameter, offset, &exists);
        
        if(exists){
            indexDataSetOffset(data, offset_temp);
            indexNodeSetData(index_array, non_empty, data);
            non_empty++;
        }
        else{
            free(data);
        }
    }

    //reesecrevemos o header do arq de dados com status '1'
    headerSetStatus(header, '1');
    binHeaderBinaryWrite(read_file, header, 0);
    fileClose(read_file);

    dataIndexArraySort(index_array, non_empty, parameter);

    FILE* index_file = binaryFileOpenWrite(index_file_name);

    //escrevemos o header do arq de indice com status '0'
    Index_Header_t* index_header = emptyIndexHeaderCreate();
    indexHeaderWrite(index_file, index_header, -1);
    
    dataIndexArrayWrite(index_file, index_array, non_empty, parameter);
    indexArrayDestroy(index_array, size, non_empty);

    //escrevemos o header do arq de indice com status '1' e num de structs setado.'
    indexHeaderSetStatus(index_header, '1');
    indexHeaderSetNum(index_header, non_empty);
    indexHeaderWrite(index_file, index_header, 0);
    
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

    indexArrayDestroy(array, node_num, diff_node_num);

    free(header);
    fileClose(index);
    return array;
}

int SearchBinaryFile(char* filename, char* index_file_name, int index_parameter, 
    void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data)){

    int found = 0;
    FILE* binary_file = binaryFileOpenRead(filename);
    FILE* index_file = binaryFileOpenRead(index_file_name);
    int parameter_num = 0;
    int parameter = 0;
    char binary_flag = 0;
    int parameter_index = 0;

    scanf("%d", &parameter_num);

    Index_Data_t** array = malloc(parameter_num*sizeof(Index_Data_t*));
    for(int kj = 0; kj < parameter_num; kj++){
        array[kj] = indexDataCreate();
    }

    for(int j = 0; j < parameter_num; j++){
        
        parameter = searchParameter();
        if(parameter == index_parameter){
            binary_flag = 1;
            parameter_index = j;
        }
        readFieldStdin(array[j], parameter);
    }
    
    if(binary_flag){
        found = binarySearchIndexArray(index_file, binary_file, array, parameter_num, parameter_index, fnt);
    }
    else{
        found = linearSearchBinaryFile(binary_file, array, parameter_num, fnt);
    }
    
    for(int kj = 0; kj < parameter_num; kj++){
        indexDataDestroy(array[kj]);
    }
    free(array);
    fileClose(binary_file);
    fileClose(index_file);

    return found;
}

void removedStructUpdate(char* read_file_name,int found){
    //mudar nome da função!
    FILE* data_file = binaryFileOpenRead(read_file_name);
    Bin_Header_t* bin_header = binHeaderRead(data_file);
    found += headerGetRemStructNum(bin_header);
    headerSetRemStructNum(bin_header, found);
    binHeaderBinaryWrite(data_file, bin_header, 0);

    free(bin_header);
    fileClose(data_file);

}