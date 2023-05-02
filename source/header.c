#include <stdio.h>
#include <stdlib.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

int binHeaderBinaryWrite(FILE* binary_file, Bin_Header_t* header) {
    int acc = 0;

    char status = headerGetStatus(header);
    acc += fwrite(&status, sizeof(char), 1, binary_file);

    int64_t offset_byte = headerGetOffset(header);
    acc += fwrite(&offset_byte, sizeof(int64_t), 1, binary_file);

    int struct_num = headerGetStructNum(header);
    acc += fwrite(&struct_num, sizeof(int32_t), 1, binary_file);

    int rem_structs = headerGetRemStructNum(header);
    acc += fwrite(&rem_structs, sizeof(int32_t), 1, binary_file);

    return acc;
}

Bin_Header_t* binHeaderBinaryRead(FILE* binary_file){
    Bin_Header_t* header = headerCreate();
    char checker;
    
    char status; 
    checker = fread(&status, sizeof(char), 1, binary_file);
    headerSetStatus(header, status);
    endFileAssert(checker, binary_file);

    int64_t offset_byte;
    checker = fread(&offset_byte, sizeof(int64_t), 1, binary_file);
    headerSetOffset(header, offset_byte);
    endFileAssert(checker, binary_file);

    int struct_num;
    checker = fread(&struct_num, sizeof(int32_t), 1, binary_file);
    headerSetStructNum(header, struct_num);
    endFileAssert(checker, binary_file);

    int num_rem;
    checker = fread(&num_rem, sizeof(int32_t), 1, binary_file);
    headerSetRemStructNum(header, num_rem);
    endFileAssert(checker, binary_file);

    return header;
}

Index_Header_t* indexHeaderRead(FILE* index){

    char status;
    int struct_num;
    int checker = fread(&status, sizeof(char), 1, index);
    checker += fread(&struct_num, sizeof(int), 1, index);
    
    if(status == 0 || checker == 0){
        return NULL; //não é possível ler o arquivo de indice
    }

    Index_Header_t* header = indexHeaderCreate();
    indexHeaderSetNum(header, struct_num);
    indexHeaderSetStatus(header, status);
    
    return header;
}

int indexHeaderWrite(FILE* binary_file, Index_Header_t* header){

    int acc = 0;
    char status = indexHeaderGetStatus(header);
    int num = indexHeaderGetNum(header); 
    acc += fwrite(&status, sizeof(char), 1, binary_file);
    acc += fwrite(&num, sizeof(int), 1, binary_file);

    return acc;
}