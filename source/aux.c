#include <stdio.h>
#include <stdlib.h>
#include <aux.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

void errorFile() {
    printf("Falha no processamento do arquivo.\n");
    exit(0);
}

void errorReg() {
    printf("Registro inexistente.\n");
    exit(0);
}

char endFileAssert(int return_value, FILE* file) {
    if (return_value == 0) {
        if (feof(file)) {
            return 1;
        }
        errorFile();
    }

    return 0;
}

char endFileChecker(FILE* file) {
    char read = '\0';
    int checker;
    checker = fread(&read, sizeof(char), 1, file);
    endFileAssert(checker, file);

    while (read == '\n' || read == '\r') {
        checker = fread(&read, sizeof(char), 1, file);
        if (endFileAssert(checker, file)) {
            return 1;
        }
    }

    ungetc(read, file);
    return 0;
}

void consumeFirstLine(FILE* csv_file) {
    char consume;
    char checker;

    do {
        checker = fread(&consume, sizeof(char), 1, csv_file);
        if (endFileAssert(checker, csv_file)) {
            return;
        }

    } while (consume != '\n');
}

FILE* csvFileOpen(char* file_name) {
    FILE* csv_file = fopen(file_name, "r");
    if (csv_file == NULL) {
        errorFile();
    }

    consumeFirstLine(csv_file);
    return csv_file;
}

FILE* binaryFileOpenWrite(char* file_name) {
    FILE* binary_file = fopen(file_name, "wb");
    if (binary_file == NULL) {
        errorFile();
    }
    return binary_file;
}

FILE* binaryFileOpenRead(char* file_name) {
    FILE* binary_file = fopen(file_name, "rb");
    if (binary_file == NULL) {
        errorFile();
    }
    return binary_file;
}

//apagar
FILE* binaryFileOpenAppend(char* file_name) {
    FILE* binary_file = fopen(file_name, "a+b");
    if (binary_file == NULL) {
        errorFile();
    }
    return binary_file;
}

FILE* binaryFileOpenReadWrite(char* file_name){
    FILE* binary_file = fopen(file_name, "r+b");
    if (binary_file == NULL) {
        errorFile();
    }
    return binary_file;
}

int isConstStringNull(char* str) {
    if (str[0] == '$') {
        return 1;
    }

    return 0;
}

void printConstString(char* str, int strlen) {
    int i = 0;
    while (i < strlen && str[i] != '$') {
        printf("%c", str[i]);
        i++;
    }
}

void fileClose(FILE* file) {
    if (file != NULL) {
        fclose(file);
    }
}

int varStrTell(char* str){

    if(str == NULL){
        return 0;
    }
    int size = 0;

    while(str[size] != '\0' && str[size] != '|'){
        size++;
    }

    return size+1;
}

int varStrSize(Bin_Data_t* data){
    
    int total_size = varStrTell(dataGetPlace(data));
    total_size += varStrTell(dataGetDescription(data));
    
    return total_size;
}

int searchParameter(){
    char field_name[20];
    

    scanf("%s", field_name);

    switch (field_name[0]){ 
    case 'i':   //id
        return 0;

    case 'n':   //article_number
        return 1;
    
    case 'd':   //crime_description
        if(field_name[1] == 'a'){   //crime_date
            return 2;
        }
        return 3;
        
    case 'l':   //place
        return 4;

    case 'm':   //brand
        return 5;

    default:
        return -1;
    }

    return -1;
}

char* copyConstVarStr(char* str1){

    if(str1 == NULL || str1[0] == '|' || str1[0] == '$') return NULL;
    int i = 0;
    char* str2 = malloc(STR_SIZE*sizeof(char));

    while( i < STR_SIZE && str1[i] != '|' && str1[i] != '$' && str1[i] != '\0'){
        str2[i] = str1[i];
        i++;
    }
    while(i < STR_SIZE){
        str2[i] = '$';
        i++;
    }

    return str2;
}

char* readQuote12(){

    char* str = malloc(12*sizeof(char));
    int size = 0;
    char read = '\0';
    scanf("%c", &read);
    char dq = '"';
    while(read != dq){
        scanf("%c", &read);
    }

    scanf("%c", &read);
    
    while(size < 12 && read != dq){
        str[size] = read;
        size++;
        scanf("%c", &read);
    }
    if(size < 12){
        while(size < 12){
            str[size] = '$';
            size++;
        }
    }
    else{
        while(read != dq){
            scanf("%c", &read);
        }
    }

    return str;

}

int stringnCmp(char* str1, char* str2, int size){

    for(int i = 0; i < size; i++){
        
        int j = str1[i] - str2[i];
        if(j != 0) return j;
    }
    return 0;
}

int64_t* offsetArrayCreate(){
    
    int64_t* offset_array = calloc(10, sizeof(int64_t));
    offset_array[0] = -1;
    return offset_array;
}

int64_t*  offsetArrayInsert(int64_t* array, int64_t offset){
    
    int size = 0;
    while(array[size] != 0 && array[size] != -1){
        size++;
    }

    if(array[size] == -1){
        array[size] = 0;
        array = realloc(array, (2*(size+1))*sizeof(int64_t));
        for(int i = size; i < 2*size; i++){
            array[i] = 0;
        }

        if(size == 0) array[1] = -1;
        array[2*size] = -1;
    }
    
    array[size] = offset;
    //printf("passou\n");
    return array;
}

void binarioNaTela(char* nomeArquivoBinario) {
    /*
     * Você não precisa entender o código dessa função.
     *
     * Use essa função para comparação no run.codes.
     * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     * Ela vai abrir de novo para leitura e depois fechar
     * (você não vai perder pontos por isso se usar ela).
     */

    unsigned long i, cs;
    unsigned char* mb;
    size_t fl;
    FILE* fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                        "não foi possível abrir o arquivo que me passou para leitura. "
                        "Ele existe e você tá passando o nome certo? Você lembrou de "
                        "fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char*)malloc(fl);
    char checker = fread(mb, 1, fl, fs);
    if (checker == 0) {
        errorFile();
    }

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}