#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

typedef struct index_header_ Index_Bin_Header_t;
typedef struct int_data_ Index_Int_t;
typedef struct str_data_ Index_String_t;

Index_Bin_Header_t* indexHeaderCreate();
Index_Int_t* indexIntDataCreate();
Index_String_t* indexStringDataCreate();

int searchParameter();
void readBinaryWriteIndex(FILE* file);

void indexHeaderDestroy(Index_Bin_Header_t* index_header);
void intDataDestroy(Index_Int_t* int_data);
void stringDataDestroy(Index_String_t* string_data);


#endif // !INDEX_H