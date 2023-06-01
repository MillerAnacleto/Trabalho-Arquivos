#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>


Data_Register* dataCsvRead(FILE* csv_file, int* size_array) {
    Data_Register* data = dataCreate();
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

Data_Register* dataRead(int* size_array) {
    Data_Register* data = dataCreate();
    int str_size = 0;

    dataSetId(data, readInt());
    
    char* str = readConstString(DATE_SIZE);
    dataSetDate(data, str);
    free(str);

    dataSetArticle(data, readInt());

    dataSetPlace(data, readVarString(&str_size));
    size_array[0] = str_size;
    
    dataSetDescription(data, readVarString(&str_size));
    size_array[1] = str_size;
    
    str = readConstString(BRAND_SIZE);
    dataSetBrand(data, str);
    free(str);
    
    return data;
}

Data_Register* dataBinaryRead(FILE* data_file) {
    
    Data_Register* data = dataCreate();
    char checker;

    char rmvd;
    checker = fread(&rmvd, sizeof(char), 1, data_file);
    dataSetRemoved(data, rmvd);
    endFileAssert(checker, data_file);

    int id;
    checker = fread(&id, sizeof(int32_t), 1, data_file);
    dataSetId(data, id);
    endFileAssert(checker, data_file);

    char crime_date[DATE_SIZE];
    checker = fread(crime_date, (DATE_SIZE) * sizeof(char), 1, data_file);
    dataSetDate(data, crime_date);
    endFileAssert(checker, data_file);

    int article;
    checker = fread(&article, sizeof(int32_t), 1, data_file);
    dataSetArticle(data, article);
    endFileAssert(checker, data_file);

    char brand[BRAND_SIZE];
    checker = fread(brand, (BRAND_SIZE) * sizeof(char), 1, data_file);
    dataSetBrand(data, brand);
    endFileAssert(checker, data_file);

    char* crime_place;
    crime_place = readBinaryVarString(data_file);
    dataSetPlace(data, crime_place);
    endFileAssert(checker, data_file);

    char* description;
    description = readBinaryVarString(data_file);
    dataSetDescription(data, description);
    endFileAssert(checker, data_file);

    char delimiter;
    checker = fread(&delimiter, sizeof(char), 1, data_file);
    
    int extra_size = 0;
    while(delimiter != '#' && !endFileAssert(checker, data_file)){
        checker = fread(&delimiter, sizeof(char), 1, data_file);
        extra_size++;
    }

    dataSetDelimiter(data, delimiter);
    dataSetExtraSize(data, extra_size);

    return data;
}

int dataBinaryWrite(FILE* data_file, Data_Register* data, int* size_array, int64_t offset) {
    if(offset != -1)
        fseek(data_file, offset, SEEK_SET);

    int acc = 0; // acumulador para verificação da escrita
    
    char rmvd = dataGetRemoved(data);
    acc += fwrite(&rmvd, sizeof(char), 1, data_file);
    
    int crime_id = dataGetId(data);
    acc += fwrite(&crime_id, sizeof(int32_t), 1, data_file);

    char* date = dataGetDate(data);
    acc += fwrite(date, sizeof(char), DATE_SIZE, data_file);

    int article = dataGetArticle(data);
    acc += fwrite(&article, sizeof(int32_t), 1, data_file);

    char* brand = dataGetBrand(data);
    acc += fwrite(brand, sizeof(char), BRAND_SIZE, data_file);

    char* crime_place = dataGetPlace(data);
    acc += fwrite(crime_place, sizeof(char), size_array[0], data_file);

    char* crime_description = dataGetDescription(data);
    acc += fwrite(crime_description, sizeof(char), size_array[1], data_file);

    char delimiter = dataGetDelimiter(data);
    acc += fwrite(&delimiter, sizeof(char), 1, data_file);

    return acc;
}

char* readBinaryVarString(FILE* data_file) {
    char* str = malloc(sizeof(char));
    char last_read = '\0';
    int str_size = 0;

    char checker = fread(&last_read, sizeof(char), 1, data_file);
    endFileAssert(checker, data_file);
    if (last_read == '|') {
        str[str_size] = last_read;
        str_size++;
        str = realloc(str, (str_size + 1) * sizeof(char));
    }

    while (last_read != '|') {
        str[str_size] = last_read;
        str_size++;
        checker = fread(&last_read, sizeof(char), 1, data_file);
        endFileAssert(checker, data_file);

        str = realloc(str, (str_size + 1) * sizeof(char));
    }
    str[str_size] = '\0';

    return str;
}

void dataPrintCsvStyle(Data_Register* data) {
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
        int i = 0;
        while(aux_str[i] != '\0' && aux_str[i] != '|'){
            printf("%c", aux_str[i]);
            i++;
        }
        printf(", ");
    } else {
        printf("NULO, ");
    }

    aux_str = dataGetDescription(data);
    if (aux_str[0] != '|') {
        int i = 0;
        while(aux_str[i] != '\0' && aux_str[i] != '|'){
            printf("%c", aux_str[i]);
            i++;
        }
        printf(", ");
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



int dataGetIntField(Data_Register* data, int param){
    if(param == 0) return dataGetId(data);
    return dataGetArticle(data);
}

char* dataGetStrField(Data_Register* data, int param){

    char* str;
    switch (param){
    case 2:
        str = copyConstVarStr(dataGetDate(data));
        break;    

    case 3:
        str = copyVarStr(dataGetDescription(data));
        break;

    case 4:
        str = copyVarStr(dataGetPlace(data));
        break;

    case 5:
        str = copyConstVarStr(dataGetBrand(data));
        break;

    default:
        return NULL;
    }
    
    return str;
}

char* parameterAddDelimiter(char* str, int size){
    
    char* str2 = malloc((1+size)*sizeof(char));
    for(int i = 0; i < size; i++){
        str2[i] = str[i];
    }
    str2[size] = '|';
    return str2;
}

void dataSetIntField(Data_Register* data, int field, int param){
    
    if(param == 0) 
        dataSetId(data, field);
    else 
        dataSetArticle(data, field);
}

void dataSetStrField(Data_Register* data, char* str, int param){

    char* str1 = NULL;
    switch (param){
    case 2:
        (dataSetDate(data, str));
        break;    

    case 3:
        str1 = dataGetDescription(data);
        free(str1);
        str1 = copyVarStr(str);
        (dataSetDescription(data, str1));
        break;

    case 4:
        str1 = dataGetPlace(data);
        free(str1);
        str1 = copyVarStr(str);
        (dataSetPlace(data, str1));
        break;

    case 5:
        (dataSetBrand(data, str));
        break;

    default:
        return;
    }
}

bool dataParamCompare(Data_Register* bin_data, Parameter_Hold** parameter_array, int parameter_num){

    if(dataGetRemoved(bin_data) == '1') return 0; //pode ocorrer caso a deleção
    //ocorra duas duas vezes num mesmo registro com parâmetros presente em duas
    //buscas, e que ainda esteja presente no índice.
    
    for(int i = 0; i < parameter_num; i++){

        int param = paramHoldGetVal(parameter_array[i]);
        if(param <= 1){
            int cmp = dataGetIntField(bin_data, param);
            if(cmp != paramHoldGetIntKey(parameter_array[i])){
                return FALSE;
            }
        }
        else{

            char* str1 = dataGetStrField(bin_data, param);
            char* str2 = paramHoldGetStrKey(parameter_array[i]);

            if(str1 == NULL || str2 == NULL) return 0;

            int tam = paramHoldGetIntKey(parameter_array[i]);
            
            if(param == 2 || param == 5){
                for(int j = 0; j < STR_SIZE; j++){
                    if(str1[j] != str2[j]){
                        free(str1);
                        return FALSE;
                    }
                }
            } else{
                int j = 0;

                while(str1[j] != '|' && str2[j] != '|' && str2[j] != '\0' && str1[j] != '\0'
                 && str2[j] != '$' && str1[j] != '$'){
                    if(str1[j] == str2[j]){
                        j++;

                    }
                    else{
                        free(str1);
                        return FALSE;
                    } 
                }  
            }
            free(str1);
        }
    }

    return TRUE;
}

int binarySearchIndexInt(Index_Node** index, int beg, int end, int field_val){
    
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

int binarySearchIndexStr(Index_Node** index, int beg, int end, char* str){
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

int nodeListCompare(Index_Node* node, Parameter_Hold** array,
    FILE* data_file, int parameter_num, Parameter_Hold** (*fnt) (fntptr)){
    
    Data_Header* bin_header = dataHeaderRead(data_file);
    
    //reescrevemos o header com status '0'
    headerSetStatus(bin_header, '0');
    dataHeaderWrite(data_file, bin_header, 0);

    Parameter_Hold** substitution_array = NULL;
    int param_num = 0;
    int found = 0;
    while(node != NULL){

        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(data_file, offset, SEEK_SET);
        Data_Register* bin_data = dataBinaryRead(data_file);
        
        if(dataParamCompare(bin_data, array, parameter_num)){
            found++;
            substitution_array = (*fnt)(data_file, offset, bin_data, bin_header, substitution_array, &param_num);
        }
        node = indexNodeGetNext(node);
        dataDestroy(bin_data);
    }
    
    if(substitution_array != NULL)
        //free(substitution_array);
        parameterArrayDestroy(substitution_array, param_num);

    //reescrevemos o header com status '1'
    headerSetStatus(bin_header, '1');
    dataHeaderWrite(data_file, bin_header, 0);
    free(bin_header);

    return found;
}

int binarySearchIndexArray(FILE* index_file, FILE* data_file, Parameter_Hold** array,  
    int parameter_num, int parameter_index, Parameter_Hold** (*fnt)(fntptr)){
    
    int found = 0;
    Index_Header* header = indexHeaderRead(index_file);

    //reescrevemos o header com status '0'
    indexHeaderSetStatus(header, '0');
    indexHeaderWrite(index_file, header, 0);

    int size = indexHeaderGetNum(header);

    if(size == 0){
        
        indexHeaderSetStatus(header, '1');
        indexHeaderWrite(index_file, header, 0);
        
        free(header);

        return 0;
    }

    int node_num = 0;
    int diff_node_num = 0;
    int index_pos = 0;

    Index_Node** node_array = indexArrayCreate(size);
    int param = paramHoldGetVal(array[parameter_index]);
    
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &diff_node_num);
        int field_val = paramHoldGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, diff_node_num, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &diff_node_num);
        char* str = paramHoldGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, diff_node_num, str);
    }

    if(index_pos == -1){
        found = 0;
    }
    else{
        Index_Node* n = node_array[index_pos];

        found = nodeListCompare(n, array, data_file, parameter_num, fnt);
    }

    indexArrayDestroy(node_array, size, diff_node_num);
    
    //reescrevemos o header do arq de indice com status '1'
    indexHeaderSetStatus(header, '1');
    indexHeaderWrite(index_file, header, 0);
    
    free(header);
    

    return found;
}

