#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

#define BRAND_SIZE 12
#define DATE_SIZE 10
#define STR_SIZE 12

//----------------------- structs para arquivo índicie -----------------------//

typedef struct index_header_ Index_Header_t;
typedef struct index_data_ Index_Data_t;

Index_Header_t* indexHeaderCreate();
Index_Data_t* indexDataCreate();

char indexHeaderGetStatus(Index_Header_t* header);
void indexHeaderSetStatus(Index_Header_t* header, char status);

int indexHeaderGetNum(Index_Header_t* header);

void indexHeaderSetNum(Index_Header_t* header, int num);

int64_t indexDataGetOffset(Index_Data_t* data);
void indexDataSetOffset(Index_Data_t* data, int64_t offset);

int indexDataGetIntKey(Index_Data_t* data);
void indexDataSetIntKey(Index_Data_t* data, int key);

char* indexDataGetStrKey(Index_Data_t* data);
void indexDataSetStrKey(Index_Data_t* data, char* key);

void indexHeaderDestroy(Index_Header_t* index_header);
void indexDataDestroy(Index_Data_t* int_data);
void stringDataDestroy(Index_Data_t* string_data);

//---------------- estrutura de dados array com lista ligada -----------------//



typedef struct index_node_ Index_Node_t;

Index_Node_t* indexNodeCreate(Index_Data_t* data);
Index_Node_t** indexArrayCreate(int node_num);

Index_Data_t* indexNodeGetData(Index_Node_t* node);
void indexNodeSetData(Index_Node_t** node, int pos, Index_Data_t* data);

Index_Node_t* indexNodeGetNext(Index_Node_t* node);
void indexNodeSetNext(Index_Node_t* node, Index_Node_t* next);

Index_Node_t* indexNodeStackData(Index_Node_t* node, Index_Node_t* next);

int indexDataStrCmp(const void *a, const void *b);
int indexDataIntCmp(const void *a, const void *b);

void indexArrayTrim(Index_Node_t** array, int node_num);

void indexArrayDestroy(Index_Node_t** array, int size, int non_empty);

//----------------------- structs para arquivo binário -----------------------// 

typedef struct Header Bin_Header_t;
typedef struct Data Data_t;
// functions to create/free the structs
Data_t* dataCreate();
void dataDestroy(Data_t* data);
Bin_Header_t* headerCreate();

char headerGetStatus(Bin_Header_t* header);
void headerSetStatus(Bin_Header_t* header, char status);

int64_t headerGetOffset(Bin_Header_t* header);
void headerSetOffset(Bin_Header_t* header, int64_t offset);

int headerGetStructNum(Bin_Header_t* header);
void headerSetStructNum(Bin_Header_t* header, int struct_num);

int headerGetRemStructNum(Bin_Header_t* header);
void headerSetRemStructNum(Bin_Header_t* header, int rem_struct_num);

char dataGetRemoved(Data_t* data);
void dataSetRemoved(Data_t* data, char removed);

int dataGetId(Data_t* data);
void dataSetId(Data_t* data, int id);

char* dataGetDate(Data_t* data);
void dataSetDate(Data_t* data, char* date);

int dataGetArticle(Data_t* data);
void dataSetArticle(Data_t* data, int article);

char* dataGetPlace(Data_t* data);
void dataSetPlace(Data_t* data, char* crime_place);

char* dataGetDescription(Data_t* data);
void dataSetDescription(Data_t* data, char* description);

char* dataGetBrand(Data_t* data);
void dataSetBrand(Data_t* data, char* brand);

char dataGetDelimiter(Data_t* data);
void dataSetDelimiter(Data_t* data, char delimiter);

#endif // !STRUCTS_H_