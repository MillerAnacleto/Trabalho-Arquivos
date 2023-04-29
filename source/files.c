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
    Bin_Header_t* header = binHeaderBinaryRead(binary_file);
    if (headerGetStatus(header) == '0') {
        fileClose(binary_file);
        errorFile();
    }

    Data_t* data;
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
    Data_t* data;

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

void fileIndexCreate(char* binary_file_name, char* index_file_name, int parameter, int64_t *offset){

    FILE* read_file = binaryFileOpenRead(binary_file_name);

    Bin_Header_t* header = binHeaderBinaryRead(read_file);
    int size = headerGetStructNum(header);
    *offset += 17; //offset provocado pelo header
    char* str = NULL;

    Index_Header_t* index_header = indexHeaderCreate();
    indexHeaderSetStatus(index_header, '0');
    //escrever o header

    Index_Node_t** index_array = indexArrayCreate(size);
    
    int non_empty = 0;
    for(int i = 0; i < size; i++){
        Index_Data_t* data = indexDataCreate();
        
        int64_t offset_temp = *offset;
        indexDataSetOffset(data, offset_temp);

        int int_key = EMPTY_INT_FIELD;
        char exists = 0;
        str = readBinaryField(read_file, parameter, offset, &int_key, &exists);
        indexDataSetStrKey(data, str);
        indexDataSetIntKey(data, int_key);
        
        if(exists){
            indexArraySetData(index_array, non_empty, data);
            non_empty++;
        }
        else{
            free(data);
        }
    }

    dataIndexArraySort(index_array, non_empty, parameter);

    FILE* index_file = binaryFileOpenWrite(index_file_name);
    
    indexHeaderWrite(index_file, index_header);
    indexArrayWrite(index_file, index_array, non_empty);

    indexArrayDestroy(index_array, size, non_empty);    

    indexHeaderSetStatus(index_header, '1');
    fseek(index_file,0, SEEK_SET);
    indexHeaderWrite(index_file, index_header);
    
    free(index_header);
    free(header);

    fileClose(read_file);
    fileClose(index_file);

    binarioNaTela(index_file_name);
}

