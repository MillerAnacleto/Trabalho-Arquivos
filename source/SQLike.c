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
#include <SQLike.h>

void SQLCreateTable() {

    char csv_file_name[MAX_FILE_NAME];
    char binary_file_name[MAX_FILE_NAME];
    int checker;

    checker = scanf("%s", csv_file_name);
    if (checker == 0) {
        errorFile();
    }

    checker = scanf("%s", binary_file_name);
    if (checker == 0) {
        errorFile();
    }

    fileReadCsvWriteBinary(csv_file_name, binary_file_name);
}

void SQLSelectFrom() {
    char binary_file_name[MAX_FILE_NAME];
    int checker;

    checker = scanf("%s", binary_file_name);
    if (checker == 0) {
        errorFile();
    }

    fileBinaryPrintAsCsv(binary_file_name);
}

void SQLCreateIndex(){
    
    char read_file_name[MAX_FILE_NAME];
    char index_file_name[MAX_FILE_NAME];
    int checker; 
    char consume[10];

    checker= scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    int parameter = searchParameter();
    scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

    checker= scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }
    int64_t offset = 0;
    fileIndexCreate(read_file_name, index_file_name, parameter, &offset);
}

void SQLSelectWhere(){

    char read_file_name[MAX_FILE_NAME];
    char index_file_name[MAX_FILE_NAME];
    int checker; 
    char consume[MAX_FILE_NAME];
    int parameter_num = 0;
    int index_parameter = 0;
    int search_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); //repete a lógica do anterior e seguirá repetindo

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }  

    scanf("%d", &search_num);
    
    char found = 0;
    Parameter_Hold** (*fnt)(fntptr) = &ptrBinDataPrint;
    for(int i = 0; i < search_num; i++){
        
        printf("Resposta para a busca %d\n", i+1);
        found = SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);
        if(found == 0){
            printf("Registro inexistente.\n");
        }
    }
}

void SQLDeleteWhere(){

    char read_file_name[MAX_FILE_NAME];
    char index_file_name[MAX_FILE_NAME];
    int checker; 
    char consume[MAX_FILE_NAME];
    int index_parameter;
    int search_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); 

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }  

    scanf("%d", &search_num);
    
    int found = 0;
    Parameter_Hold** (*fnt)(fntptr) = &ptrBinDataDelete;
    for(int i = 0; i < search_num; i++){
        found += SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);  
    }

    int64_t offset = 0;
    binarioNaTela(read_file_name);
    fileIndexCreate(read_file_name, index_file_name, index_parameter, &offset);
}

void SQLInsertInto(){
    
    char read_file_name[MAX_FILE_NAME];
    char index_file_name[MAX_FILE_NAME];
    int checker; 
    char consume[MAX_FILE_NAME];
    int index_parameter;
    int insert_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume);

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }

    scanf("%d", &insert_num);
    
    for(int i = 0; i < insert_num; i++){
        insertIntoBinaryFile(read_file_name, index_file_name, index_parameter);
    }
    
    int64_t offset = 0;
    binarioNaTela(read_file_name);
    binarioNaTela(index_file_name);
}


void SQLUpdateSetWhere(){

    char read_file_name[MAX_FILE_NAME];
    char index_file_name[MAX_FILE_NAME];
    int checker; 
    char consume[MAX_FILE_NAME];
    int index_parameter;
    int search_num = 0;
    char any_found = 0;

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume);

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }
    
    scanf("%d", &search_num);
    Parameter_Hold** (*fnt)(fntptr) = &ptrUpdateField;
    char found = 0;
    for(int i = 0; i < search_num; i++){
        
        found = SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);
        if(!found){ 
            //precisamos remover a linha debaixo (que só é lida caso pelo menos uma ocorrência seja encontrada).
            char c;
            char str[1000];
            scanf("%c", &c);
            scanf("%[^\n]s", str);
        }
        else any_found = 1;
    }

    binarioNaTela(read_file_name);
    int64_t offset = 0;
 
    if(any_found)
        fileIndexCreate(read_file_name, index_file_name, index_parameter, &offset);
    else    
        binarioNaTela(index_file_name);
}