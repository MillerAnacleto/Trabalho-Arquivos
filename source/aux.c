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

    return binary_file;
}

FILE* binaryFileOpenRead(char* file_name) {
    FILE* binary_file = fopen(file_name, "rb");
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