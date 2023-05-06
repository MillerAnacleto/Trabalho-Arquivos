#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

Bin_Data_t* dataCsvRead(FILE* csv_file, int* size_array) {
    Bin_Data_t* data = dataCreate();
    int str_size = 0;

    if (endFileChecker(csv_file)) {
        free(data);
        return NULL;
    }

    dataSetId(data, readCsvInt(csv_file));
    
    char crime_date[DATE_SIZE];
    readCsvConstString(csv_file, crime_date, DATE_SIZE);
    dataSetDate(data, crime_date);

    dataSetArticle(data, readCsvInt(csv_file));

    dataSetPlace(data, readCsvVarString(csv_file, &str_size));
    size_array[0] = str_size;
    
    dataSetDescription(data, readCsvVarString(csv_file, &str_size));
    
    size_array[1] = str_size;
    
    char cell_brand[BRAND_SIZE];
    readCsvConstString(csv_file, cell_brand, BRAND_SIZE);
    dataSetBrand(data, cell_brand);
    
    return data;
}

Bin_Data_t* dataBinaryRead(FILE* binary_file) {
    Bin_Data_t* data = dataCreate();
    char checker;

    char rmvd;
    checker = fread(&rmvd, sizeof(char), 1, binary_file);
    dataSetRemoved(data, rmvd);
    endFileAssert(checker, binary_file);

    int id;
    checker = fread(&id, sizeof(int32_t), 1, binary_file);
    dataSetId(data, id);
    endFileAssert(checker, binary_file);

    char crime_date[DATE_SIZE];
    checker = fread(crime_date, (DATE_SIZE) * sizeof(char), 1, binary_file);
    dataSetDate(data, crime_date);
    endFileAssert(checker, binary_file);

    int article;
    checker = fread(&article, sizeof(int32_t), 1, binary_file);
    dataSetArticle(data, article);
    endFileAssert(checker, binary_file);

    char brand[BRAND_SIZE];
    checker = fread(brand, (BRAND_SIZE) * sizeof(char), 1, binary_file);
    dataSetBrand(data, brand);
    endFileAssert(checker, binary_file);

    char* crime_place;
    crime_place = readBinaryVarString(binary_file);
    dataSetPlace(data, crime_place);
    endFileAssert(checker, binary_file);

    char* description;
    description = readBinaryVarString(binary_file);
    dataSetDescription(data, description);
    endFileAssert(checker, binary_file);

    char delimiter;
    checker = fread(&delimiter, sizeof(char), 1, binary_file);
    dataSetDelimiter(data, delimiter);
    endFileAssert(checker, binary_file);

    return data;
}

int dataBinaryWrite(FILE* binary_file, Bin_Data_t* data, int* size_array) {
    int acc = 0; // accumulator for write verification
    
    char rmvd = dataGetRemoved(data);
    acc += fwrite(&rmvd, sizeof(char), 1, binary_file);
    
    int crime_id = dataGetId(data);
    acc += fwrite(&crime_id, sizeof(int32_t), 1, binary_file);

    char* date = dataGetDate(data);
    acc += fwrite(date, sizeof(char), DATE_SIZE, binary_file);

    int article = dataGetArticle(data);
    acc += fwrite(&article, sizeof(int32_t), 1, binary_file);

    char* brand = dataGetBrand(data);
    acc += fwrite(brand, sizeof(char), BRAND_SIZE, binary_file);

    char* crime_place = dataGetPlace(data);
    acc += fwrite(crime_place, sizeof(char), size_array[0], binary_file);

    char* crime_description = dataGetDescription(data);
    acc += fwrite(crime_description, sizeof(char), size_array[1], binary_file);

    char delimiter = dataGetDelimiter(data);
    acc += fwrite(&delimiter, sizeof(char), 1, binary_file);

    return acc;
}

char* readBinaryVarString(FILE* binary_file) {
    char* str = malloc(sizeof(char));
    char last_read = '\0';
    int str_size = 0;

    char checker = fread(&last_read, sizeof(char), 1, binary_file);
    endFileAssert(checker, binary_file);
    if (last_read == '|') {
        str[str_size] = last_read;
        str_size++;
        str = realloc(str, (str_size + 1) * sizeof(char));
    }

    while (last_read != '|') {
        str[str_size] = last_read;
        str_size++;
        checker = fread(&last_read, sizeof(char), 1, binary_file);
        endFileAssert(checker, binary_file);

        str = realloc(str, (str_size + 1) * sizeof(char));
    }
    str[str_size] = '\0';

    return str;
}

