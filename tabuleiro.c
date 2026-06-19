#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    printf("     A B C D E F G H I J\n");
    printf("    +-+-+-+-+-+-+-+-+-+-+\n");


    for(int linha = 0; linha < TAM; linha++){

        printf("  %d |", linha);

        for(int coluna = 0; coluna < TAM; coluna++){

            printf("%c|", tabuleiro[linha][coluna]);

        }
        printf(" %d", linha);

        printf("\n    +-+-+-+-+-+-+-+-+-+-+\n");
    }

    printf("     A B C D E F G H I J\n");
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

    char jogada[7];
    int numero_linha = 1;

    while((fscanf(arquivo, " %7s", jogada) != EOF)){

        numero_linha++;
        
        if(validar_jogada(jogada) == 0){

                printf("Jogada invalida na linha %d do arquivo de entrada.\n", numero_linha); // continua lendo, mas nao faz a jogada. preciso das regras
                continue;

        }

        int coluna_inicial = jogada[0] - 'A';
        int linha_inicial = jogada[1] - '0';
        int coluna_final = jogada[4] - 'A';
        int linha_final = jogada[5] - '0';
    
        char pecinha = tabuleiro[linha_inicial][coluna_inicial];
        tabuleiro[linha_final][coluna_final] = pecinha;
        tabuleiro[linha_inicial][coluna_inicial] = ' ';
    }

    fclose(arquivo);

    printf("\n------ TABULEIRO FINAL ------\n");
    printf("\n");

    imprimir_tabuleiro(tabuleiro);

    int pecinhas_restantes_cima = 0;
    int pecinhas_restantes_baixo = 0;

    for(int linha = 0; linha < TAM; linha++){

        for(int coluna = 0; coluna < TAM; coluna++){

            if(tabuleiro[linha][coluna] == 'o' || tabuleiro[linha][coluna] == 'O'){
                pecinhas_restantes_cima++;
            }
            if(tabuleiro[linha][coluna] == '@' || tabuleiro[linha][coluna] == '&'){
                pecinhas_restantes_baixo++;
            }
        }
    }

    int pecinhas_capturadas_cima = 15 - pecinhas_restantes_baixo;
    int pecinhas_capturadas_baixo = 15 - pecinhas_restantes_cima;

    printf("------------------------------\n");
    printf("Pecinhas capturadas por cada jogador:\n");
    printf("Cima = %d / Baixo = %d\n", pecinhas_capturadas_cima, pecinhas_capturadas_baixo);

    if(pecinhas_capturadas_cima > pecinhas_capturadas_baixo){
        printf("O vencedor eh o usuario de CIMA.\n");
    } else if(pecinhas_capturadas_cima < pecinhas_capturadas_baixo){
        printf("O vencedor eh o usuario de BAIXO.\n");
    } else {
        printf("O jogo deu empate.\n");
    }

}

void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM], char primeiro_jogador){

    char jogador_da_vez = primeiro_jogador;
    char jogada[7];
    int rodadas = 1;

    while(1){

        #ifdef _WIN32
            system("cls"); // limpa a tela tanto no ws quanto no linux ou mac.
        #else
            system("clear");
        #endif

        // apenas pra testar ate ter as regras
        printf("Digite SAIR para fechar o programa a qualquer momento.\n");
        printf("\n------ RODADA NUMERO %d ------\n", rodadas);
        printf("\n");

        imprimir_tabuleiro(tabuleiro);
        printf("------------------------------\n");
        printf("Jogador %c digite a sua jogada:\n", jogador_da_vez);



        scanf("%6s", jogada);
        
        if(jogada[0] == 'S' && jogada[1] == 'A' && jogada[2] == 'I' && jogada[3] == 'R'){

            printf("Usuario escolheu sair, fechando o programa...\n");
            break;

        }

        if(validar_jogada(jogada) ==  0){

            printf("Jogada ou formato invalido!\n");
            printf("Pressione enter para tentar novamente.\n");

            char c; while ((c = getchar()) != '\n' && c != EOF); // 1. Limpa a sugeira no buffer. \n permanece e é preciso remover.
            getchar(); // 2. Após buffer limpo, podemos pausar de verdade.
            continue;
        }

        int coluna_inicial = jogada[0] - 'A';
        int linha_inicial = jogada[1] - '0';
        int coluna_final = jogada[4] - 'A';
        int linha_final = jogada[5] - '0';

        char pecinha = tabuleiro[linha_inicial][coluna_inicial];

        tabuleiro[linha_final][coluna_final] = pecinha;

        tabuleiro[linha_inicial][coluna_inicial] = ' ';

        if(jogador_da_vez == 'C'){
            jogador_da_vez = 'B';
        } else {
            jogador_da_vez = 'C';
        }  
        rodadas++;
    }
}

int validar_jogada(char jogada[]){

    if(strlen(jogada) != 6){
        return 0;
    }

    if(jogada[0] < 'A' || jogada[0] > 'J' || 
        jogada[1] < '0' || jogada[1] > '9' ||
        jogada[2] != '-' || jogada[3] != '-' ||    
        jogada[4] < 'A' || jogada[4] > 'J' || 
        jogada[5] < '0' || jogada[5] > '9' ) {

        return 0;
    }

    return 1;
}