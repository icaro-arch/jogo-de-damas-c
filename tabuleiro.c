/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "damas.h"
#include "regras.h"

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

    char jogador_da_vez = primeiro_jogador; // declarei essa variavel para controlar os turnos - icaro 
    char jogada[7];
    int numero_linha = 1;

    /*  esse while foi feito antes das regras, modifiquei em um novo usando elas - icaro
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
    */

    // novo while (offline) - icaro
    // alterei para %6s para evitar estouros de memória (a string jogada tem tamanho 7) - icaro
    while((fscanf(arquivo, " %6s", jogada) != EOF)){

        numero_linha++;
        
        int coluna_inicial, linha_inicial, coluna_final, linha_final;
        int coluna_capturada, linha_capturada;

        // 1: usando parsear_jogada para validar o formato do texto tipo "B0--C1" - icaro
        if (parsear_jogada(jogada, &coluna_inicial, &linha_inicial, &coluna_final, &linha_final) == 0) {
            printf("Jogada invalida na linha %d do arquivo de entrada.\n", numero_linha);
            continue;
        }

        // 2: usando a função oficial validar_jogada feita pelo victor que testa todas as regras de movimento e captura - icaro
        ResultadoJogada res = validar_jogada(tabuleiro, coluna_inicial, linha_inicial, 
                                             coluna_final, linha_final, jogador_da_vez, 
                                             &coluna_capturada, &linha_capturada);

        if (res == JOGADA_INVALIDA) {
            printf("Jogada invalida na linha %d do arquivo de entrada.\n", numero_linha);
            continue;
        }

        // 3: movimentacao da peça que passou na validacao - icaro
        char pecinha = tabuleiro[linha_inicial][coluna_inicial];
        tabuleiro[linha_final][coluna_final] = pecinha;
        tabuleiro[linha_inicial][coluna_inicial] = ' ';

        // 4: se detectou captura, apaga a peca comida - icaro 
        if (res == JOGADA_CAPTURA) {
            tabuleiro[linha_capturada][coluna_capturada] = ' ';
        }

        // 5: chamada da promocao automatica caso a peca tenha chegado no fim do tabuleiro - icaro
        verificar_promocao(tabuleiro, coluna_final, linha_final);

        // 6: gerenciamento de turnos no modo offline com a regra estabelecida pelo Miguel - icaro
        if (res == JOGADA_SIMPLES) {
            jogador_da_vez = (jogador_da_vez == 'C') ? 'B' : 'C';
        } else if (res == JOGADA_CAPTURA) {
            // se capturou, o jogador_da_vez NAO muda, ele joga de novo - icaro
        }
    }

    // usando a funcao criada pelo victor para contar as pecas restantes - icaro
    // inves da contagem manual no antigo while - icaro
    int pecinhas_restantes_cima = contar_pecas_jogador(tabuleiro, 'C');
    int pecinhas_restantes_baixo = contar_pecas_jogador(tabuleiro, 'B');

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

    fclose(arquivo); // garantir o fechamento do arquivo - icaro

}

void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM], char primeiro_jogador){

    char jogador_da_vez = primeiro_jogador;
    char jogada[7];
    int rodadas = 1;

    /*  esse while tambem foi feito antes das regras, tambem modifiquei em um novo usando elas - icaro
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
        printf("\n------------------------------\n");
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
    */

    // novo while (usuario contra usuario) - icaro
    while(1){
        // 1: condicao de fim de partida. Verifica (precisa ser antes de comecar a rodada) se alguem perdeu - icaro
        // perde quem ficar sem peças ou quem nao tiver movimentos disponiveis - icaro
        if (contar_pecas_jogador(tabuleiro, 'C') == 0 || jogador_sem_jogadas(tabuleiro, 'C')) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            imprimir_tabuleiro(tabuleiro);
            printf("\n-----------------------------------------------\n");
            printf("FIM DE JOGO! O jogador de BAIXO (@) venceu a partida!\n");
            printf("-----------------------------------------------\n");
            break;
        }
        if (contar_pecas_jogador(tabuleiro, 'B') == 0 || jogador_sem_jogadas(tabuleiro, 'B')) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            imprimir_tabuleiro(tabuleiro);
            printf("\n-----------------------------------------------\n");
            printf("FIM DE JOGO! O jogador de CIMA (o) venceu a partida!\n");
            printf("-----------------------------------------------\n");
            break;
        }

        #ifdef _WIN32
            system("cls"); 
        #else
            system("clear");
        #endif

        printf("Digite SAIR para fechar o programa a qualquer momento.\n");
        printf("\n------ RODADA NUMERO %d ------\n", rodadas);
        printf("\n");

        imprimir_tabuleiro(tabuleiro);
        printf("\n------------------------------\n");
        printf("Jogador %c digite a sua jogada:\n", jogador_da_vez);

        scanf("%6s", jogada);
        
        if(jogada[0] == 'S' && jogada[1] == 'A' && jogada[2] == 'I' && jogada[3] == 'R'){
            printf("Usuario escolheu sair, fechando o programa...\n");
            break;
        }

        int coluna_inicial, linha_inicial, coluna_final, linha_final;
        int coluna_capturada, linha_capturada;

        // 2: parsear_jogada para validar o formato da entrada do jogador - icaro
        if (parsear_jogada(jogada, &coluna_inicial, &linha_inicial, &coluna_final, &linha_final) == 0) {
            printf("Jogada ou formato invalido!\n");
            printf("Pressione enter para tentar novamente.\n");
            char c; 
            while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
            getchar(); // prossegue apos o enter
            continue;
        }

        // 3: valida as regras de movimentacao no tabuleiro - icaro
        ResultadoJogada res = validar_jogada(tabuleiro, coluna_inicial, linha_inicial, 
                                             coluna_final, linha_final, jogador_da_vez, 
                                             &coluna_capturada, &linha_capturada);

        if (res == JOGADA_INVALIDA) {
            // verifica se o erro foi por ignorar uma captura obrigatoria
            if (existe_captura_obrigatoria(tabuleiro, jogador_da_vez)) {
                printf("Voce tem uma captura obrigatoria!\n");
            } else {
                printf("Jogada ou formato invalido!\n");
            }
            
            printf("Pressione enter para tentar novamente.\n");
            char c;
            while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
            getchar(); // prossegue apos o enter
            continue;
        }

        // 4: execucao do movimento aprovado - icaro
        char pecinha = tabuleiro[linha_inicial][coluna_inicial];
        tabuleiro[linha_final][coluna_final] = pecinha;
        tabuleiro[linha_inicial][coluna_inicial] = ' ';

        //5: se foi captura, apaga a peca comida do tabuleiro de jogo - icaro
        if (res == JOGADA_CAPTURA) {
            tabuleiro[linha_capturada][coluna_capturada] = ' ';
        }

        // 6: aplicacao de promocao automatica para Dama - icaro
        verificar_promocao(tabuleiro, coluna_final, linha_final);

        // 7: mecanica de turnos mais regra de jogar novamente estabelecida pelo Miguel - icaro
        if (res == JOGADA_SIMPLES) {
            jogador_da_vez = (jogador_da_vez == 'C') ? 'B' : 'C';
            rodadas++;
        } else if (res == JOGADA_CAPTURA) {
            // sempre joga de novo ao capturar (o jogador_da_vez NAO muda) - icaro
            printf("\nPeca capturada! Voce pode jogar de novo.\n");
            printf("Pressione enter para continuar.\n");
            char c; 
            while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
            getchar(); // prossegue apos o enter
        }
    }
}