int linearSearchBinaryFile(FILE* file, Parameter_Hold** array, int array_size,
     Parameter_Hold** (*fnt)(fntptr)){
    
    int found = 0;
    int64_t offset = 0;
    Data_Header* header;
    header = dataHeaderRead(file);

    //reesecrevemos o header com status '0'
    headerSetStatus(header, '0');
    dataHeaderWrite(file, header, 0);

    offset += BIN_HEADER_SIZE;
    int struct_num = headerGetStructNum(header);

    Parameter_Hold** substitution_array = NULL;
    int substitution_num = 0;

    for(int i = 0; i < struct_num; i++){
            
        Data_Register* bin_data = dataBinaryRead(file);
        int var = dataGetSize(bin_data);
        offset += var;

        if(dataGetRemoved(bin_data) == '1'){
            dataDestroy(bin_data);
            continue;
        }

        if(dataParamCompare(bin_data, array, array_size)){
            found++;
            substitution_array = (*fnt)(file, offset, bin_data, header, substitution_array, &substitution_num);
        }

        //if(substitution_array == NULL)
            dataDestroy(bin_data);
        
    }
    
    if(substitution_array != NULL)
        //free(substitution_array);
        parameterArrayDestroy(substitution_array, substitution_num);

    //reescrevemos o header com status '1'
    headerSetStatus(header, '1');
    dataHeaderWrite(file, header, 0);

    free(header);

    return found;
}

