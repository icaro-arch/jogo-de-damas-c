/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#include "regras.h"

/*Validacao de sintaxe e limites */

/*
 * Converte um caractere de coluna ('A'-'J' ou 'a'-'j') para indice
 * 0-9. Devolve -1 se o caractere nao for uma coluna valida.
 */
int converter_coluna(char c) {
    if (c >= 'A' && c <= 'J') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'j') {
        return c - 'a';
    }
    return -1;
}

/*
 * Converte um caractere de linha ('0'-'9') para indice 0-9.
 * Devolve -1 se o caractere nao for um digito valido.
 */
int converter_linha(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return -1;
}

/* Verifica se uma coordenada coluna, linha esta dentro dos limites do tabuleiro 10x10.*/
int posicao_dentro_limites(int coluna, int linha) {
    return (coluna >= 0 && coluna < TAM && linha >= 0 && linha < TAM);
}

/*Faz o parsing da string digitada no formato estrito, por exemplo "B0--C1".*/
int parsear_jogada(const char *entrada, int *coluna_inicial, int *linha_inicial,
                    int *coluna_final, int *linha_final) {
    size_t tamanho;

    if (entrada == NULL || coluna_inicial == NULL || linha_inicial == NULL ||
        coluna_final == NULL || linha_final == NULL) {
        return 0;
    }

    tamanho = strlen(entrada);

    // Formato esperado tem exatamente 6 caracteres;
    if (tamanho != 6) {
        return 0;
    }

    // Posições dos '-';
    if (entrada[2] != '-' || entrada[3] != '-') {
        return 0;
    }

    int col_ini = converter_coluna(entrada[0]);
    int lin_ini = converter_linha(entrada[1]);
    int col_fim = converter_coluna(entrada[4]);
    int lin_fim = converter_linha(entrada[5]);

    if (col_ini == -1 || lin_ini == -1 || col_fim == -1 || lin_fim == -1) {
        return 0;
    }

    *coluna_inicial = col_ini;
    *linha_inicial = lin_ini;
    *coluna_final = col_fim;
    *linha_final = lin_fim;

    return 1;
}

/* Verifica se a posicao corresponde a uma casa jogavel, uma casa que nao tem '#';*/
int casa_jogavel(char tabuleiro[TAM][TAM], int coluna, int linha) {
    if (!posicao_dentro_limites(coluna, linha)) {
        return 0;
    }
    return tabuleiro[linha][coluna] != '#';
}

/*Validacao de movimentos comuns e promocao*/

//Retorna 1 se o caractere representa uma casa vazia (' ');
static int casa_vazia(char c) {
    return c == ' ';
}


 // Retorna 1 se o caractere representa uma peca;
static int eh_peca(char c) {
    return c == 'o' || c == 'O' || c == '@' || c == '&';
}

//Retorna 1 se a peca pertence ao jogador de cima;
static int eh_peca_de_cima(char c) {
    return c == 'o' || c == 'O';
}

//Retorna 1 se a peca pertence ao jogador de baixo;
static int eh_peca_de_baixo(char c) {
    return c == '@' || c == '&';
}

//Retorna 1 se a peca pertence ao jogador informado;
int peca_pertence_ao_jogador(char peca, char jogador) {
    if (jogador == 'C') {
        return eh_peca_de_cima(peca);
    }
    if (jogador == 'B') {
        return eh_peca_de_baixo(peca);
    }
    return 0;
}

//Retorna 1 se o caractere representa uma dama;.
int eh_dama(char peca) {
    return peca == 'O' || peca == '&';
}

// Retorna a direcao de avanco de um jogador;
// 1 ou -1 no sentido do tabuleiro;
static int direcao_avanco(char jogador) {
    if (jogador == 'C') {
        return 1;
    }
    if (jogador == 'B') {
        return -1;
    }
    return 0;
}

//Verifica se o caminho em linha reta diagonal entre origem e destino ta livre;
static int caminho_diagonal_livre(char tabuleiro[TAM][TAM], int coluna_inicial,
                                   int linha_inicial, int coluna_final,
                                   int linha_final) {
    //Qual lado a peça se move;
    int passo_linha = (linha_final > linha_inicial) ? 1 : -1;
    int passo_coluna = (coluna_final > coluna_inicial) ? 1 : -1;
    int linha = linha_inicial + passo_linha;
    int coluna = coluna_inicial + passo_coluna;

    while (linha != linha_final && coluna != coluna_final) {
        if (!casa_vazia(tabuleiro[linha][coluna])) {
            return 0;
        }
        linha += passo_linha;
        coluna += passo_coluna;
    }

    return 1;
}

