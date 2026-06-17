#include <stdio.h>
#include <stdlib.h>
#include "damas.h"


int main(int argc[], char *argv[]){

    char tabuleiro[TAM][TAM];
    inicializar_tabuleiro(tabuleiro);

    if(argc < 2){
        printf("Modo online selecionado.\n");
    }
    else{
        printf("Modo offline selecioado. Arquivo: %s\n", argv[1]);
    }
    
    return 0;

}


