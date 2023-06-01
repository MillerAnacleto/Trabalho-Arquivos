#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>

#define BRAND_SIZE 12
#define DATE_SIZE 10
#define STR_SIZE 12

//os blocos seguem a lógica: alocadores, get/set, desalocadores
//--------------------------- structs para arquivo ---------------------------//

typedef struct file_container_ File_Container;

File_Container* fileContainerCreate();

FILE* fileContainerGetFile(File_Container* file_cont);
void fileContainerSetFile(File_Container* file_cont, FILE* file);

int64_t fileContainerGetPtrPos(File_Container* file_cont);
void fileContainerSetPtrPos(File_Container* file_cont, int64_t ptr_pos);

int64_t fileContainerGetOffset(File_Container* file_cont);
void fileContainerSetOffset(File_Container* file_cont, int64_t max_offset);

//----------------------- structs para arquivo índicie -----------------------//

typedef struct index_header_ Index_Header;
typedef struct index_data_ Index_Data;

Index_Header* indexHeaderCreate();
Index_Data* indexDataCreate();

char indexHeaderGetStatus(Index_Header* header);
void indexHeaderSetStatus(Index_Header* header, char status);

int indexHeaderGetNum(Index_Header* header);
void indexHeaderSetNum(Index_Header* header, int num);

int64_t indexDataGetOffset(Index_Data* data);
void indexDataSetOffset(Index_Data* data, int64_t offset);

int indexDataGetIntKey(Index_Data* data);
void indexDataSetIntKey(Index_Data* data, int key);

char* indexDataGetStrKey(Index_Data* data);
void indexDataSetStrKey(Index_Data* data, char* key);

void indexHeaderDestroy(Index_Header* index_header);
void indexDataDestroy(Index_Data* int_data);
void stringDataDestroy(Index_Data* string_data);


//---------------- estruturas para parâmetros de busca -----------------------//

typedef struct parameter_hold Parameter_Hold;

Parameter_Hold* parameterHoldCreate();
Parameter_Hold** parameterArrayCreate(int parameter_num);

int paramHoldGetIntKey(Parameter_Hold* param);
void paramHoldSetIntKey(Parameter_Hold* param, int key);

int paramHoldGetVal(Parameter_Hold* param);
void paramHoldSetVal(Parameter_Hold* param, int param_val);

char* paramHoldGetStrKey(Parameter_Hold* param);
void paramHoldSetStrKey(Parameter_Hold* param, char* key);

void parameterArrayDestroy(Parameter_Hold** array, int size);
void parameterHoldDestroy(Parameter_Hold* param);


//---------------- estrutura de dados array com lista ligada -----------------//

typedef struct index_node_ Index_Node;

Index_Node* indexNodeCreate(Index_Data* data);
Index_Node** indexArrayCreate(int node_num);

Index_Data* indexNodeGetData(Index_Node* node);
void indexNodeSetData(Index_Node** node, int pos, Index_Data* data);

Index_Node* indexNodeGetNext(Index_Node* node);
void indexNodeSetNext(Index_Node* node, Index_Node* next);

Index_Node* indexNodeStackData(Index_Node* node, Index_Node* next);

//funções de comparação usadas no qsort();
int indexDataStrCmp(const void *a, const void *b);
int indexDataIntCmp(const void *a, const void *b);

void indexArrayDestroy(Index_Node** array, int size, int non_empty);

//----------------------- structs para arquivo binário -----------------------// 

typedef struct Header Data_Header;
typedef struct Data Data_Register;
// functions to create/free the structs
Data_Register* dataCreate();
void dataDestroy(Data_Register* data);
Data_Header* headerCreate();

char headerGetStatus(Data_Header* header);
void headerSetStatus(Data_Header* header, char status);

int64_t headerGetOffset(Data_Header* header);
void headerSetOffset(Data_Header* header, int64_t offset);

int headerGetStructNum(Data_Header* header);
void headerSetStructNum(Data_Header* header, int struct_num);

int headerGetRemStructNum(Data_Header* header);
void headerSetRemStructNum(Data_Header* header, int rem_struct_num);

char dataGetRemoved(Data_Register* data);
void dataSetRemoved(Data_Register* data, char removed);

int dataGetId(Data_Register* data);
void dataSetId(Data_Register* data, int id);

char* dataGetDate(Data_Register* data);
void dataSetDate(Data_Register* data, char* date);

int dataGetArticle(Data_Register* data);
void dataSetArticle(Data_Register* data, int article);

char* dataGetPlace(Data_Register* data);
void dataSetPlace(Data_Register* data, char* crime_place);

char* dataGetDescription(Data_Register* data);
void dataSetDescription(Data_Register* data, char* description);

char* dataGetBrand(Data_Register* data);
void dataSetBrand(Data_Register* data, char* brand);

char dataGetDelimiter(Data_Register* data);
void dataSetDelimiter(Data_Register* data, char delimiter);

int dataGetExtraSize(Data_Register* data);
void dataSetExtraSize(Data_Register* data, int extra_size);

int dataGetSize(Data_Register* data);

#endif // !STRUCTS_H_