Parameter_Hold** ptrBinDataPrint(FILE* data_file, int64_t offset, 
    Data_Register* bin_data, Data_Header* bin_header, 
    Parameter_Hold** param_array, int* param_num){

    dataPrintCsvStyle(bin_data);
    printf("\n");

    return NULL;
}

Parameter_Hold** ptrBinDataDelete(FILE* data_file, int64_t offset, 
    Data_Register* bin_data, Data_Header* bin_header, 
    Parameter_Hold** param_array, int* param_num){

    int size = dataGetSize(bin_data);
    fseek(data_file, -size, SEEK_CUR);
    char rmvd = '1';
    fwrite(&rmvd, sizeof(char), 1, data_file);
    fseek(data_file, size-1, SEEK_CUR);

    int rem_num = headerGetRemStructNum(bin_header);
    rem_num++;
    headerSetRemStructNum(bin_header, rem_num);

    return NULL;
}

void binDataPadding(Data_Register* bin_data, int* size_arr){
    
    size_arr[0] = varStrTell(dataGetPlace(bin_data));
    size_arr[1] = varStrTell(dataGetDescription(bin_data)); 
    
    char* str1 = dataGetDescription(bin_data);
    char* str2 = dataGetPlace(bin_data);
    
    if(size_arr[1] != 0) str1[size_arr[1]-1] = '|';
    if(size_arr[0] != 0) str2[size_arr[0]-1] = '|';

    dataSetDescription(bin_data, str1);
    dataSetPlace(bin_data, str2);

}


