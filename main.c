/**
 * @file main.c
 * @author Guilherme da Motta Tranche (13671549) && Téo Sobrino Alves (12557192)
 * 
 * @brief Trabalho introdutório da disciplina de organização
 * de arquivos 1S 2023
 * 
 * O trabalho foi dividido igualmente entre os integrantes, cada um 
 * desenvolveu uma funcionalidade e suas funções auxiliares, que foram
 * modificadas ao longo do trabalho, pois são usadas em ambas funcionalidades
 * 
 * @version 1.0
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <SQLike.h>
#include <stdio.h>
#include <stdlib.h>
#include<index.h>

int main(void) {

    int option;

    int checker = scanf("%d", &option);
    if (checker == 0) {
        exit(2);
    }

    FILE* file = fopen("b.bin", "rb");

    switch (option) {
    case 1:
        SQLCreateTable();

        break;
    case 2:
        // função do tranche;
        SQLSelectFrom();

        break;
    case 3:
        readBinaryWriteIndex(file);
        break;
    default:
        printf("funcionalidade inválida\n");

        break;
    }

    fclose(file);
    return 0;
}