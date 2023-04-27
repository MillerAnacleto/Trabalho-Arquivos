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

typedef struct Header Bin_Header_t;
typedef struct Data Data_t;

// functions to create/free the structs
Data_t* dataCreate();
void dataFree(Data_t* data);
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