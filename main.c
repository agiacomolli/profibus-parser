#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "profibus.h"

int main(int argc, char** argv)
{
    FILE *fDados = fopen("dados.txt", "r");
    if (fDados == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return EXIT_FAILURE;
    }

    FILE *fSaida = fopen("saida.txt", "w");
    if (fSaida == NULL) {
        printf("Erro ao criar o arquivo!\n");
        fclose(fDados);
        return EXIT_FAILURE;
    }

    unsigned char byte, byte_tmp;
    int frameNumber = 0;

    printf("Programa iniciado.\n");
    fputs("Leitura dos dados.\n", fSaida);

    frame fr;

    while (!feof(fDados)) {
        byte = getDado(fDados);
        byte_tmp = 0;

        getFrame(byte, fDados, fSaida);

    }
    
    fclose(fDados);
    fclose(fSaida);

    return EXIT_SUCCESS;
}

