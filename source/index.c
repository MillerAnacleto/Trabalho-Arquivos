#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <input_output.h>
#include <index.h>
#include <structs.h>

#define STR_SIZE 12

struct index_header_{
    int status;
};

struct int_data_ {
    int key;
    int64_t offset;
};

struct str_data_ {
    char *key;
    int64_t offset;
};

Index_Bin_Header_t* indexHeaderCreate(){
    Index_Bin_Header_t* header_indx = malloc(sizeof(Index_Bin_Header_t));
    header_indx->status = 1;
    return header_indx;
}

Index_Int_t* indexIntDataCreate(){
    Index_Int_t* header_int = malloc(sizeof(Index_Int_t));

    return header_int;
}

Index_String_t* indexStringDataCreate(){
    Index_String_t* header_String = malloc(sizeof(Index_String_t));
    header_String->key = malloc(STR_SIZE*sizeof(char));

    return header_String;
}

int searchParameter(){
    char field_name[20];
    char field_type[10];

    scanf("%s", field_name);
    scanf("%s", field_type);

    if(field_type[0] == 's'){
        
        switch (field_name[0]){
        case 'd':
            if(field_name[1] == 'a'){
                return 1;
            }
            return 5;

        case 'm':
            return 3;
            
        case 'l':
            return 4;
        
        default:
            return -1;
        }
    }
    else{
        if(field_name[0] == 'i') return 0;
        if(field_name[0] == 'n') return 2;
    }
    
    return -1;
}

// void readFileLimiter(FILE* file, int pipe_num, int64_t* offset){

//     int pipes_read = 0;
//     char read = '\0';

//     while(read != '#' && pipes_read != pipe_num){

//         fread(&read, sizeof(char), 1, file);
//         if(read == '|') pipes_read++;
//         *offset += 1;
//     }
// }

// void positionRead(FILE* file, int parameter, int64_t *offset){
//     char consume;
    
//     switch (parameter){
//     case 0:
//         fread(&consume, sizeof(char), 1, file);
//         *offset += 1;
//         return;

//     case 1:
//         for(int i = 0; i < 5; i++){
//             fread(&consume, sizeof(char), 1, file);
//             *offset += 1;
//         }
//         return;

//     case 2:
//         for(int i = 0; i < 15; i++){
//             fread(&consume, sizeof(char), 1, file);
//             *offset += 1;
//         }
//         return;

//     case 3:
//         for(int i = 0; i < 19; i++){
//             fread(&consume, sizeof(char), 1, file);
//             *offset += 1;
//         }
//         return;

//     case 4:
//         for(int i = 0; i < 31; i++){
//             fread(&consume, sizeof(char), 1, file);
//             *offset += 1;
//         }
//         return;
    
//     case 5:
//         readFileLimiter(file, 1, offset);
//         return;

//     default:
//         errorFile();
//         break;
//     }
// }

// char readBinaryConstString(FILE* file, int64_t* offset, char* key, int fixup){

//     char read = '\0';
//     int size = 0;

//     while(size < STR_SIZE-fixup && read != '|' && read != '$'){

//         fread(&read, sizeof(char), 1, file);
//         *offset += 1;
//         if(read != '|' && read != '$'){
//             key[size] = read;
//             size++;
//         }
//         else if(size == 0){
//             return 0;
//         }
//     }

//     printf("size = %d\n", size);
//     while(size < STR_SIZE){
//         key[size] = '$';
//         size++;
//     }
//     return 1;
// }

// int readBinaryInt(FILE* file, int64_t* offset){

//     int read;
   
//     fread(&read, sizeof(int), 1, file);
//     printf("Read = %d\n", read);

//     *offset += 4;
//     return read; //if there is no number, read = -1
// }

// int readBinaryField(FILE* file, int parameter, int64_t *offset, char* key){

//     int fixup = 0;
//     int ret;
//     positionRead(file, parameter, offset);//positions the file pointer immeadiately before the desired field 

//     if(parameter == 0 || parameter == 2){
//         ret = readBinaryInt(file, offset);
//     }

//     else{

//         if(parameter == 1) fixup = 2; //caso seja data serão impressos 10 caracteres        
//         if(readBinaryConstString(file, offset, key, fixup)){
//             ret = 1;
//         }
//         else{
//             ret = -1;
//         }
//     }//reads a string to the struct
    
//     readFileLimiter(file, -5, offset); //reads until end of line
    
//     return ret;
// }

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

int varStrSize(Data_t* data){
    
    int total_size = varStrTell(dataGetPlace(data));
    total_size += varStrTell(dataGetDescription(data));
    
    return total_size;
}

char* readBinaryField(FILE* file, int parameter, int64_t *offset, int* key){

    Data_t* data = dataBinaryRead(file);
    printf("ID = %d\n", dataGetId(data));
    *offset += 32 + varStrSize(data);

    switch (parameter){
    case 0:
        *key = dataGetId(data);
        return NULL;
        
    case 1:
        return dataGetDate(data);
        
    case 2:
        *key = dataGetArticle(data);
        return NULL;
        
    case 3:
        return dataGetBrand(data);

    case 4:
        return dataGetPlace(data);
    
    default:
        break;
    }
    return NULL;
}

void printField(char* str, int64_t offset, int key, int parameter){

    if(parameter == 0 || parameter == 2){
        printf("%d\n", key);
    }

    else{
        if(str[0] == '|' || str[0] == '$'){
            printf("NAO CONSTA\n");
        }

        else{
            int i = 0;
            while(str[i] != '|' && str[i] != '\0' && i < STR_SIZE){
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


void createStringIndex(FILE* file, int parameter, int64_t *offset){

    Bin_Header_t* header = binHeaderBinaryRead(file);
    int size = headerGetStructNum(header);
    *offset += 17;
    printf("\n offset = %ld", *offset);

    Index_Bin_Header_t* index_header = indexHeaderCreate();
    index_header->status = '0';

    Index_String_t** idx_str_vec = malloc(size*sizeof(Index_String_t*));
    
    for(int i = 0; i < size; i++){
        idx_str_vec[i] = indexStringDataCreate();
        
        int64_t offset_temp = *offset;
        idx_str_vec[i]->offset = offset_temp;

        int check;
        idx_str_vec[i]->key = readBinaryField(file, parameter, offset, &check); 
        
        if(check != -1){
            printField(idx_str_vec[i]->key, *offset, check, parameter);
        }
        else printf("\nNAO CONSTA\n");
    }

    int64_t checouSE = headerGetOffset(header);
    printf("\noffset = %ld, headeroffset = %ld", *offset, checouSE);
    
    //data_array_sort;
    //data_array_binary_print;
    
    index_header->status = '0';

    //free array;
}

//data_array_sort(sort by paramater, the use byte_offset to untie){}

//data_array_binary_print(copy-paste the print using fwrite){}

//free_array(free each allocated field (place and description) the free the elem, then the array){}

void readBinaryWriteIndex(FILE* file){
    
    int parameter = searchParameter();
    printf("parameter = %d\n", parameter);
    int64_t offset = 0;

    createStringIndex(file, parameter, &offset);

}

void indexHeaderDestroy(Index_Bin_Header_t* index_header);
void intDataDestroy(Index_Int_t* int_data);
void stringDataDestroy(Index_String_t* string_data);