void dataPrintCsvStyle(Bin_Data_t* data) {
    char* aux_str;

    printf("%d, ", dataGetId(data));
    
    char* date = dataGetDate(data);
    if (!isConstStringNull(date)) {
        printConstString(date, DATE_SIZE);
        printf(", ");
    } else
        printf("NULO, ");

    int article = dataGetArticle(data);
    if (article != -1) {
        printf("%d, ", article);
    } else {
        printf("NULO, ");
    }

    aux_str = dataGetPlace(data);
    if (aux_str[0] != '|') {
        printf("%s, ", aux_str);
    } else {
        printf("NULO, ");
    }

    aux_str = dataGetDescription(data);
    if (aux_str[0] != '|') {
        printf("%s, ", aux_str);
    } else {
        printf("NULO, ");
    }

    char* brand = dataGetBrand(data);
    if (!isConstStringNull(brand)) {
        printConstString(brand, BRAND_SIZE);
    } else {
        printf("NULO");
    }
}

void dataIndexArraySort(Index_Node_t** index_array, int size, int parameter){

    if(parameter <= 1){
        qsort(index_array, size, sizeof(Index_Node_t*), indexDataIntCmp);
        
        return;
    }

    qsort(index_array, size, sizeof(Index_Node_t*), indexDataStrCmp);
}

