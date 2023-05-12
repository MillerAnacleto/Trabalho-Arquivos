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


// falta checar se o arquivo está inconsistente
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

        binary_write_checker = dataBinaryWrite(binary_file, data, size_array, 0);
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
    
    int unique_node_num = 0;

    Index_Data_t* data;
    //colocar o que está dentro do for numa função.
    for(int i = 0; i < size; i++){
            
        int64_t offset_temp = *offset;
        char exists = 0;
        data = readBinaryField(read_file, parameter, offset, &exists);
        
        if(exists){
            indexNodeSetData(index_array, unique_node_num, data);
            unique_node_num++;
        }
        else{
            free(data);
        }
    }
    fileClose(read_file);

    dataIndexArraySort(index_array, unique_node_num, parameter);

    //modularizar melhor? Talvez fazer numa função de write?
    FILE* index_file = binaryFileOpenWrite(index_file_name);
    
    indexHeaderWrite(index_file, index_header);
  
    dataIndexArrayWrite(index_file, index_array, unique_node_num);

    indexArrayDestroy(index_array, size, unique_node_num);

    indexHeaderSetStatus(index_header, '1');
    indexHeaderSetNum(index_header, unique_node_num);
    fseek(index_file,0, SEEK_SET);
    indexHeaderWrite(index_file, index_header);
    
    free(index_header);
    free(header);
    
    fileClose(index_file);
}

Index_Node_t** fileIndexRead(char* index_filename, int parameter ){
    
    int node_num = 0; 
    int unique_node_num = 0;
    
    FILE* index = binaryFileOpenRead(index_filename);
    Index_Header_t* header = indexHeaderRead(index);
    int size = indexHeaderGetNum(header);

    if(header == NULL) return NULL;

    Index_Node_t** array = indexArrayCreate(size);

    if(parameter <= 1){
        dataIndexArrayIntRead(index, array, size, &unique_node_num);
    }
    else{
        dataIndexArrayStrRead(index, array, size, &unique_node_num);
    }   

    indexArrayDestroy(array, node_num, unique_node_num);

    free(header);
    fileClose(index);
    return array;
}

/*
    search: data
    delete: offset, file
    update: offset, file
*/

int SearchBinaryFile(char* filename, char* index_file_name, int index_parameter){

    int found = 0;
    FILE* binary_file = binaryFileOpenRead(filename);
    FILE* index_file = binaryFileOpenRead(index_file_name);
    int parameter_num = 0;
    int parameter = 0;
    char binary_flag = 0;
    int parameter_index = 0;
    //
    Bin_Header_t* header;
    
    { // abre o bin header
        header = binHeaderRead(binary_file);
        if (headerGetStatus(header) == '0') {
            fileClose(binary_file);
            fileClose(index_file);
            errorFile();
        }

        headerSetStatus(header, '0');
        printf("header: %c %ld %d %d\n", headerGetStatus(header), headerGetOffset(header), headerGetStructNum(header),headerGetRemStructNum(header));
    }
    
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
        found = linearSearchBinaryFile(binary_file, header, array, parameter_num, 1);
    }

    for(int kj = 0; kj < parameter_num; kj++){
        indexDataDestroy(array[kj]);
    }

    //    
    free(header);
    free(array);
    fileClose(binary_file);
    fileClose(index_file);

    return found;
}

// int SearchUpdateBinaryFile(char* filename, char* index_file_name, int index_parameter){

//     char found = 0;
//     FILE* binary_file = binaryFileOpenRead(filename);
//     FILE* index_file = binaryFileOpenRead(index_file_name);
//     int parameter_num = 0;
//     int parameter = 0;
//     int set_num = 0;
//     int set_campo = 0;
//     char binary_flag = 0;
//     int parameter_index = 0;
    
//     scanf("%d", &parameter_num);
    
//     printf("%d\n", parameter_num);

//     Index_Data_t** array = malloc(parameter_num*sizeof(Index_Data_t*));
//     for(int kj = 0; kj < parameter_num; kj++){
//         array[kj] = indexDataCreate();
//     }

//     for(int j = 0; j < parameter_num; j++){
        
//         parameter = searchParameter();
//         if(parameter == index_parameter){
//             binary_flag = 1;
//             parameter_index = j;
//         }
//         readFieldStdin(array[j], parameter);
//         if(indexDataGetParam(array[j])<=1)
//             printf("%d %d\n",indexDataGetParam(array[j]), indexDataGetIntKey(array[j]));
//         else
//             printf("%d %s\n",indexDataGetParam(array[j]), indexDataGetStrKey(array[j]));
//     }

//     scanf("%d", &set_num);
    
//     printf("%d\n", set_num);

