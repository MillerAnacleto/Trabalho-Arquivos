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