int dataIndexArrayWrite(FILE* index_file, Index_Node_t** index_array, int size, int parameter){

    int acc = 0;
    Index_Data_t* data;

    for(int i = 0; i < size; i++){

        Index_Node_t* next = index_array[i];

        while(next != NULL){

            data = indexNodeGetData(index_array[i]);
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

Index_Data_t* indexDataReadInt(FILE* index){
    Index_Data_t* data = indexDataCreate();
    int key = EMPTY_INT_FIELD;
    int64_t offset = -1;

    fread(&key, sizeof(int), 1, index);
    fread(&offset, sizeof(int64_t), 1, index);

    indexDataSetIntKey(data, key);
    indexDataSetOffset(data, offset);
    

    return data;
}

Index_Data_t* indexDataReadStr(FILE* index){
    Index_Data_t* data = indexDataCreate();
    int64_t offset = -1;
    char str[12];

    fread(str, sizeof(char), 12, index);
    fread(&offset, sizeof(int64_t), 1, index);

    indexDataSetOffset(data, offset);
    indexDataSetStrKey(data, str);

    return data;
}

void dataIndexArrayIntRead(FILE* index, Index_Node_t** array, int size, int* node_num, int* diff_node_num){

    int curr_pos = 0;
    int last_val = 0;
    int curr_val = 0;

    Index_Node_t* curr_elem = NULL;
    Index_Node_t* next_elem = NULL;
    Index_Data_t* data = indexDataReadInt(index);
    last_val = indexDataGetIntKey(data);

    indexNodeSetData(array, curr_pos, data);
    curr_pos++;
    curr_elem = array[0];

    for(int i = 1; i < size; i++){
        Index_Data_t* data = indexDataReadInt(index);
        curr_val = indexDataGetIntKey(data);
        
        if(curr_val == last_val){
            Index_Node_t* next = indexNodeCreate(data);
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
    *node_num = size;
}

void dataIndexArrayStrRead(FILE* index, Index_Node_t** array, int size, int* node_num, int* diff_node_num){

    int curr_pos = 0;
    char* last_val;
    char* curr_val;

    Index_Node_t* curr_elem = NULL;
    Index_Node_t* next_elem = NULL;
    Index_Data_t* data = indexDataReadStr(index);
    last_val = indexDataGetStrKey(data);

    indexNodeSetData(array, curr_pos, data);
    curr_pos++;
    curr_elem = array[0];

    for(int i = 1; i < size; i++){
        Index_Data_t* data = indexDataReadStr(index);
        curr_val = indexDataGetStrKey(data);
        
        if( stringnCmp(curr_val, last_val, STR_SIZE) == 0){
            Index_Node_t* next = indexNodeCreate(data);
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
    *node_num = size;
}

int dataGetIntField(Bin_Data_t* data, int param){
    if(param == 0) return dataGetId(data);
    return dataGetArticle(data);
}

char* dataGetStrField(Bin_Data_t* data, int param){

    char* str;
    switch (param){
    case 2:
        str = copyConstVarStr(dataGetDate(data));
        break;    

    case 3:
        str = copyConstVarStr(dataGetDescription(data));
        break;

    case 4:
        str = copyConstVarStr(dataGetPlace(data));
        break;

    case 5:
        str = copyConstVarStr(dataGetBrand(data));
        break;

    default:
        return NULL;
    }
    
    return str;
}

char dataParamCompare(Bin_Data_t* bin_data, Index_Data_t** parameter_array, int parameter_num){

    char equal = 1;
    for(int i = 0; i < parameter_num; i++){

        int param = indexDataGetParam(parameter_array[i]);
        if(param <= 1){
            int cmp = dataGetIntField(bin_data, param);
            if(cmp != indexDataGetIntKey(parameter_array[i])){
                return 0;
            }
        }
        else{
            char* str1 = dataGetStrField(bin_data, param);
            char* str2 = indexDataGetStrKey(parameter_array[i]);
    
            if(str1 == NULL) return 0;
            
            for(int j = 0; j < STR_SIZE; j++){
                if(str1[j] != str2[j]){
                    free(str1);
                    return 0;
                }
            }
            free(str1);
        }
    }

    return equal;
}

int binarySearchIndexInt(Index_Node_t** index, int beg, int end, int field_val){
    
    int mid = (beg+end)/2;

    int curr_val = indexDataGetIntKey(indexNodeGetData(index[mid]));
    if(beg >= end){
        
        if(curr_val != field_val) return -1;
        return mid;
    }

    if(curr_val > field_val){
        return binarySearchIndexInt(index, beg, mid-1, field_val);
    }
    else if(curr_val < field_val){
        return binarySearchIndexInt(index, mid+1, end, field_val);
    }
    else return mid;
    
}

int binarySearchIndexStr(Index_Node_t** index, int beg, int end, char* str){
    int mid = (beg+end)/2;

    char* curr_val = indexDataGetStrKey(indexNodeGetData(index[mid]));
    if(beg >= end){
        
        if(stringnCmp(curr_val, str, STR_SIZE) != 0 ) return -1;
        return mid;
    }

    if(stringnCmp(curr_val, str, STR_SIZE) > 0){
        return binarySearchIndexStr(index, beg, mid-1, str);
    }
    else if(stringnCmp(curr_val, str, STR_SIZE) < 0){
        return binarySearchIndexStr(index, mid+1, end, str);
    }
    else return mid;
}

int64_t* nodeListCompare(Index_Node_t* node, Index_Data_t** array, int64_t* offset_array,
    FILE* binary_file, int parameter_num, int print){
    
    
    while(node != NULL){
        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(binary_file, offset, SEEK_SET);
        Bin_Data_t* bin_data = dataBinaryRead(binary_file);
        if(dataParamCompare(bin_data, array, parameter_num)){
            offset_array = offsetArrayInsert(offset_array, offset);
            if(print){
                dataPrintCsvStyle(bin_data);
                printf("\n");
            }
        }
        dataDestroy(bin_data);
        node = indexNodeGetNext(node);
    }
    return offset_array;
}

int64_t* binarySearchIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index){
    
    Index_Header_t* header = indexHeaderRead(index_file);
    int size = indexHeaderGetNum(header);
    int node_num = 0;
    int diff_node_num = 0;
    int index_pos = 0;

    Index_Node_t** node_array = indexArrayCreate(size);
    int param = indexDataGetParam(array[parameter_index]);
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &node_num, &diff_node_num);
        int field_val = indexDataGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, diff_node_num, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &node_num, &diff_node_num);
        char* str = indexDataGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, diff_node_num, str);
    }

    if(index_pos == -1){
        
        indexArrayDestroy(node_array, size, diff_node_num);
        free(header);
        return NULL;
    }

    Index_Node_t* n = node_array[index_pos];
    int64_t* offset_array = offsetArrayCreate();

    offset_array = nodeListCompare(n, array, offset_array, binary_file, parameter_num, 1);

    indexArrayDestroy(node_array, size, diff_node_num);
    free(header);

    if(offset_array == NULL || offset_array[0] == -1){
        free(offset_array);
        return NULL;
    }
    return offset_array;
}

int64_t* linearSearchBinaryFile(FILE* file, Index_Data_t** array, int array_size, char print){
    
    int64_t offset = 0;
    Bin_Header_t* header;
    header = binHeaderRead(file);
    offset += 17;
    int struct_num = headerGetStructNum(header);

    int64_t* offset_array = offsetArrayCreate();

    for(int i = 0; i < struct_num; i++){
        Bin_Data_t* bin_data = dataBinaryRead(file);
        int var = 32+varStrSize(bin_data);
        offset += var;

        if(dataGetRemoved(bin_data) == '1'){
            dataDestroy(bin_data);
            continue;
        }

        if(dataParamCompare(bin_data, array, array_size)){
            offset_array = offsetArrayInsert(offset_array, offset);
            if(print){
                dataPrintCsvStyle(bin_data);
                printf("\n");
            }
        }

        dataDestroy(bin_data);
    }

    free(header);

    return offset_array;
}