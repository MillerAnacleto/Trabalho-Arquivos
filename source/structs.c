#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <files.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <data.h>
#include <structs.h>

//----------------------- structs para arquivo índicie -----------------------//

struct index_header_{
    char status;
    int struct_num;
};

struct index_data_{

    union Keys_{
        int int_key;
        char str_key[STR_SIZE];
    }keys;

    int64_t offset;
    
};

char indexHeaderGetStatus(Index_Header* header){
    return header->status;
}

void indexHeaderSetStatus(Index_Header* header, char status){
    header->status = status;
}

int indexHeaderGetNum(Index_Header* header){
    return header->struct_num;
}

void indexHeaderSetNum(Index_Header* header, int num){
    header->struct_num = num;
}

int indexDataGetIntKey(Index_Data* data){
    return data->keys.int_key;
}

void indexDataSetIntKey(Index_Data* data, int key){
    data->keys.int_key = key;
}

int64_t indexDataGetOffset(Index_Data* data){
    return data->offset;
}

void indexDataSetOffset(Index_Data* data, int64_t offset){
    data->offset = offset;
}

char* indexDataGetStrKey(Index_Data* data){
    return data->keys.str_key;
}

void indexDataSetStrKey(Index_Data* data, char* key){
    for(int i = 0; i < STR_SIZE; i++){
        data->keys.str_key[i] = key[i];
    }
}

Index_Header* indexHeaderCreate(){
    Index_Header* header_indx = malloc(sizeof(Index_Header));
    header_indx->status = '1';
    header_indx->struct_num = 0;
    return header_indx;
}

Index_Data* indexDataCreate(){
    Index_Data* data = malloc(sizeof(Index_Data));
    data->keys.str_key[0] = '\0';
    data->offset = EMPTY_INT_FIELD;
    data->keys.int_key = EMPTY_INT_FIELD;

    return data;
}

void indexHeaderDestroy(Index_Header* index_header){
    free(index_header);
}

void indexDataDestroy(Index_Data* data){
    free(data);
}

//---------------- estruturas para parâmetros de busca -----------------------//
struct parameter_hold{

    int int_key;
    char* str_key;
    int param_val;
};

Parameter_Hold* parameterHoldCreate(){

    Parameter_Hold* param = malloc(sizeof(Parameter_Hold));
    param->int_key = EMPTY_INT_FIELD;
    param->param_val = EMPTY_INT_FIELD;
    param->str_key = NULL;

    return param;
};

Parameter_Hold** parameterArrayCreate(int parameter_num){

    Parameter_Hold** arr = malloc(parameter_num*sizeof(Parameter_Hold*));

    for(int i = 0; i < parameter_num; i++){
        arr[i] = parameterHoldCreate();
    }

    return arr; 
}

void parameterHoldDestroy(Parameter_Hold* param){
    if(param->str_key != NULL)
        free(param->str_key);
    free(param);
}

void parameterArrayDestroy(Parameter_Hold** array, int size){
    for(int i = 0; i < size; i++){
        //if(array[i] != NULL)
            parameterHoldDestroy(array[i]);
    }
    free(array);
}

void paramHoldSetIntKey(Parameter_Hold* param, int key){
    param->int_key = key;
}

int paramHoldGetIntKey(Parameter_Hold* param){
    return param->int_key;
}

int paramHoldGetVal(Parameter_Hold* param){
    return param->param_val;
}

void paramHoldSetVal(Parameter_Hold* param, int param_val){
    param->param_val = param_val;
}

char* paramHoldGetStrKey(Parameter_Hold* param){
    return param->str_key;
}

void paramHoldSetStrKey(Parameter_Hold* param, char* key){
    param->str_key = key;
}

//---------------- estrutura de dados array com lista ligada -----------------//

struct index_node_{
    
    Index_Data* data;
    Index_Node* next;
};

Index_Node* indexNodeCreate(Index_Data* data){
    Index_Node* node = malloc(sizeof(Index_Node));
    node->data = data;
    node->next = NULL;

    return node;
}

Index_Node** indexArrayCreate(int node_num){

    Index_Node** array = malloc(node_num*sizeof(Index_Node*));
    
    for(int i = 0; i < node_num; i++){
        array[i] = malloc(sizeof(Index_Node));
        array[i]->data = NULL;
        array[i]->next = NULL;
    }

    return array;
}

Index_Data* indexNodeGetData(Index_Node* node){
    return node->data;
}

void indexNodeSetData(Index_Node** node, int pos, Index_Data* data){
    node[pos]->data = data;
}

Index_Node* indexNodeStackData(Index_Node* node, Index_Node* next){
    node->next = next;
    return node->next;
}

Index_Node* indexNodeGetNext(Index_Node* node){
    return node->next;
}

void indexNodeSetNext(Index_Node* node, Index_Node* next){
    node->next = next;
}

int indexDataStrCmp(const void *a, const void *b){
  
    Index_Node** elem1_ptr = (Index_Node **)a;
    Index_Node** elem2_ptr = (Index_Node **)b;

    Index_Node* elem1 = *elem1_ptr;
    Index_Node* elem2 = *elem2_ptr;

    char* str1 = elem1->data->keys.str_key;
    char* str2 = elem2->data->keys.str_key;
    
    for(int i = 0; i < STR_SIZE; i++){
        int j = str1[i] - str2[i];
        if(j != 0) return j;
    }

    return (elem1->data->offset - elem2->data->offset);
}

