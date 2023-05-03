#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <data.h>
#include <files.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

void SQLCreateTable() {

    char csv_file_name[30];
    char binary_file_name[30];
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
    char binary_file_name[30];
    int checker;

    checker = scanf("%s", binary_file_name);
    if (checker == 0) {
        errorFile();
    }

    fileBinaryPrintAsCsv(binary_file_name);
}

void SQLCreateIndex(){
    
    char read_file_name[30];
    char index_file_name[30];
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

    char read_file_name[30];
    char index_file_name[30];
    int checker; 
    char consume[30];
    int index_parameter;
    //char binary_flag = 0; //flag para busca binária
    int search_num = 0;
    

    checker= scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

    checker= scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }  

    fileIndexRead(index_file_name, index_parameter);

    scanf("%d", &search_num);
    
    //fazer uma função que é o que está aqui embaixo:
    char found = 0;
    for(int i = 0; i < search_num; i++){
        
        printf("Resposta para a busca %d\n", i+1);
        found = SearchBinaryFile(read_file_name, index_file_name, index_parameter);
        if(!found){
            printf("Registro inexistente.\n");
        }
    }
    
}