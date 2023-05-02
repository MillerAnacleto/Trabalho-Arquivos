#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>



//----------------------- structs para arquivo índicie -----------------------//

struct index_header_{
    char status;
    int struct_num;
};

struct index_data_ {
    int int_key;
    char *str_key;
    int64_t offset;
};

char indexHeaderGetStatus(Index_Header_t* header){
    return header->status;
}

void indexHeaderSetStatus(Index_Header_t* header, char status){
    header->status = status;
}

int indexHeaderGetNum(Index_Header_t* header){
    return header->struct_num;
}

void indexHeaderSetNum(Index_Header_t* header, int num){
    header->struct_num = num;
}

int indexDataGetIntKey(Index_Data_t* data){
    return data->int_key;
}

void indexDataSetIntKey(Index_Data_t* data, int key){
    data->int_key = key;
}

int64_t indexDataGetOffset(Index_Data_t* data){
    return data->offset;
}

void indexDataSetOffset(Index_Data_t* data, int64_t offset){
    data->offset = offset;
}

char* indexDataGetStrKey(Index_Data_t* data){
    return data->str_key;
}

void indexDataSetStrKey(Index_Data_t* data, char* key){
    data->str_key = key;
}

Index_Header_t* indexHeaderCreate(){
    Index_Header_t* header_indx = malloc(sizeof(Index_Header_t));
    header_indx->status = 1;
    header_indx->struct_num = 0;
    return header_indx;
}

Index_Data_t* indexDataCreate(){
    Index_Data_t* data = malloc(sizeof(Index_Data_t));
    data->str_key = NULL;
    data->offset = EMPTY_INT_FIELD;
    data->int_key = EMPTY_INT_FIELD;

    return data;
}

void indexHeaderDestroy(Index_Header_t* index_header){
    free(index_header);
}

void indexDataDestroy(Index_Data_t* data){

    if(data->str_key != NULL) free(data->str_key);
    free(data);
}

//---------------- estrutura de dados array com lista ligada -----------------//

struct index_node_{
    
    Index_Data_t* data;
    Index_Node_t* next;
};

Index_Node_t* indexNodeCreate(Index_Data_t* data){
    Index_Node_t* node = malloc(sizeof(Index_Node_t));
    node->data = data;
    node->next = NULL;

    return node;
}

Index_Node_t** indexArrayCreate(int node_num){

    Index_Node_t** array = malloc(node_num*sizeof(Index_Node_t*));
    
    for(int i = 0; i < node_num; i++){
        array[i] = malloc(sizeof(Index_Node_t));
        array[i]->data = NULL;
        array[i]->next = NULL;
    }

    return array;
}

Index_Data_t* indexNodeGetData(Index_Node_t* node){
    return node->data;
}

void indexNodeSetData(Index_Node_t** node, int pos, Index_Data_t* data){
    node[pos]->data = data;
}

Index_Node_t* indexNodeStackData(Index_Node_t* node, Index_Node_t* next){
    node->next = next;
    return node->next;
}

Index_Node_t* indexNodeGetNext(Index_Node_t* node){
    return node->next;
}

void indexNodeSetNext(Index_Node_t* node, Index_Node_t* next){
    node->next = next;
}

void indexArrayTrim(Index_Node_t** array, int node_num){

    array = realloc(array, node_num*sizeof(Index_Node_t*));
}

int indexDataStrCmp(const void *a, const void *b){
  
    Index_Node_t** elem1_ptr = (Index_Node_t **)a;
    Index_Node_t** elem2_ptr = (Index_Node_t **)b;

    Index_Node_t* elem1 = *elem1_ptr;
    Index_Node_t* elem2 = *elem2_ptr;

    char* str1 = elem1->data->str_key;
    char* str2 = elem2->data->str_key;
    
    for(int i = 0; i < STR_SIZE; i++){
        int j = str1[i] - str2[i];
        if(j != 0) return j;
    }

    return 0;
}

