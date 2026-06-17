#include <stdio.h>
#include <stdlib.h>
#include "damas.h"


int main(){
    
    char matriz[TAM][TAM];

    inicializar_tabuleiro(matriz);

    imprimir_tabuleiro(matriz);

    return 0;

}


