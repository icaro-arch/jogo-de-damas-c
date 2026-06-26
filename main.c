/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "damas.h"

int main(int argc, char *argv[]){    

    srand(time(NULL)); //usar o relogio do computador para gerar a pseudoaleatoridade - icaro

    char tabuleiro[TAM][TAM];
    inicializar_tabuleiro(tabuleiro);

    if(argc < 2){
        printf("Modo online selecionado.\n");
        printf("Digite o jogador que ira iniciar(C/B):\n" );

        char primeiro_jogador;

        while(scanf(" %c", &primeiro_jogador) == 1 && primeiro_jogador != 'C' && primeiro_jogador != 'B'){
            printf("Jogador invalido.\n");
        }
        /* essa verificacao com if e redundante, o while ja faz isso - icaro
        if(primeiro_jogador != 'C' || primeiro_jogador != 'B'){

                printf("Jogador invalido.\n");

            }
        }            
        */   

        if(primeiro_jogador == 'C'){

            printf("O jogador de cima (C) ira iniciar!\n");
        
        } else {

            printf("O jogador de baixo (B) ira iniciar!\n");

        }

        modo_usuario_contra_usuario(tabuleiro, primeiro_jogador);

    }
    else{

        printf("\nModo offline selecionado. Arquivo: %s\n", argv[1]);
        modo_offline(tabuleiro, argv[1]);

    }
    
    return 0;

}


