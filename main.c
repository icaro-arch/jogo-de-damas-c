#include <stdio.h>
#include <stdlib.h>
#include "damas.h"


int main(int argc[], char *argv[]){

    char tabuleiro[TAM][TAM];
    inicializar_tabuleiro(tabuleiro);
    imprimir_tabuleiro(tabuleiro);

    if(argc < 2){
        printf("Modo online selecionado.\n");
        printf("Digite o jogador que ira iniciar(C/B):\n" );

        char primeiro_jogador;

        while(scanf(" %c", &primeiro_jogador) == 1 && primeiro_jogador != 'C' && primeiro_jogador != 'B'){

            if(primeiro_jogador != 'C' || primeiro_jogador != 'B'){

                printf("Jogador invalido.\n");

            }
        }            
        
        if(primeiro_jogador == 'C'){

            printf("O jogador de cima (C) ira iniciar!\n");
        
        } else {

            printf("O jogador de baixo(B) ira iniciar!\n");

        }

        modo_usuario_contra_usuario(tabuleiro, primeiro_jogador);

    }
    else{

        printf("Modo offline selecioado. Arquivo: %s\n", argv[1]);
        modo_offline(tabuleiro, argv[1]);

    }
    
    return 0;

}


