#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

#define BRAND_SIZE 12
#define DATE_SIZE 10

// char * ?
char* readCsvConstString(FILE* csv_file, int strlen) {
    char* str = malloc(sizeof(char) * strlen);
    char last_read = '\0';
    int str_size = -1; // value initialized at -1 to simplify the loop

    int checker = fread(&last_read, sizeof(char), 1, csv_file); // if the field is empty the while-loop is skipped
    endFileAssert(checker, csv_file);                           // faliure in string read

    while (last_read != ',' && last_read != '\n' && last_read != ' ') {
        str_size++;
        str[str_size] = last_read;
        checker = fread(&last_read, sizeof(char), 1, csv_file);
        endFileAssert(checker, csv_file); // in case there is a faliure in reading the
        // error mesage is displayed
    }

    for (int i = str_size + 1; i < strlen; i++) {
        str[i] = '$';
    }
    return str;
}

char* readConstString(int strlen) {
    char* str = malloc(sizeof(char) * strlen);
    char last_read = '\0';
    int str_size = 0; // value initialized at -1 to simplify the loop

    scanf("%c", &last_read); // if the field is empty the while-loop is skipped
    while(last_read == ' ' || last_read == '\n'){
        scanf("%c", &last_read);
    }

    if(last_read == '"'){
        while (last_read != '"' || str_size == 0) {
            if(last_read != '"'){
                str[str_size] = last_read;
                str_size++;
            }
            scanf("%c", &last_read);
        }
    }
    else{
        while (last_read != 'O') {
            scanf("%c", &last_read);
        }
    }
    for (int i = str_size; i < strlen; i++) {
        str[i] = '$';
    }

    return str;
}

char* readCsvVarString(FILE* csv_file, int* strlen) {
    char* str = malloc(sizeof(char));
    char last_read = '\0';
    int str_size = 0; // unlike the other loop, the final value of the string
    // has size 1 + original string, because the '|' character must be added, therefore it must
    // be initialized in 0

    char checker = fread(&last_read, sizeof(char), 1, csv_file); // if the field is empty the while-loop is skipped
    endFileAssert(checker, csv_file);

    while (last_read != ',' && last_read != '\n') {
        str[str_size] = last_read;
        str_size++;
        checker = fread(&last_read, sizeof(char), 1, csv_file);
        endFileAssert(checker, csv_file);

        str = realloc(str, (str_size + 1)*sizeof(char));
    }
    str[str_size] = '|';
    *strlen = (str_size + 1);

    return str;
}

char* readVarString(int* strlen) {
    char* str = malloc(sizeof(char));
    char last_read = '\0';
    int str_size = 0; // unlike the other loop, the final value of the string
    // has size 1 + original string, because the '|' character must be added, therefore it must
    // be initialized in 0

    scanf("%c", &last_read); // if the field is empty the while-loop is skipped
    while(last_read == ' ' || last_read == '\n'){
        scanf("%c", &last_read);
    }

    if(last_read == '"'){
        while (last_read != '"' || str_size == 0) {
            if(last_read != '"'){
                str[str_size] = last_read;
                str_size++;
            }
            scanf("%c", &last_read);

            str = realloc(str, (str_size + 1)*sizeof(char));
            if(str_size > 50)
                exit(0);

            // (last != ' ' && last != '\n') || str[str_size] != '"'
        }
        str[str_size] = '\0';
    }
    else if(last_read == 'N'){
        while (last_read != ' ' && last_read != '\n') {
            scanf("%c", &last_read);
        }

        
    }

    str[str_size] = '|';
    *strlen = (str_size + 1);

    return str;
}

int readCsvInt(FILE* csv_file) {
    // the reading occurs byte by byte, to avoid problems with empty fields
    int integer = 0;
    char last_read[11]; // 10 digits is the maximum number a 32 bit integer can
    // have,another byte is added for the comma.

    int str_size = 0;

    char checker = fread(&last_read[0], sizeof(char), 1, csv_file);
    endFileAssert(checker, csv_file); //failure displays the error message

    while (last_read[str_size] != ',') {
        str_size++;
        checker = fread(&last_read[str_size], sizeof(char), 1, csv_file);
        endFileAssert(checker, csv_file);
    }
    if (str_size == 0) {
        integer = -1;
    } else {
        integer = atoi(last_read);
    }

    return integer;
}

int readInt() {
    // the reading occurs byte by byte, to avoid problems with empty fields
    int integer = 0;
    char last_read[11]; // 10 digits is the maximum number a 32 bit integer can
    // have,another byte is added for the comma.

    int str_size = 0;

    scanf("%c", &last_read[0]);

    while(last_read[0] == ' ' || last_read[0] == '\n'){
        scanf("%c", &last_read[0]);
    }

    while (last_read[str_size] != ' ' && last_read[str_size] != '\n') {
        str_size++;
        scanf("%c", &last_read[str_size]);
    }
    
    if (last_read[0] > '9') {
        integer = -1;
    } else {
        integer = atoi(last_read);
    }

    return integer;
}

Index_Data_t* readBinaryField(FILE* file, int parameter, int64_t *offset, char* exists){

    Index_Data_t* idx_data = indexDataCreate();
    Bin_Data_t* data = dataBinaryRead(file);
    *offset += 32 + varStrSize(data);
    int key = EMPTY_INT_FIELD;
    char* str = NULL;

    if(parameter <= 1){
        key = dataGetIntField(data, parameter);
    }
    else{
        str = dataGetStrField(data, parameter);
    }

    indexDataSetIntKey(idx_data, key);
    indexDataSetStrKey(idx_data, str);
    
    // modificar
    if((str != NULL || (key != EMPTY_INT_FIELD )) && !dataIsRemoved(data)) *exists = 1;
    
    dataDestroy(data);
    return idx_data;
}


void printField(char* str, int64_t offset, int key, int parameter){

    if(parameter > 1 && str == NULL){
        printf("Não consta\n");
        return;
    }

    if(parameter <= 1){
        printf("%d\n", key);
    }
    else{
        if(str[0] == '|' || str[0] == '$'){
            printf("Não consta\n");
        }

        else{
            int i = 0;
            while( i < STR_SIZE && str[i] != '|' && str[i] != '\0'){
                printf("%c", str[i]);
                i++;
            }
            while(i < STR_SIZE){
                printf("$");
                i++;
            }
            printf("\n");
        }
    }
    printf("\noffset = %ld\n", offset);
}

void readFieldStdin(Index_Data_t* array_elem, int parameter){
    if(parameter <= 1){
        int key;
        scanf("%d", &key);
        indexDataSetIntKey(array_elem, key);
    }
    else{
        char* str = readQuote12();
        indexDataSetStrKey(array_elem, str);
    }

    // int64_t cast = (int64_t) parameter; 
    // indexDataSetOffset(array_elem, cast);
    indexDataSetParam(array_elem, parameter);
}