int indexDataIntCmp(const void *a, const void *b){
  
    Index_Node_t** elem1_ptr = (Index_Node_t **)a;
    Index_Node_t** elem2_ptr = (Index_Node_t **)b;

    Index_Node_t* elem1 = *elem1_ptr;
    Index_Node_t* elem2 = *elem2_ptr;

    int int1 = elem1->data->int_key;
    int int2 = elem2->data->int_key;
    
    int j = int1 - int2;
    if(j != 0) return j;

    return 0;
}

void freeNodeList(Index_Node_t* node){
    if(node == NULL) return;
    
    Index_Node_t* next = indexNodeGetNext(node);
    indexDataDestroy(node->data);
    free(node);
    freeNodeList(next);
}

void indexArrayDestroy(Index_Node_t** array, int size, int non_empty){

    for(int i = 0;  i < non_empty; i++){
        Index_Node_t* next = indexNodeGetNext(array[i]);
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
};

Bin_Data_t* dataCreate() {
    Bin_Data_t* data = calloc(1, sizeof(Bin_Data_t));
    data->removed = '0';
    data->delimiter = '#';

    return data;
}

void dataDestroy(Bin_Data_t* data) {

    free(data->crime_description);
    free(data->crime_place);

    free(data);
}

Bin_Header_t* headerCreate() {
    Bin_Header_t* header = calloc(1, sizeof(Bin_Header_t));

    header->num_struct_arch = 0;
    header->num_rem_structs = 0;
    header->next_offset_byte = 0;
    header->status = '1';

    return header;
}

char headerGetStatus(Bin_Header_t* header){
    return header->status;
}

void headerSetStatus(Bin_Header_t* header, char status){
    header->status = status;
}

int64_t headerGetOffset(Bin_Header_t* header){
    return header->next_offset_byte;
}

void headerSetOffset(Bin_Header_t* header, int64_t offset){
    header->next_offset_byte = offset;
}

int headerGetStructNum(Bin_Header_t* header){
    return header->num_struct_arch;
}

void headerSetStructNum(Bin_Header_t* header, int struct_num){
    header->num_struct_arch = struct_num;
}

int headerGetRemStructNum(Bin_Header_t* header){
    return header->num_rem_structs;
}

void headerSetRemStructNum(Bin_Header_t* header, int rem_struct_num){
    header->num_rem_structs = rem_struct_num;
}

char dataGetRemoved(Bin_Data_t* data){
    return data->removed;
}

void dataSetRemoved(Bin_Data_t* data, char removed){
    data->removed = removed;
}

int dataGetId(Bin_Data_t* data){
    return data->crime_id;
}

void dataSetId(Bin_Data_t* data, int id){
    data->crime_id = id;
}

char* dataGetDate(Bin_Data_t* data){
    return data->crime_date;
}

void dataSetDate(Bin_Data_t* data, char* date){
    for(int i = 0; i < DATE_SIZE; i++){
        data->crime_date[i] = date[i];
    }
}

int dataGetArticle(Bin_Data_t* data){
    return data->article_number;
}

void dataSetArticle(Bin_Data_t* data, int article){
    data->article_number = article;
}

char* dataGetPlace(Bin_Data_t* data){
    return data->crime_place;
}

void dataSetPlace(Bin_Data_t* data, char* crime_place){
    data->crime_place = crime_place;
}

char* dataGetDescription(Bin_Data_t* data){
    return data->crime_description;
}

void dataSetDescription(Bin_Data_t* data, char* description){
    data->crime_description = description;
}

char* dataGetBrand(Bin_Data_t* data){
    return data->cellphone_brand;
}

void dataSetBrand(Bin_Data_t* data, char* brand){
    for(int i = 0; i < BRAND_SIZE; i++){
        data->cellphone_brand[i] = brand[i];
    }
} 

char dataGetDelimiter(Bin_Data_t* data){
    return data->delimiter;
}

void dataSetDelimiter(Bin_Data_t* data, char delimiter){
    data->delimiter = delimiter;
} 