/*Valida um movimento simples de origem para destino. Pressupoe que a origem contem uma peca do jogador e que origem é diferente de destino;*/
int validar_movimento_simples(char tabuleiro[TAM][TAM], int coluna_inicial,
                               int linha_inicial, int coluna_final,
                               int linha_final, char jogador) {
    int diff_linha = linha_final - linha_inicial;
    int diff_coluna = coluna_final - coluna_inicial;
    char peca = tabuleiro[linha_inicial][coluna_inicial];

    if (!casa_vazia(tabuleiro[linha_final][coluna_final])) {
        return 0;
    }
    if (abs(diff_linha) != abs(diff_coluna) || diff_linha == 0) {
        return 0;
    }

    if (eh_dama(peca)) {
        return caminho_diagonal_livre(tabuleiro, coluna_inicial, linha_inicial,
                                       coluna_final, linha_final);
    }

    /* Peca normal é 1 casa e sempre para frente*/
    if (abs(diff_linha) != 1) {
        return 0;
    }

    if (diff_linha != direcao_avanco(jogador)) {
        return 0;
    }

    return 1;
}

/*Verifica se a peca que acabou de chegar na coluna_final, linha_final*/
void verificar_promocao(char tabuleiro[TAM][TAM], int coluna_final, int linha_final) {
    char peca = tabuleiro[linha_final][coluna_final];

    if (peca == 'o' && linha_final == TAM - 1) {
        tabuleiro[linha_final][coluna_final] = 'O';
    } else if (peca == '@' && linha_final == 0) {
        tabuleiro[linha_final][coluna_final] = '&';
    }
}


/*Valida uma captura de peça normal, salto de exatamente 2 casas na diagonal, onde a casa intermediaria contem exatamente uma peca adversaria
e a casa de destino esta vazia.*/
static int validar_captura_peao(char tabuleiro[TAM][TAM], int coluna_inicial,
                                 int linha_inicial, int coluna_final,
                                 int linha_final, char jogador,
                                 int *coluna_capturada, int *linha_capturada) {
    int diff_linha = linha_final - linha_inicial;
    int diff_coluna = coluna_final - coluna_inicial;

    /* Deve ser um salto diagonal de exatamente 2 casas */
    if (abs(diff_linha) != 2 || abs(diff_coluna) != 2) {
        return 0;
    }

    if (!casa_vazia(tabuleiro[linha_final][coluna_final])) {
        return 0;
    }

    int linha_meio = linha_inicial + diff_linha / 2;
    int coluna_meio = coluna_inicial + diff_coluna / 2;

    char peca_meio = tabuleiro[linha_meio][coluna_meio];

    if (!eh_peca(peca_meio)) {
        return 0;
    }

    /* A peca do meio precisa ser do adversario do jogador atual */
    if (peca_pertence_ao_jogador(peca_meio, jogador)) {
        return 0;
    }

    *linha_capturada = linha_meio;
    *coluna_capturada = coluna_meio;
    return 1;
}

/*Valida uma captura da dama: na diagonal o caminho antes da peca adversaria deve estar livre, a peca
  encontrada deve ser exatamente uma e deve tem uma casa vazia*/
static int validar_captura_dama(char tabuleiro[TAM][TAM], int coluna_inicial,
                                 int linha_inicial, int coluna_final,
                                 int linha_final, char jogador,
                                 int *coluna_capturada, int *linha_capturada) {
    int diff_linha = linha_final - linha_inicial;
    int diff_coluna = coluna_final - coluna_inicial;

    if (abs(diff_linha) != abs(diff_coluna) || diff_linha == 0) {
        return 0;
    }

    int passo_linha = (diff_linha > 0) ? 1 : -1;
    int passo_coluna = (diff_coluna > 0) ? 1 : -1;

    int linha = linha_inicial + passo_linha;
    int coluna = coluna_inicial + passo_coluna;

    int pecas_adversarias_encontradas = 0;
    int linha_adversaria = 0;
    int coluna_adversaria = 0;
    int passou_pela_peca = 0;

    while (linha != linha_final || coluna != coluna_final) {
        char atual = tabuleiro[linha][coluna];

        if (!passou_pela_peca) {
            if (eh_peca(atual)) {
                if (peca_pertence_ao_jogador(atual, jogador)) {
                    return 0;
                }
                pecas_adversarias_encontradas++;
                if (pecas_adversarias_encontradas > 1) {
                    return 0;
                }
                linha_adversaria = linha;
                coluna_adversaria = coluna;
                passou_pela_peca = 1;
            }
        } else {
            if (!casa_vazia(atual)) {
                return 0;
            }
        }

        linha += passo_linha;
        coluna += passo_coluna;
    }
    if (!passou_pela_peca) {
        return 0;
    }

    if (!casa_vazia(tabuleiro[linha_final][coluna_final])) {
        return 0;
    }

    *linha_capturada = linha_adversaria;
    *coluna_capturada = coluna_adversaria;
    return 1;
}

