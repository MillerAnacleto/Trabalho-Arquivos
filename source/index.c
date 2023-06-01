#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <data.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

void dataIndexArraySort(Index_Node** index_array, int size, int parameter){

    if(parameter <= 1){
        qsort(index_array, size, sizeof(Index_Node*), indexDataIntCmp);
        
        return;
    }

    qsort(index_array, size, sizeof(Index_Node*), indexDataStrCmp);
}

int dataIndexArrayWrite(FILE* index_file, Index_Node** index_array, int size, int parameter){

    int acc = 0;
    Index_Data* data;

    for(int i = 0; i < size; i++){

        Index_Node* next = index_array[i];

        while(next != NULL){

            data = indexNodeGetData(next);
            int64_t offset = indexDataGetOffset(data);

            if(parameter <= 1){
                int int_key =  indexDataGetIntKey(data);
                if(int_key != -1){
                    acc += fwrite(&int_key, sizeof(int), 1, index_file);
                }
            }
            else{
                char* char_key = indexDataGetStrKey(data);
                if(char_key != NULL){
                    acc += fwrite(char_key, sizeof(char), STR_SIZE, index_file);
                }
            }
            acc += fwrite(&offset, sizeof(int64_t), 1, index_file);
            next = indexNodeGetNext(next);
        }
    }

    return acc;
}

Index_Data* indexDataReadInt(FILE* index){
    Index_Data* data = indexDataCreate();
    int key = EMPTY_INT_FIELD;
    int64_t offset = -1;

    char checker = '\0';

    checker = fread(&key, sizeof(int), 1, index);
    endFileAssert(checker, index);

    checker = fread(&offset, sizeof(int64_t), 1, index);
    endFileAssert(checker, index);

    indexDataSetIntKey(data, key);
    indexDataSetOffset(data, offset);
    

    return data;
}

Index_Data* indexDataReadStr(FILE* index){
    Index_Data* data = indexDataCreate();
    int64_t offset = -1;
    char str[12];

    char checker = '\0';

    checker = fread(str, sizeof(char), 12, index);
    endFileAssert(checker, index);

    checker = fread(&offset, sizeof(int64_t), 1, index);
    endFileAssert(checker, index);

    indexDataSetOffset(data, offset);
    indexDataSetStrKey(data, str);

    return data;
}

void dataIndexArrayIntRead(FILE* index, Index_Node** array, int size, int* diff_node_num){

    
    int curr_pos = 0;
    int last_val = 0;
    int curr_val = 0;

    Index_Node* curr_elem = NULL;
    Index_Node* next_elem = NULL;
    Index_Data* data = indexDataReadInt(index);
    last_val = indexDataGetIntKey(data);

    indexNodeSetData(array, curr_pos, data);
    curr_pos++;
    curr_elem = array[0];

    for(int i = 1; i < size; i++){
        Index_Data* data = indexDataReadInt(index);
        curr_val = indexDataGetIntKey(data);
        
        if(curr_val == last_val){
            Index_Node* next = indexNodeCreate(data);
            next_elem = indexNodeStackData(curr_elem, next);
            curr_elem = next_elem;
        }
        else{
            indexNodeSetData(array, curr_pos, data);
            curr_elem = array[curr_pos];
            last_val = curr_val;
            curr_pos++;
        }
    }
    *diff_node_num = curr_pos;
}

void dataIndexArrayStrRead(FILE* index, Index_Node** array, int size, int* diff_node_num){

    int curr_pos = 0;
    char* last_val;
    char* curr_val;

    Index_Node* curr_elem = NULL;
    Index_Node* next_elem = NULL;
    Index_Data* data = indexDataReadStr(index);
    last_val = indexDataGetStrKey(data);

    indexNodeSetData(array, curr_pos, data);
    curr_pos++;
    curr_elem = array[0];

    for(int i = 1; i < size; i++){
        Index_Data* data = indexDataReadStr(index);
        curr_val = indexDataGetStrKey(data);
        
        if( stringnCmp(curr_val, last_val, STR_SIZE) == 0){
            Index_Node* next = indexNodeCreate(data);
            next_elem = indexNodeStackData(curr_elem, next);
            curr_elem = next_elem;
        }
        else{
            indexNodeSetData(array, curr_pos, data);
            curr_elem = array[curr_pos];
            last_val = curr_val;
            curr_pos++;
        }
    }
    *diff_node_num = curr_pos;
}