//     Index_Data_t** array_set = malloc(set_num*sizeof(Index_Data_t*));
//     for(int kj = 0; kj < set_num; kj++){
//         array_set[kj] = indexDataCreate();
//     }

//     for(int j = 0; j < set_num; j++){
        
//         set_campo = searchParameter();
//         if(set_campo == index_parameter){
//             binary_flag = 1;
//         }
//         readFieldStdin(array_set[j], set_campo);
//         if(indexDataGetParam(array_set[j])<=1)
//             printf("%d %d\n",indexDataGetParam(array_set[j]), indexDataGetIntKey(array_set[j]));
//         else
//             printf("%d %s\n",indexDataGetParam(array_set[j]), indexDataGetStrKey(array_set[j]));
//     }
    
//     int64_t* arr = NULL;

//     if(binary_flag){
//         arr = binarySearchIndexArray(index_file, binary_file, array, parameter_num, parameter_index);
//     }
//     else{
//         arr = linearSearchBinaryFile(binary_file, array, parameter_num, 1);
//     }

//     printf("start update-----------------------\n");

//     for(int i = 0; arr != NULL && arr[i] != -1; i++){
//         //printf("\noset: %ld\n", arr[i]);
//         fseek(binary_file, arr[i], SEEK_SET);
//         Bin_Data_t* bin_data = dataBinaryRead(binary_file);
//         dataPrintCsvStyle(bin_data);
//     }

//     if(arr != NULL && arr[0] != -1){
//         found = 1;
        
//     }
//     free(arr);
     

   

//     for(int kj = 0; kj < parameter_num; kj++){
//         indexDataDestroy(array[kj]);
//     }
//     free(array);
//     fileClose(binary_file);
//     fileClose(index_file);

//     return found;
// }

// atualizar os headers
int SearchDeleteBinaryFile(char* filename, char* index_file_name, int index_parameter){

    int found = 0;
    FILE* binary_file = binaryFileOpenReadWrite(filename);
    FILE* index_file = binaryFileOpenRead(index_file_name);
    int parameter_num = 0;
    int parameter = 0;
    char binary_flag = 0;
    int parameter_index = 0;
    Bin_Header_t* header;

    { // abre o bin header
        header = binHeaderRead(binary_file);
        if (headerGetStatus(header) == '0') {
            fileClose(binary_file);
            fileClose(index_file);
            errorFile();
        }

    }

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
        found = binarySearchDeleteIndexArray(index_file, binary_file, array, parameter_num, parameter_index);
    }
    else{
        found = linearDeleteBinaryFile(binary_file, header, array, parameter_num);
    }
    
    for(int kj = 0; kj < parameter_num; kj++){
        indexDataDestroy(array[kj]);
    }


    { // atualiza e escreve o bin header
        printf("header: %c %ld %d %d\n", headerGetStatus(header), headerGetOffset(header), headerGetStructNum(header),headerGetRemStructNum(header));
    
        headerSetRemStructNum(header, headerGetRemStructNum(header) + found);

        headerSetStatus(header, '1');

        printf("header: %c %ld %d %d\n", headerGetStatus(header), headerGetOffset(header), headerGetStructNum(header),headerGetRemStructNum(header));
        binHeaderBinaryWrite(binary_file, header);
    }

    printf("deleted: %d\n", found);

    free(header);
    free(array);
    fileClose(binary_file);
    fileClose(index_file);

    return found;
}

int linearSearchBinaryFile(FILE* file, Bin_Header_t* header, Index_Data_t** array, int array_size, char print){

    int found = 0;
    
    int64_t offset = 0;
    offset += 17;
    int struct_num = headerGetStructNum(header);

    for(int i = 0; i < struct_num; i++){
        Bin_Data_t* bin_data = dataBinaryRead(file);
        int var = 32 + varStrSize(bin_data);
        offset += var;

        // if(dataGetRemoved(bin_data) == '1'){
        //     dataDestroy(bin_data);
        //     continue;
        // }

        if(dataParamCompare(bin_data, array, array_size)){
            found++;
            
            if(print){
                dataPrintCsvStyle(bin_data);
                printf("\n");
            }
        }

        dataDestroy(bin_data);
    }

    return found;
}