int indexDataIntCmp(const void *a, const void *b){
  
    Index_Node** elem1_ptr = (Index_Node **)a;
    Index_Node** elem2_ptr = (Index_Node **)b;

    Index_Node* elem1 = *elem1_ptr;
    Index_Node* elem2 = *elem2_ptr;

    int int1 = elem1->data->keys.int_key;
    int int2 = elem2->data->keys.int_key;
    
    int j = int1 - int2;
    if(j != 0) return j;

    return (elem1->data->offset - elem2->data->offset);
}

void freeNodeList(Index_Node* node){
    if(node == NULL) return;
    
    Index_Node* next = indexNodeGetNext(node);
    indexDataDestroy(node->data);
    free(node);
    freeNodeList(next);
}

void indexArrayDestroy(Index_Node** array, int size, int non_empty){

    for(int i = 0;  i < non_empty; i++){
        Index_Node* next = indexNodeGetNext(array[i]);
        freeNodeList(next);
        indexDataDestroy(array[i]->data);
        free(array[i]);
    }
    for(int i = non_empty; i < size; i++){
        free(array[i]);
    }

    free(array);
}

//----------------------- structs para arquivo binário -----------------------// 

struct Header {
    char status;
    int64_t next_offset_byte;
    int32_t num_struct_arch;
    int32_t num_rem_structs;
};

struct Data {
    char removed;
    int32_t crime_id;
    char crime_date[DATE_SIZE];
    int32_t article_number;
    char* crime_place;
    char* crime_description;
    char cellphone_brand[BRAND_SIZE];
    char delimiter;
    int extra_size; //tamanho adicional que pode ocorrer pelo preenchimento no update
};

Data_Register* dataCreate() {
    Data_Register* data = calloc(1, sizeof(Data_Register));
    data->removed = '0';
    data->delimiter = '#';
    data->extra_size = 0;

    return data;
}

void dataDestroy(Data_Register* data) {

    free(data->crime_description);
    free(data->crime_place);

    free(data);
}

Data_Header* headerCreate() {
    Data_Header* header = calloc(1, sizeof(Data_Header));

    header->num_struct_arch = 0;
    header->num_rem_structs = 0;
    header->next_offset_byte = 0;
    header->status = '1';

    return header;
}

char headerGetStatus(Data_Header* header){
    return header->status;
}

void headerSetStatus(Data_Header* header, char status){
    header->status = status;
}

int64_t headerGetOffset(Data_Header* header){
    return header->next_offset_byte;
}

void headerSetOffset(Data_Header* header, int64_t offset){
    header->next_offset_byte = offset;
}

int headerGetStructNum(Data_Header* header){
    return header->num_struct_arch;
}

void headerSetStructNum(Data_Header* header, int struct_num){
    header->num_struct_arch = struct_num;
}

int headerGetRemStructNum(Data_Header* header){
    return header->num_rem_structs;
}

void headerSetRemStructNum(Data_Header* header, int rem_struct_num){
    header->num_rem_structs = rem_struct_num;
}

char dataGetRemoved(Data_Register* data){
    return data->removed;
}

void dataSetRemoved(Data_Register* data, char removed){
    data->removed = removed;
}

int dataGetId(Data_Register* data){
    return data->crime_id;
}

void dataSetId(Data_Register* data, int id){
    data->crime_id = id;
}

char* dataGetDate(Data_Register* data){
    return data->crime_date;
}

void dataSetDate(Data_Register* data, char* date){
    for(int i = 0; i < DATE_SIZE; i++){
        data->crime_date[i] = date[i];
    }
}

int dataGetArticle(Data_Register* data){
    return data->article_number;
}

void dataSetArticle(Data_Register* data, int article){
    data->article_number = article;
}

char* dataGetPlace(Data_Register* data){
    return data->crime_place;
}

void dataSetPlace(Data_Register* data, char* crime_place){
    data->crime_place = crime_place;
}

char* dataGetDescription(Data_Register* data){
    return data->crime_description;
}

void dataSetDescription(Data_Register* data, char* description){
    data->crime_description = description;
}

char* dataGetBrand(Data_Register* data){
    return data->cellphone_brand;
}

void dataSetBrand(Data_Register* data, char* brand){
    for(int i = 0; i < BRAND_SIZE; i++){
        data->cellphone_brand[i] = brand[i];
    }
} 

char dataGetDelimiter(Data_Register* data){
    return data->delimiter;
}

void dataSetDelimiter(Data_Register* data, char delimiter){
    data->delimiter = delimiter;
} 

int dataGetExtraSize(Data_Register* data){
    return data->extra_size;
}

void dataSetExtraSize(Data_Register* data, int extra_size){
    data->extra_size = extra_size;
} 

int dataGetSize(Data_Register* data){
    
    int total_size = 0;
    int str_size = varStrTell(dataGetPlace(data));
    str_size += varStrTell(dataGetDescription(data));
    total_size = dataGetExtraSize(data);
    total_size += str_size;

    return total_size + DATA_BASE_SIZE;
}