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

void modo_offline(char tabuleiro[TAM][TAM], const char *nome_arquivo){

    FILE *arquivo = fopen(nome_arquivo, "r");

    if(arquivo == NULL){
        
        printf("Nao foi possivel abrir o arquivo '%s'.\n", nome_arquivo);
        
        return;
    }
    
    char primeiro_jogador;

    fscanf(arquivo, " %c", &primeiro_jogador);

    if(primeiro_jogador != 'C' && primeiro_jogador != 'B'){
        
        printf("Jogador inicial invalido.\n");
        
        fclose(arquivo);
        
        return;
    }
    
    printf("Jogador %c inicia a partida.\n", primeiro_jogador);

    char jogada[20];
    int numero_linha = 1;

    while((fscanf(arquivo, " %19s", jogada) != EOF)){

        numero_linha++;

        int coluna_inicial = jogada[0] - 'A';
        int linha_inicial = jogada[1] - '0';
        int coluna_final = jogada[4] - 'A';
        int linha_final = jogada[5] - '0';

        printf("Posicao inicial: [%d][%d].\n Posicao final: [%d][%d].",coluna_inicial, linha_inicial, coluna_final, linha_final);

        // fazer apos as regras
    }

    fclose(arquivo);
}

void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM], char primeiro_jogador){
    
}