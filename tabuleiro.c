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

// diretiva ANSI para colorir o terminal - icaro
#define ANSI_VERMELHO     "\033[1;31m"
#define ANSI_VERDE   "\033[1;32m"
#define ANSI_AMARELO  "\033[1;33m"
#define ANSI_AZUL    "\033[1;34m"
#define ANSI_RESET   "\033[0m"

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

            // identifica a peca atual - Icaro
            char peca = tabuleiro[linha][coluna];
            if (peca == 'o' || peca == 'O') {
                printf(ANSI_AZUL "%c" ANSI_RESET "|", peca); // azul para pecas e damas de cima - icaro
            } else if (peca == '@' || peca == '&') {
                printf(ANSI_VERMELHO "%c" ANSI_RESET "|", peca); // vermelho para pecas e damas de baixo - icaro
            } else {
                printf("%c|", peca); // espacos vazios e '#' vao continuam normais - icaro
            }

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

    // novo while (offline) - icaro
    // alterei para %6s para evitar estouros de memoria (a string jogada tem tamanho 7) - icaro
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

        // 3: movimentacao da peca que passou na validacao - icaro
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
        printf(ANSI_AZUL "O vencedor eh o usuario de CIMA.\n" ANSI_RESET);
    } else if(pecinhas_capturadas_cima < pecinhas_capturadas_baixo){
        printf(ANSI_VERMELHO "O vencedor eh o usuario de BAIXO.\n" ANSI_RESET);
    } else {
        printf(ANSI_AMARELO "O jogo deu empate.\n" ANSI_RESET);
    }

    fclose(arquivo); // garantir o fechamento do arquivo - icaro

}

