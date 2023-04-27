#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //para garantir o tamanho dos campos
#include <aux.h>
#include <data.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>

Data_t* dataCsvRead(FILE* csv_file, int* size_array) {
    Data_t* data = dataCreate();
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

Data_t* dataBinaryRead(FILE* binary_file) {
    Data_t* data = dataCreate();
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

int dataBinaryWrite(FILE* binary_file, Data_t* data, int* size_array) {
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

void dataPrintCsvStyle(Data_t* data) {
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