int linearDeleteBinaryFile(FILE* file, Bin_Header_t* header, Index_Data_t** array, int array_size){
    
    int found = 0;

    int64_t offset = 0;
    offset += 17;
    int struct_num = headerGetStructNum(header);

    for(int i = 0; i < struct_num; i++){
        Bin_Data_t* bin_data = dataBinaryRead(file);
        int var = 32+varStrSize(bin_data);
        offset += var;

        if(dataGetRemoved(bin_data) == '1'){
            dataDestroy(bin_data);
            continue;
        }
        if(dataParamCompare(bin_data, array, array_size)){
            found++;

            int64_t str_size = varStrSize(bin_data);
            int64_t data_offset =  str_size + DATA_BASE_SIZE;
            dataMarkDeleted(file, data_offset);
        }

        dataDestroy(bin_data);
    }

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

int nodeListCompare(Index_Node_t* node, Index_Data_t** array,
    FILE* binary_file, int parameter_num, int print){
    int found = 0;
    
    while(node != NULL){
        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(binary_file, offset, SEEK_SET);
        Bin_Data_t* bin_data = dataBinaryRead(binary_file);
        // printf("offset: %ld ||||| data_bin: ", offset);
        // dataPrintCsvStyle(bin_data);
        // printf("  foi retornada pela busca bin\nparameternum: %d\n", parameter_num);
        if(dataParamCompare(bin_data, array, parameter_num)){
            
            // if(dataGetRemoved(bin_data) == '1'){
            //     dataDestroy(bin_data);            
            //     node = indexNodeGetNext(node);
            //     continue;
            // }

            found++;

            if(print){
                dataPrintCsvStyle(bin_data);
                printf("\n");
            }
        }
        dataDestroy(bin_data);
        node = indexNodeGetNext(node);
    }

    return found;
}

int nodeListCompareDelete(Index_Node_t* node, Index_Data_t** array,
    FILE* binary_file, int parameter_num){
    int found = 0;
    
    while(node != NULL){
        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(binary_file, offset, SEEK_SET);
        Bin_Data_t* bin_data = dataBinaryRead(binary_file);
        dataPrintCsvStyle(bin_data);
        putchar('\n');
        printf("compare: %d\n", dataParamCompare(bin_data, array, parameter_num));
    
        if(dataParamCompare(bin_data, array, parameter_num)){

            if(dataGetRemoved(bin_data) == '1'){
                dataDestroy(bin_data);            
                node = indexNodeGetNext(node);
                continue;
            }

            found++;

            int64_t str_size = varStrSize(bin_data);
            int64_t data_offset =  str_size + DATA_BASE_SIZE;
            dataPrintCsvStyle(bin_data);
            putchar('\n');
            dataMarkDeleted(binary_file, data_offset);
        }
        dataDestroy(bin_data);
        node = indexNodeGetNext(node);
    }

    return found;
}

int binarySearchIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index){
    int found = 0;

    printf("binaria\n");
    Index_Header_t* index_header = indexHeaderRead(index_file);
    int size = indexHeaderGetNum(index_header);
    int unique_node_num = 0;
    int index_pos = 0;

    Index_Node_t** node_array = indexArrayCreate(size);
    // isso aqui é modularizavel,  n?
    int param = indexDataGetParam(array[parameter_index]);
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &unique_node_num);
        int field_val = indexDataGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, unique_node_num, field_val);
    }
    else{
        printf("binaria str\n");
        dataIndexArrayStrRead(index_file, node_array, size, &unique_node_num);
        char* field_val = indexDataGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, unique_node_num, field_val);
    }

    if(index_pos == -1){
        
        indexArrayDestroy(node_array, size, unique_node_num);
        free(index_header);
        return 0;
    }

    Index_Node_t* n = node_array[index_pos];

    found = nodeListCompare(n, array, binary_file, parameter_num, 1);

    printf("found: %d\n", found);

    indexArrayDestroy(node_array, size, unique_node_num);
    free(index_header);

    return found;
}

int binarySearchDeleteIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index){

    int found = 0;
    
    Index_Header_t* index_header = indexHeaderRead(index_file);
    int size = indexHeaderGetNum(index_header);
    int unique_node_num = 0;
    int index_pos = 0;

    Index_Node_t** node_array = indexArrayCreate(size);
    int param = indexDataGetParam(array[parameter_index]);
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &unique_node_num);
        int field_val = indexDataGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, unique_node_num, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &unique_node_num);
        char* field_val = indexDataGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, unique_node_num, field_val);
    }

    if(index_pos == -1){
        
        indexArrayDestroy(node_array, size, unique_node_num);
        free(index_header);
        return 0;
    }

    Index_Node_t* n = node_array[index_pos];

    found = nodeListCompareDelete(n, array, binary_file, parameter_num);
    printf("found: %d\n", found);

    indexHeaderSetNum(index_header, indexHeaderGetNum(index_header) + found);

    indexArrayDestroy(node_array, size, unique_node_num);
    free(index_header);

    return found;
}