Parameter_Hold** ptrUpdateField(FILE* data_file, int64_t offset, 
    Data_Register* bin_data, Data_Header* bin_header, 
    Parameter_Hold** substitution_array, int* param_num){

    Data_Register* old_bin_data = dataCreate();
    
    //copiamos para não haver cruzamentos na referência e podermos liberar a memória 
    char* strd = copyVarStr(dataGetDescription(bin_data));
    char* strp = copyVarStr(dataGetPlace(bin_data));

    dataSetDescription(old_bin_data, strd);
    dataSetPlace(old_bin_data, strp);

    if(substitution_array == NULL){
        int substitution_num = 0;
        scanf("%d", &substitution_num);
        
        substitution_array = parameterArrayCreate(substitution_num);
        
        int parameter = 0;
        for(int i = 0; i < substitution_num; i++){
            parameter = searchParameter();
            readFieldBinStdin(substitution_array[i], parameter);
        }
        *param_num = substitution_num;
    }
    
    int curr_data_size = dataGetSize(bin_data);

    for(int i = 0; i < *param_num; i++){
        int parameter = paramHoldGetVal(substitution_array[i]);
        if(parameter <= 1){
            dataSetIntField(bin_data, paramHoldGetIntKey(substitution_array[i]), parameter);
        }
        else{
            dataSetStrField(bin_data, paramHoldGetStrKey(substitution_array[i]), parameter);
        }
    }
    
    //tratamento dos campos da struct de parametro, que foram lidos do stdin, para escrita no binário:
    
    int size_arr[2];
    binDataPadding(bin_data, size_arr);  
    
    int insert_data_size = dataGetSize(bin_data);

    if(curr_data_size < insert_data_size){
        
        //marcar como deletado
        ptrBinDataDelete(data_file, offset, old_bin_data, bin_header, NULL, NULL);
        int64_t end_file_offset = headerGetOffset(bin_header);

        // escreve novo data
        dataBinaryWrite(data_file, bin_data, size_arr, end_file_offset);

        // atualiza bin header
        headerSetStructNum(bin_header, headerGetStructNum(bin_header) + 1);
        
        //atualizar offset do arq de dados (+tam reg_inserido)
        headerSetOffset(bin_header, end_file_offset + dataGetSize(bin_data));

        fseek(data_file, offset+curr_data_size, SEEK_SET);

    }else{
        //apenas reesecrever
        fseek(data_file, -curr_data_size, SEEK_CUR);
        dataBinaryWrite(data_file, bin_data, size_arr, -1);
        
        if(insert_data_size < curr_data_size){
            fseek(data_file, -1, SEEK_CUR);
            while(insert_data_size < curr_data_size){
                char dol = '$';
               
                fwrite(&dol, sizeof(char), 1, data_file);
                insert_data_size++;
            }
            char delimiter = '#';
            fwrite(&delimiter, sizeof(char), 1, data_file);
        }
       
    }

    dataDestroy(old_bin_data);
    return substitution_array;
}

Parameter_Hold** parameterArrayRead(int parameter_num, int index_parameter, char* binary_flag,
    int* parameter_index){
    
    Parameter_Hold** array = parameterArrayCreate(parameter_num);

    for(int j = 0; j < parameter_num; j++){
        
        int parameter = searchParameter();
        if(parameter == index_parameter){
            *binary_flag = 1;
            *parameter_index = j;
        }
        readFieldStdin(array[j], parameter, STR_SIZE);
    }

    return array;
}