/*Valida um movimento de captura do peao ou dama*/
int validar_captura(char tabuleiro[TAM][TAM], int coluna_inicial, int linha_inicial,
                     int coluna_final, int linha_final, char jogador,
                     int *coluna_capturada, int *linha_capturada) {
    char peca = tabuleiro[linha_inicial][coluna_inicial];

    if (eh_dama(peca)) {
        return validar_captura_dama(tabuleiro, coluna_inicial, linha_inicial,
                                     coluna_final, linha_final, jogador,
                                     coluna_capturada, linha_capturada);
    }

    return validar_captura_peao(tabuleiro, coluna_inicial, linha_inicial,
                                 coluna_final, linha_final, jogador,
                                 coluna_capturada, linha_capturada);
}

/*Testa as 4 diagonais procurando captura.*/
int peca_possui_captura(char tabuleiro[TAM][TAM], int coluna, int linha, char jogador) {
    char peca = tabuleiro[linha][coluna];
    int coluna_capturada, linha_capturada;
    int dl, dc;

    if (eh_dama(peca)) {
        for (dl = -1; dl <= 1; dl += 2) {
            for (dc = -1; dc <= 1; dc += 2) {
                int dist;
                for (dist = 1; dist < TAM; dist++) {
                    int linha_destino = linha + dl * dist;
                    int coluna_destino = coluna + dc * dist;

                    if (!posicao_dentro_limites(coluna_destino, linha_destino)) {
                        break;
                    }

                    if (validar_captura_dama(tabuleiro, coluna, linha,
                                              coluna_destino, linha_destino,
                                              jogador, &coluna_capturada,
                                              &linha_capturada)) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }

    for (dl = -1; dl <= 1; dl += 2) {
        for (dc = -1; dc <= 1; dc += 2) {
            int linha_destino = linha + dl * 2;
            int coluna_destino = coluna + dc * 2;

            if (!posicao_dentro_limites(coluna_destino, linha_destino)) {
                continue;
            }

            if (validar_captura_peao(tabuleiro, coluna, linha, coluna_destino,
                                      linha_destino, jogador, &coluna_capturada,
                                      &linha_capturada)) {
                return 1;
            }
        }
    }

    return 0;
}

int existe_captura_obrigatoria(char tabuleiro[TAM][TAM], char jogador) {
    int linha, coluna;
    /* Percorre o tabuleiro inteiro procurando alguma captura;*/
    for (linha = 0; linha < TAM; linha++) {
        for (coluna = 0; coluna < TAM; coluna++) {
            char peca = tabuleiro[linha][coluna];

            if (peca_pertence_ao_jogador(peca, jogador)) {
                if (peca_possui_captura(tabuleiro, coluna, linha, jogador)) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

/*Funcao principal de validacao. 
  Retorna JOGADA_INVALIDA, JOGADA_SIMPLES ou JOGADA_CAPTURA. Definidos no regras.h;*/
ResultadoJogada validar_jogada(char tabuleiro[TAM][TAM], int coluna_inicial,
                                int linha_inicial, int coluna_final, int linha_final,
                                char jogador, int *coluna_capturada, int *linha_capturada) {
    /* verifica limites e casas jogaveis*/
    if (!posicao_dentro_limites(coluna_inicial, linha_inicial) ||
        !posicao_dentro_limites(coluna_final, linha_final)) {
        return JOGADA_INVALIDA;
    }

    if (!casa_jogavel(tabuleiro, coluna_inicial, linha_inicial) ||
        !casa_jogavel(tabuleiro, coluna_final, linha_final)) {
        return JOGADA_INVALIDA;
    }

    /*Origem nao pode ser igual ao destino*/
    if (linha_inicial == linha_final && coluna_inicial == coluna_final) {
        return JOGADA_INVALIDA;
    }

    /*Origem deve conter peca do jogador da vez*/
    char peca_origem = tabuleiro[linha_inicial][coluna_inicial];
    if (!peca_pertence_ao_jogador(peca_origem, jogador)) {
        return JOGADA_INVALIDA;
    }

    /*Obrigatoriedade de captura*/
    int captura_obrigatoria = existe_captura_obrigatoria(tabuleiro, jogador);

    if (validar_captura(tabuleiro, coluna_inicial, linha_inicial, coluna_final,
                         linha_final, jogador, coluna_capturada, linha_capturada)) {
        return JOGADA_CAPTURA;
    }

    if (captura_obrigatoria) {
        return JOGADA_INVALIDA;
    }

    if (validar_movimento_simples(tabuleiro, coluna_inicial, linha_inicial,
                                   coluna_final, linha_final, jogador)) {
        return JOGADA_SIMPLES;
    }

    return JOGADA_INVALIDA;
}


int contar_pecas_jogador(char tabuleiro[TAM][TAM], char jogador) {
    int linha, coluna;
    int total = 0;

    for (linha = 0; linha < TAM; linha++) {
        for (coluna = 0; coluna < TAM; coluna++) {
            if (peca_pertence_ao_jogador(tabuleiro[linha][coluna], jogador)) {
                total++;
            }
        }
    }

    return total;
}

int jogador_sem_jogadas(char tabuleiro[TAM][TAM], char jogador) {
    int linha, coluna;

    /* Se ha captura obrigatoria disponivel entao tem jogada*/
    if (existe_captura_obrigatoria(tabuleiro, jogador)) {
        return 0;
    }

    for (linha = 0; linha < TAM; linha++) {
        for (coluna = 0; coluna < TAM; coluna++) {
            char peca = tabuleiro[linha][coluna];

            if (!peca_pertence_ao_jogador(peca, jogador)) {
                continue;
            }
            int dl, dc;
            for (dl = -1; dl <= 1; dl += 2) {
                for (dc = -1; dc <= 1; dc += 2) {
                    int dist;
                    int max_dist = eh_dama(peca) ? TAM - 1 : 1;

                    for (dist = 1; dist <= max_dist; dist++) {
                        int linha_destino = linha + dl * dist;
                        int coluna_destino = coluna + dc * dist;

                        if (!posicao_dentro_limites(coluna_destino, linha_destino)) {
                            break;
                        }

                        if (validar_movimento_simples(tabuleiro, coluna, linha,
                                                        coluna_destino, linha_destino,
                                                        jogador)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

// Funcao para encontrar varias jogadas validas e sugerir uma aleatoria - icaro
int obter_sugestao_jogada(char tabuleiro[TAM][TAM], char jogador, char *sugestao_str) {
    int coluna_capturada, linha_capturada;
    int tem_captura_obrigatoria = existe_captura_obrigatoria(tabuleiro, jogador);

    // matriz para armazenar os textos das jogadas validas
    char lista_jogadas[200][10];
    int total_jogadas = 0;

    // varre todas as posicoes iniciais possiveis para a epca e todas as possiveis para ela ir - icaro
    for (int linha_inicial = 0; linha_inicial < TAM; linha_inicial++) {
        for (int coluna_inicial = 0; coluna_inicial < TAM; coluna_inicial++) {
            for (int linha_final = 0; linha_final < TAM; linha_final++) {
                for (int coluna_final = 0; coluna_final < TAM; coluna_final++) {
                    
                    ResultadoJogada resultado = validar_jogada(tabuleiro, 
                                                               coluna_inicial, linha_inicial, 
                                                               coluna_final, linha_final, 
                                                               jogador, 
                                                               &coluna_capturada, &linha_capturada);
                    
                    // se a captura for obrigatoria guardamos apenas ela na matriz - icaro
                    if (tem_captura_obrigatoria && resultado == JOGADA_CAPTURA) {
                        //sprintf para armazenar a string - icaro
                        //esse + 'A' serve para converter o numero em letra de novo
                        sprintf(lista_jogadas[total_jogadas], "%c%d--%c%d", coluna_inicial + 'A', linha_inicial, coluna_final + 'A', linha_final);
                        total_jogadas++;
                    }
                    // se nao houver nenhuma obrigatoria guardamos os movimentos possiveis - icaro
                    else if (!tem_captura_obrigatoria && resultado == JOGADA_SIMPLES) {
                        //sprintf para armazenar a string - icaro
                        //esse + 'A' serve para converter o numero em letra de novo
                        sprintf(lista_jogadas[total_jogadas], "%c%d--%c%d", coluna_inicial + 'A', linha_inicial, coluna_final + 'A', linha_final);
                        total_jogadas++;
                    }
                }
            }
        }
    }

    // se encontramos pelo menos uma jogada valida, sorteamos uma delas usando rand
    if (total_jogadas > 0) {
        int indice_sorteado = rand() % total_jogadas;
        strcpy(sugestao_str, lista_jogadas[indice_sorteado]);
        return 1; // dica sorteada
    }

    return 0; // caso nenhuma disponivel

}