void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM], char primeiro_jogador){

    char jogar_novamente = 'S'; //chave para fim do modo 

    int vitoria_registrada = 0; //chave para saber se e uma nova partida

    // contadores historicos de vitorias acumuladas - icaro
    int vitorias_cima = 0;
    int vitorias_baixo = 0;

    //novo laco mestre para poder reiniciar a partida - icaro
    while (jogar_novamente == 'S') {

        //escolhendo o jogador dessa nova partida - icaro
        if (vitoria_registrada > 0) {
            printf("Digite o jogador que ira iniciar esta partida (C/B):\n");
            while(scanf(" %c", &primeiro_jogador) == 1 && primeiro_jogador != 'C' && primeiro_jogador != 'B'){
                printf("Jogador invalido.\n");
            }
        }

        // limpar o tabuleiro a cada nova partida - icaro
        inicializar_tabuleiro(tabuleiro);

        char jogador_da_vez = primeiro_jogador;
        char jogada[7];
        int rodadas = 1;

        // variaveis chave para os bots (comecam desligadas) - icaro
        int bot_C = 0;
        int bot_B = 0;

        // variavel chave para saida manual (comeca desligada) - icaro
        int usuario_saiu = 0;
        
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
                printf(ANSI_VERMELHO "O jogador de BAIXO (@) venceu a partida!\n" ANSI_RESET);
                printf("-----------------------------------------------\n");
                //somador de vitorias - icaro
                vitoria_registrada++; 
                vitorias_baixo++;
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
                printf(ANSI_AZUL "O jogador de CIMA (o) venceu a partida!\n" ANSI_RESET);
                printf("-----------------------------------------------\n");
                //soma de vitorias - icaro
                vitoria_registrada++; 
                vitorias_cima++; 
                break;
            }

            #ifdef _WIN32
                system("cls"); 
            #else
                system("clear");
            #endif

            printf("Digite " ANSI_VERMELHO "SAIR" ANSI_RESET " para fechar o programa a qualquer momento.\n");
            printf("Digite " ANSI_VERDE "DICA" ANSI_RESET " para ver uma sugestao de movimento qualquer.\n");
            printf("Digite " ANSI_AZUL "AUTO" ANSI_RESET " para que o jogador selecionado vire um bot.\n");
            printf( "\n------ " ANSI_AMARELO "RODADA NUMERO %d" ANSI_RESET " ------\n" , rodadas);
            printf("\n");

            imprimir_tabuleiro(tabuleiro);  
            printf("\n------------------------------\n");

            // eexpressao logicaa de checagem se e a vez de um bot jogar - icaro
            int eh_bot = (jogador_da_vez == 'C' && bot_C == 1) || (jogador_da_vez == 'B' && bot_B == 1);

            if (eh_bot) {
                // se for bot, a maquina gera a jogada sozinha na variavel 'jogada' - icaro
                if (obter_sugestao_jogada(tabuleiro, jogador_da_vez, jogada)) {
                    if (jogador_da_vez == 'C') {
                        printf("\n" ANSI_AZUL "[BOT C]" ANSI_RESET " Escolheu e executou a jogada: %s\n", jogada);
                    } else {
                        printf("\n" ANSI_VERMELHO "[BOT B]" ANSI_RESET " Escolheu e executou a jogada: %s\n", jogada);
                    }
                printf(ANSI_VERDE "Pressione enter para continuar.\n" ANSI_RESET);
                char c; 
                while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
                if (!eh_bot) getchar(); // prossegue apos o enter
                }
            } else {
                // se for humano mantem o print que tinha feito antes - icaro
                if(jogador_da_vez == 'C') {
                    printf(ANSI_AMARELO "Jogador " ANSI_RESET ANSI_AZUL "%c" ANSI_RESET ANSI_AMARELO " digite a sua jogada:\n" ANSI_RESET, jogador_da_vez);
                }
                else {
                    printf(ANSI_AMARELO "Jogador " ANSI_RESET ANSI_VERMELHO "%c" ANSI_RESET ANSI_AMARELO " digite a sua jogada:\n" ANSI_RESET, jogador_da_vez);
                }
                scanf("%6s", jogada); 
            }
            
            if(jogada[0] == 'S' && jogada[1] == 'A' && jogada[2] == 'I' && jogada[3] == 'R'){
                printf("Usuario escolheu sair, fechando o programa...\n");
                usuario_saiu = 1; //finaliza sem perguntar se quer jogar novamente - icaro
                break;
            }
            
            if(jogada[0] == 'D' && jogada[1] == 'I' && jogada[2] == 'C' && jogada[3] == 'A'){
                char texto_dica[10];
                
                if (obter_sugestao_jogada(tabuleiro, jogador_da_vez, texto_dica)) {
                    printf("\n" ANSI_VERDE "[SUGESTAO]" ANSI_RESET " Que tal tentar a jogada: %s ?\n", texto_dica);
                } else {
                    printf("\n[DICA] Voce nao tem nenhuma jogada valida restando!\n");
                }
                
                printf("Pressione enter para voltar ao jogo.\n");
                char c; 
                while ((c = getchar()) != '\n' && c != EOF);
                getchar();
                continue; //recomeca o loop pedindo a jogada real, sem mudar o jogador da vez
            }

            if (jogada[0] == 'A' && jogada[1] == 'U' && jogada[2] == 'T' && jogada[3] == 'O') {
                // ativa o bot permanente para o jogador que digitou o comando - icaro
                if (jogador_da_vez == 'C') bot_C = 1;
                if (jogador_da_vez == 'B') bot_B = 1;
                
                printf("\n" ANSI_VERDE "[MODO AUTO ATIVADO!]" ANSI_RESET " O jogador %c agora e um bot por toda a partida.\n", jogador_da_vez);
                printf("Pressione enter para voltar ao jogo.\n");
                char c; 
                while ((c = getchar()) != '\n' && c != EOF);
                getchar();
                continue; // recomeca o loop, na proxima volta, 'eh_bot' sera verdadeiro (=1) - icaro
            }
        

            int coluna_inicial, linha_inicial, coluna_final, linha_final;
            int coluna_capturada, linha_capturada;

            // 2: parsear_jogada para validar o formato da entrada do jogador - icaro
            if (parsear_jogada(jogada, &coluna_inicial, &linha_inicial, &coluna_final, &linha_final) == 0) {
                printf(ANSI_VERMELHO "Jogada invalida.\n" ANSI_RESET);
                printf(ANSI_VERDE "Pressione enter para tentar novamente.\n" ANSI_RESET);
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
                    printf(ANSI_AMARELO "Jogada invalida.\n" ANSI_RESET);
                } else {
                    printf(ANSI_VERMELHO "Jogada invalida.\n" ANSI_RESET);
                }
                
                printf(ANSI_VERDE "Pressione enter para tentar novamente.\n" ANSI_RESET);
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
                // sempre joga de novo ao capturar (o jogador_da_vez nao muda) - icaro
                // para o bot - icaro
                if (eh_bot) {
                    if (jogador_da_vez == 'C') {
                        printf("\n" ANSI_AZUL "[BOT C]" ANSI_RESET " Capturou uma peca e vai jogar novamente!\n");
                    } else {
                        printf("\n" ANSI_VERMELHO "[BOT B]" ANSI_RESET " Capturou uma peca e vai jogar novamente!\n");
                    }
                // para o jogador - icaro
                } else {
                    printf(ANSI_VERDE "\nPeca capturada! Voce pode jogar de novo.\n" ANSI_RESET);
                }
                
                printf(ANSI_VERDE "Pressione enter para continuar.\n" ANSI_RESET);
                char c; 
                while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
                if (!eh_bot) getchar(); // prossegue apos o enter
            }
        }
        if (usuario_saiu == 1) {
            break;
        }  

        printf(ANSI_AMARELO "\nDesejam iniciar uma nova partida? (S/N): " ANSI_RESET);
        while(scanf(" %c", &jogar_novamente) == 1 && jogar_novamente != 'S' && jogar_novamente != 'N'){
                printf("Entrada invalida.\n");
        }
        char c; 
        while ((c = getchar()) != '\n' && c != EOF); // loop para limpeza do buffer
    }
    //placar final - icaro
    printf("\n===============================================\n");
    printf("               PLACAR FINAL                    \n");
    printf("===============================================\n");
    printf("Jogador de " ANSI_AZUL "CIMA (o)" ANSI_RESET " ganhou: " ANSI_VERDE "%d" ANSI_RESET " vez(es)\n", vitorias_cima);
    printf("Jogador de " ANSI_VERMELHO "BAIXO (@)" ANSI_RESET " ganhou: " ANSI_VERDE "%d" ANSI_RESET " vez(es)\n", vitorias_baixo);
    printf("===============================================\n");
    if(vitorias_cima > vitorias_baixo) {
        printf("             " ANSI_AZUL "JOGADOR C CAMPEAO" ANSI_RESET "             \n");
        printf("===============================================\n\n");
    }
    else if(vitorias_baixo > vitorias_cima) {
        printf("             " ANSI_VERMELHO "JOGADOR B CAMPEAO" ANSI_RESET "             \n");
        printf("===============================================\n\n");
    }
    else {
        printf("                 " ANSI_VERDE "EMPATE" ANSI_RESET "                \n");
        printf("===============================================\n\n");
    }
}