#include <stdio.h>

#include "damas.h"

void inicializar_tabuleiro(char tabuleiro[TAM][TAM]){

    for(int linha = 0; linha < TAM; linha++){

        for(int coluna = 0; coluna < TAM; coluna++){

            if((linha + coluna) % 2 == 0){

                tabuleiro[linha][coluna] = '#';

            }
            else {

                if(linha < 3){
                    tabuleiro[linha][coluna] = 'o';

                }else if(linha > 6){
                    tabuleiro[linha][coluna] = '@';
                } 
                else {
                    tabuleiro[linha][coluna] = ' ';
                }
            }
        }
    }
}

void imprimir_tabuleiro(char tabuleiro[TAM][TAM]){

    printf("   A B C D E F G H I J\n");
    printf("  +-+-+-+-+-+-+-+-+-+-+\n");


    for(int linha = 0; linha < TAM; linha++){

        printf("%d |", linha);

        for(int coluna = 0; coluna < TAM; coluna++){

            printf("%c|", tabuleiro[linha][coluna]);

        }
        printf(" %d", linha);
        printf("\n  +-+-+-+-+-+-+-+-+-+-+\n");
    }
    printf("   A B C D E F G H I J\n");
}