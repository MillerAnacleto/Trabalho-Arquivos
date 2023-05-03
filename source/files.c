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

    indexArrayDestroy(array, node_num, diff_node_num);

    free(header);
    fileClose(index);
    return array;
}

char SearchBinaryFile(char* filename, char* index_file_name, int index_parameter){

    char found = 0;
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
        found = binarySearchIndexArray(index_file, binary_file, array, parameter_num, parameter_index);
    }
    else{
        found = linearSearchBinaryFile(binary_file, array, parameter_num);
    }    

    for(int kj = 0; kj < parameter_num; kj++){
        indexDataDestroy(array[kj]);
    }
    free(array);
    fileClose(binary_file);
    fileClose(index_file);

    return found;
}

char linearSearchBinaryFile(FILE* file, Index_Data_t** array, int array_size){
    char found = 0;
    Bin_Header_t* header;
    header = binHeaderRead(file);
    int struct_num = headerGetStructNum(header);

    for(int i = 0; i < struct_num; i++){
        Bin_Data_t* bin_data = dataBinaryRead(file);
        if(dataGetRemoved(bin_data) == '1'){
            dataDestroy(bin_data);
            continue;
        }

        if(dataParamCompare(bin_data, array, array_size)){
            found = 1;
            dataPrintCsvStyle(bin_data);
            printf("\n");
        }

        dataDestroy(bin_data);
    }

    free(header);

    return found;
}

int binarySearchIndexInt(Index_Node_t** index, int beg, int end, int field_val){
    
    int mid = (beg+end)/2;

    int curr_val = indexDataGetIntKey(indexNodeGetData(index[mid]));
    if(beg >= end){
        
        if(curr_val != field_val) return -1;
        return mid;
    }

    if(curr_val > field_val){
        return binarySearchIndexInt(index, beg, mid-1, field_val);
    }
    else if(curr_val < field_val){
        return binarySearchIndexInt(index, mid+1, end, field_val);
    }
    else return mid;
    
}

int binarySearchIndexStr(Index_Node_t** index, int beg, int end, char* str){
    int mid = (beg+end)/2;

    char* curr_val = indexDataGetStrKey(indexNodeGetData(index[mid]));
    if(beg >= end){
        
        if(stringnCmp(curr_val, str, STR_SIZE) != 0 ) return -1;
        return mid;
    }

    if(stringnCmp(curr_val, str, STR_SIZE) > 0){
        return binarySearchIndexStr(index, beg, mid-1, str);
    }
    else if(stringnCmp(curr_val, str, STR_SIZE) < 0){
        return binarySearchIndexStr(index, mid+1, end, str);
    }
    else return mid;
}

char nodeListCompare(Index_Node_t* node, Index_Data_t** array, FILE* binary_file, int parameter_num){
    
    char found = 0;
    while(node != NULL){
        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(binary_file, offset, SEEK_SET);
        Bin_Data_t* bin_data = dataBinaryRead(binary_file);
        if( dataParamCompare(bin_data, array, parameter_num) ){
            dataPrintCsvStyle(bin_data);
            printf("\n");
            found = 1;
        }
        dataDestroy(bin_data);
        node = indexNodeGetNext(node);
    }
    return found;
}

char binarySearchIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index){
    
    char found = 0;
    Index_Header_t* header = indexHeaderRead(index_file);
    int size = indexHeaderGetNum(header);
    int node_num = 0;
    int diff_node_num = 0;
    int index_pos = 0;

    Index_Node_t** node_array = indexArrayCreate(size);
    int param = (int) indexDataGetOffset(array[parameter_index]);
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &node_num, &diff_node_num);
        int field_val = indexDataGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, diff_node_num, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &node_num, &diff_node_num);
        char* str = indexDataGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, diff_node_num, str);
    }

    if(index_pos == -1){
        
        indexArrayDestroy(node_array, size, diff_node_num);
        free(header);
        return 0;
    }
    Index_Node_t* n = node_array[index_pos];
    found = nodeListCompare(n, array, binary_file, parameter_num);

    indexArrayDestroy(node_array, size, diff_node_num);
    free(header);

    if(!found) return 0;

    return 1;
}
