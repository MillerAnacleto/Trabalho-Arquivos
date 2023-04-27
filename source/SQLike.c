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
