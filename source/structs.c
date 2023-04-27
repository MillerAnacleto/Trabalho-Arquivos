#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

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

Data_t* dataCreate() {
    Data_t* data = calloc(1, sizeof(Data_t));
    data->removed = '0';
    data->delimiter = '#';

    return data;
}

void dataFree(Data_t* data) {

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

char dataGetRemoved(Data_t* data){
    return data->removed;
}

void dataSetRemoved(Data_t* data, char removed){
    data->removed = removed;
}

int dataGetId(Data_t* data){
    return data->crime_id;
}

void dataSetId(Data_t* data, int id){
    data->crime_id = id;
}

char* dataGetDate(Data_t* data){
    return data->crime_date;
}

void dataSetDate(Data_t* data, char* date){
    for(int i = 0; i < DATE_SIZE; i++){
        data->crime_date[i] = date[i];
    }
}

int dataGetArticle(Data_t* data){
    return data->article_number;
}

void dataSetArticle(Data_t* data, int article){
    data->article_number = article;
}

char* dataGetPlace(Data_t* data){
    return data->crime_place;
}

void dataSetPlace(Data_t* data, char* crime_place){
    data->crime_place = crime_place;
}

char* dataGetDescription(Data_t* data){
    return data->crime_description;
}

void dataSetDescription(Data_t* data, char* description){
    data->crime_description = description;
}

char* dataGetBrand(Data_t* data){
    return data->cellphone_brand;
}

void dataSetBrand(Data_t* data, char* brand){
    for(int i = 0; i < BRAND_SIZE; i++){
        data->cellphone_brand[i] = brand[i];
    }
} 

char dataGetDelimiter(Data_t* data){
    return data->delimiter;
}

void dataSetDelimiter(Data_t* data, char delimiter){
    data->delimiter = delimiter;
} 