// retorno necessário?
char insertIntoBinaryFile(char* filename, char* index_file_name, int index_parameter){

    FILE* binary_file = binaryFileOpenReadWrite(filename);
    FILE* index_file = binaryFileOpenReadWrite(index_file_name);
    int size_array[2];
    Bin_Data_t* data;
    Bin_Header_t* header;
    Index_Header_t* index_header;
    int64_t offset;
    
    int index_pos = 0;

    // adicionar o registro no indice

    { // abre o bin header
        header = binHeaderRead(binary_file);
        if (headerGetStatus(header) == '0') {
            fileClose(binary_file);
            fileClose(index_file);
            errorFile();
        }

        headerSetStatus(header, '0');
    }

    { // abre o index header
        index_header = indexHeaderRead(index_file);
        if (indexHeaderGetStatus(index_header) == '0') {
            free(header);
            fileClose(binary_file);
            fileClose(index_file);
            errorFile();
        }

        indexHeaderSetStatus(index_header, '0');
        printf("size indice: %d\n", indexHeaderGetNum(index_header));
    }


    data = dataRead(size_array);
    dataPrintCsvStyle(data);
    putchar('\n');
    
    offset = headerGetOffset(header);
    int size = indexHeaderGetNum(index_header);

    dataBinaryWrite(binary_file, data, size_array, offset);
    // printf("ch: %d\n", DATA_BASE_SIZE + size_array[0] + size_array[1]);

    { // atualiza e escreve o bin header
        // printf("header: %c %ld %d %d\n", headerGetStatus(header), headerGetOffset(header), headerGetStructNum(header),headerGetRemStructNum(header));
        headerSetStructNum(header, headerGetStructNum(header) + 1);

        headerSetOffset(header, headerGetOffset(header) + DATA_BASE_SIZE + size_array[0] + size_array[1]);
    
        headerSetStatus(header, '1');

        // printf("header: %c %ld %d %d\n", headerGetStatus(header), headerGetOffset(header), headerGetStructNum(header),headerGetRemStructNum(header));
        binHeaderBinaryWrite(binary_file, header);
    }
    
    // node a ser inserido
    Index_Data_t* new_node_data = indexDataCreate();
    indexDataSetOffset(new_node_data, offset);
    
    Index_Node_t** node_array = indexArrayCreate(size);
    int unique_node_num;
    char index_checker = 1;
    // isso aqui é modularizavel,  n?
    if(index_parameter <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &unique_node_num);
        int field_val = dataGetIntField(data, index_parameter);
        if(field_val == -1){
            index_checker = 0;
            unique_node_num--;
        }
        indexDataSetIntKey(new_node_data, field_val);
        index_pos = binarySearchIndexInt(node_array, 0, unique_node_num - 1, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &unique_node_num);
        char * field_val = dataGetStrField(data, index_parameter);
        if(field_val == NULL){
            index_checker = 0;
            unique_node_num--;
        }
        indexDataSetStrKey(new_node_data, field_val);
        index_pos = binarySearchIndexStr(node_array, 0, unique_node_num - 1, field_val);
    }


    // verificar melhor se o valor do ngc do indice n é -1
    Index_Node_t* new_node = indexNodeCreate(new_node_data);

    printf("idx pos: %d\n idx_chk: %d\n", index_pos, (int)index_checker);

    if(index_checker == 1){
        
        { // atualiza e escreve o index header
            // printf("header: %c %d\n", indexHeaderGetStatus(index_header), indexHeaderGetNum(index_header));

            indexHeaderSetNum(index_header, indexHeaderGetNum(index_header) + 1);

            indexHeaderSetStatus(index_header, '1');

            // printf("header: %c %d\n", indexHeaderGetStatus(index_header), indexHeaderGetNum(index_header));
            indexHeaderWrite(index_file, index_header);
        }

        if(index_pos == -1){
            if(unique_node_num < size){
                node_array[unique_node_num] = new_node;
                unique_node_num++;
            }
            else{
                node_array = realloc(node_array, (++size) * sizeof(Index_Node_t *));
                if(node_array == NULL){
                    indexArrayDestroy(node_array, size, unique_node_num);
                    dataDestroy(data);
                    free(header);
                    free(index_header);
                    fileClose(binary_file);
                    fileClose(index_file);
                }
                unique_node_num++;
                node_array[size - 1] = new_node;
            }

        }
        else{
            Index_Node_t* node = node_array[index_pos];

            while(indexNodeGetNext(node) != NULL){
                node = indexNodeGetNext(node);
            }
            
            indexNodeStackData(node, new_node);
        }

        dataIndexArraySort(node_array, unique_node_num, index_parameter);

        dataIndexArrayWrite(index_file, node_array, unique_node_num);
    }

    indexArrayDestroy(node_array, size, unique_node_num);
    dataDestroy(data);
    free(header);
    free(index_header);
    fileClose(binary_file);
    fileClose(index_file);

    //mudar isso
    return 0;
}