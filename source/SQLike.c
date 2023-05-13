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
    int search_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }  

    scanf("%d", &search_num);
    
    //fazer uma função que é o que está aqui embaixo:
    char found = 0;
    void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data) = &ptrBinDataPrint;
    for(int i = 0; i < search_num; i++){
        
        printf("Resposta para a busca %d\n", i+1);
        found = SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);
        if(found == 0){ //
            printf("Registro inexistente.\n");
        }
    }
    
}

void SQLDeleteWhere(){

    char read_file_name[30];
    char index_file_name[30];
    int checker; 
    char consume[30];
    int index_parameter;
    int search_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }  

    scanf("%d", &search_num);
    
    //fazer uma função que é o que está aqui embaixo:
    int found = 0;
    void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data) = &ptrBinDataDelete;
    for(int i = 0; i < search_num; i++){
        found += SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);  
    }

    removedStructUpdate(read_file_name, found);

    int64_t offset = 0;
    binarioNaTela(read_file_name);
    fileIndexCreate(read_file_name, index_file_name, index_parameter, &offset);
}


// void SQLInsertInto(){
    
//     char read_file_name[30];
//     char index_file_name[30];
//     int checker; 
//     char consume[30];
//     int index_parameter;
//     int insert_num = 0;
    

//     checker = scanf("%s", read_file_name);
//     if (checker == 0) {
//         errorFile();
//     }

//     index_parameter = searchParameter();
//     scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

//     checker = scanf("%s", index_file_name);
//     if (checker == 0) {
//         errorFile();
//     }  

//     fileIndexRead(index_file_name, index_parameter);

//     scanf("%d", &insert_num);
    
//     //fazer uma função que é o que está aqui embaixo:
//     for(int i = 0; i < insert_num; i++){
//         insertIntoBinaryFile(read_file_name, index_file_name, index_parameter);
//     }
    
//     binarioNaTela(read_file_name);
//     binarioNaTela(index_file_name);
// }


void SQLUpdateSetWhere(){

    char read_file_name[30];
    char index_file_name[30];
    int checker; 
    char consume[30];
    int index_parameter;
    int search_num = 0;
    

    checker = scanf("%s", read_file_name);
    if (checker == 0) {
        errorFile();
    }

    index_parameter = searchParameter();
    scanf("%s", consume); //consome "string" ou "inteiro" (desnecessário)

    checker = scanf("%s", index_file_name);
    if (checker == 0) {
        errorFile();
    }

    //modularizar a construção do array de parametros para poder passar como argumento!!
    //modularizar a construção do array de parametros para poder passar como argumento!!
    
    scanf("%d", &search_num);
    printf("search num = %d\n", search_num);
    //fazer uma função que é o que está aqui embaixo:
    void (*fnt)(FILE* file, int64_t offset, Bin_Data_t* bin_data) = &ptrUpdateField;
    char found = 0;
    for(int i = 0; i < search_num; i++){
        
        printf("\nResposta para a update %d\n", i+1);
        found = SearchBinaryFile(read_file_name, index_file_name, index_parameter, fnt);
        if(!found){ //
            

            //precisamos consumir a linha debaixo caso a função não encontre nada.
            //deve ser tipo scanf("%["\n");
            int substitution_num = 0;
            scanf("%d", &substitution_num);
            printf("substituition num = %d\n", substitution_num);

            Parameter_Hold_t** param_array = parameterArrayCreate(substitution_num);

            int parameter = 0;
            for(int i = 0; i < substitution_num; i++){
                parameter = searchParameter();
                readFieldStdin(param_array[i], parameter, 0);
            }
            
            printf("O registro:");
            printf("inexistente");
            printf("\n");
            printf("deve ter os campos substituídos por: ");
            paramArrPrint(param_array, substitution_num);
            printf("\n");
        }
    }

    binarioNaTela(read_file_name);
}