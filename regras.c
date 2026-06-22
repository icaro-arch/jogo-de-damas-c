/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#include "regras.h"

/* 2.1 - Validacao de sintaxe e limites */

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

/*
 * Verifica se uma coordenada (coluna, linha) esta dentro dos limites
 * do tabuleiro 10x10. Como converter_coluna/converter_linha ja
 * restringem a faixa 0-9, esta funcao serve principalmente como uma
 * checagem de seguranca extra para qualquer coordenada calculada em
 * outros pontos do codigo (ex: diagonais de dama, saltos de captura).
 */
int posicao_dentro_limites(int coluna, int linha) {
    return (coluna >= 0 && coluna < TAM && linha >= 0 && linha < TAM);
}

/*
 * Faz o parsing da string digitada/lida no formato estrito
 * "<COL_INI><LIN_INI>--<COL_FIM><LIN_FIM>", por exemplo "B0--C1".
 *
 * Retorna 1 se o formato estiver correto e preenche, por ponteiro,
 * as 4 coordenadas convertidas. Retorna 0 se o formato estiver
 * incorreto (tamanho errado, falta o "--", coluna/linha fora de
 * A-J/0-9, etc). Neste caso, as coordenadas de saida nao devem ser
 * consideradas validas.
 *
 * Observacao: esta funcao SO valida o formato da string. Nao valida
 * se origem == destino (isso e regra de jogo, feito em validar_jogada)
 */
int parsear_jogada(const char *entrada, int *coluna_inicial, int *linha_inicial,
                    int *coluna_final, int *linha_final) {
    size_t tamanho;

    if (entrada == NULL || coluna_inicial == NULL || linha_inicial == NULL ||
        coluna_final == NULL || linha_final == NULL) {
        return 0;
    }

    tamanho = strlen(entrada);

    /* Formato esperado tem exatamente 6 caracteres:
     * [0] coluna inicial
     * [1] linha inicial
     * [2] '-'
     * [3] '-'
     * [4] coluna final
     * [5] linha final
     */
    if (tamanho != 6) {
        return 0;
    }

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

/*
 * Verifica se a posicao corresponde a uma casa jogavel, ou seja,
 * uma casa que NAO contem o caractere '#'. Pressupoe que (coluna,
 * linha) ja esta dentro dos limites do tabuleiro (chame
 * posicao_dentro_limites antes, se a origem da coordenada nao for
 * confiavel).
 */
int casa_jogavel(char tabuleiro[TAM][TAM], int coluna, int linha) {
    if (!posicao_dentro_limites(coluna, linha)) {
        return 0;
    }
    return tabuleiro[linha][coluna] != '#';
}

/*2.2 - Validacao de movimentos comuns e promocao (U.4, U.5, U.14)*/

/*
 * Retorna 1 se o caractere representa uma casa vazia (' ').
 */
static int casa_vazia(char c) {
    return c == ' ';
}

/*
 * Retorna 1 se o caractere representa uma peca (normal ou dama) de
 * QUALQUER jogador. Util para checar se uma casa esta ocupada.
 */
static int eh_peca(char c) {
    return c == 'o' || c == 'O' || c == '@' || c == '&';
}

/*
 * Retorna 1 se a peca pertence ao jogador de cima ('C'): 'o' ou 'O'.
 */
static int eh_peca_de_cima(char c) {
    return c == 'o' || c == 'O';
}

/*
 * Retorna 1 se a peca pertence ao jogador de baixo ('B'): '@' ou '&'.
 */
static int eh_peca_de_baixo(char c) {
    return c == '@' || c == '&';
}

/*
 * Retorna 1 se a peca pertence ao jogador informado ('C' ou 'B').
 * Pecas normais e damas do mesmo lado contam como "do jogador".
 */
int peca_pertence_ao_jogador(char peca, char jogador) {
    if (jogador == 'C') {
        return eh_peca_de_cima(peca);
    }
    if (jogador == 'B') {
        return eh_peca_de_baixo(peca);
    }
    return 0;
}

/*
 * Retorna 1 se o caractere representa uma dama ('O' ou '&').
 */
int eh_dama(char peca) {
    return peca == 'O' || peca == '&';
}

/*
 * Retorna a "direcao de avanco" (para frente) de um jogador, em
 * termos de variacao de linha:
 *  - jogador 'C' avanca de linhas menores para maiores  -> +1
 *  - jogador 'B' avanca de linhas maiores para menores  -> -1
 * Retorna 0 caso o caractere do jogador seja invalido (nao deveria
 * ocorrer se a entrada for sempre validada antes).
 */
static int direcao_avanco(char jogador) {
    if (jogador == 'C') {
        return 1;
    }
    if (jogador == 'B') {
        return -1;
    }
    return 0;
}

/*
 * Verifica se o caminho em linha recta diagonal entre origem e
 * destino (exclusive ambas as extremidades) esta totalmente livre
 * de pecas. Pressupoe que origem e destino estao na mesma diagonal
 * (mesma diferenca absoluta entre linhas e colunas).
 */
static int caminho_diagonal_livre(char tabuleiro[TAM][TAM], int coluna_inicial,
                                   int linha_inicial, int coluna_final,
                                   int linha_final) {
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

/*
 * Valida um movimento SIMPLES (sem captura) de origem para destino.
 * Pressupoe que a origem contem uma peca do jogador e que origem !=
 * destino (essas checagens sao feitas em validar_jogada). Verifica:
 *  - se o movimento e diagonal;
 *  - se o destino esta vazio;
 *  - para peca normal: exatamente 1 casa, sempre para frente ;
 *  - para dama: 1 ou mais casas, qualquer direcao, caminho livre
 * Nao verifica obrigatoriedade de captura  - isso e feito em
 * validar_jogada, que chama existe_captura_obrigatoria antes de aceitar
 * um movimento simples.
 */
int validar_movimento_simples(char tabuleiro[TAM][TAM], int coluna_inicial,
                               int linha_inicial, int coluna_final,
                               int linha_final, char jogador) {
    int diff_linha = linha_final - linha_inicial;
    int diff_coluna = coluna_final - coluna_inicial;
    char peca = tabuleiro[linha_inicial][coluna_inicial];

    /* Destino deve estar vazio */
    if (!casa_vazia(tabuleiro[linha_final][coluna_final])) {
        return 0;
    }

    /* Movimento deve ser diagonal: variacao igual em modulo entre
     * linha e coluna */
    if (abs(diff_linha) != abs(diff_coluna) || diff_linha == 0) {
        return 0;
    }

    if (eh_dama(peca)) {
        /* Dama: qualquer distancia, qualquer direcao, caminho livre */
        return caminho_diagonal_livre(tabuleiro, coluna_inicial, linha_inicial,
                                       coluna_final, linha_final);
    }

    /* Peca normal: exatamente 1 casa e sempre para frente (U.9) */
    if (abs(diff_linha) != 1) {
        return 0;
    }

    if (diff_linha != direcao_avanco(jogador)) {
        return 0;
    }

    return 1;
}

/*
 * Verifica se a peca que acabou de chegar a (coluna_final, linha_final)
 * deve ser promovida e, em caso afirmativo, realiza a promocao
 * diretamente na matriz:
 *  - 'o' que atinge a linha 9 vira 'O'
 *  - '@' que atinge a linha 0 vira '&'
 * Pecas que ja sao damas, ou que nao atingiram a linha de promocao,
 * permanecem inalteradas.
 */
void verificar_promocao(char tabuleiro[TAM][TAM], int coluna_final, int linha_final) {
    char peca = tabuleiro[linha_final][coluna_final];

    if (peca == 'o' && linha_final == TAM - 1) {
        tabuleiro[linha_final][coluna_final] = 'O';
    } else if (peca == '@' && linha_final == 0) {
        tabuleiro[linha_final][coluna_final] = '&';
    }
}

/*
 * 2.3 - Captura e captura obrigatoriA */

/*
 * Valida uma captura de PECA NORMAL: salto de exatamente 2 casas na
 * diagonal, onde a casa intermediaria contem exatamente uma peca
 * adversaria e a casa de destino esta vazia. Pecas normais podem
 * capturar para frente OU para tras, diferente do movimento
 * simples.
 * Se valida, preenche '*coluna_capturada'/'*linha_capturada' com a
 * posicao da peca adversaria a remover e retorna 1. Caso contrario,
 * retorna 0.
 */
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

    /* A peca do meio precisa ser do ADVERSARIO do jogador da vez */
    if (peca_pertence_ao_jogador(peca_meio, jogador)) {
        return 0;
    }

    *linha_capturada = linha_meio;
    *coluna_capturada = coluna_meio;
    return 1;
}

/*
 * Valida uma captura de DAMA: a dama anda por uma diagonal,
 * o caminho antes da peca adversaria deve estar livre, a peca
 * encontrada deve ser exatamente uma e do adversario, e deve haver
 * pelo menos uma casa vazia imediatamente apos ela na mesma diagonal
 * para a dama poder pousar. O destino informado deve ser uma dessas
 * casas vazias apos a peca capturada (a dama pode escolher onde
 * parar, mas nao pode saltar sobre outra peca).
 * Se valida, preenche '*coluna_capturada'/'*linha_capturada' e
 * retorna 1. Caso contrario, 0.
 */
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
                /* So pode haver UMA peca no caminho, e deve ser
                 * adversaria; senao a captura e invalida */
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
            /* Apos a peca adversaria, todas as casas no caminho
             * (incluindo o destino, verificado fora do loop) devem
             * estar vazias - nao pode saltar sobre mais nada */
            if (!casa_vazia(atual)) {
                return 0;
            }
        }

        linha += passo_linha;
        coluna += passo_coluna;
    }

    /* Encontrou exatamente uma peca adversaria no caminho e a casa
     * de destino precisa estar vazia */
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

/*
 * Valida um movimento de CAPTURA (peao ou dama), delegando para a
 * funcao especifica de acordo com o tipo de peca na origem.
 */
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

/*
 * Retorna 1 se a peca em (coluna, linha) possui pelo menos uma
 * captura possivel, testando as 4 diagonais. Para peca normal, testa
 * o salto fixo de 2 casas em cada diagonal. Para dama, testa todas
 * as distancias possiveis em cada diagonal (ja que o alcance e
 * variavel).
 */
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

/*
 * Retorna 1 se o jogador da vez possui QUALQUER captura disponivel
 * em qualquer uma de suas pecas no tabuleiro. Usado para impor a
 * obrigatoriedade de captura e para combo de capturas
 * .
 */
int existe_captura_obrigatoria(char tabuleiro[TAM][TAM], char jogador) {
    int linha, coluna;

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

/*Validacao geral - ponto de entrada unico para o Pilar 3 */

/*
 * Funcao principal de validacao. Verifica, na ordem:
 *  1. Limites do tabuleiro e casas jogaveis (U.7, U.14.h);
 *  2. Origem == destino (U.14.h);
 *  3. Origem contem peca do jogador da vez (U.14.a);
 *  4. Se ha captura obrigatoria disponivel para o jogador, a jogada
 *     PRECISA ser uma captura valida (U.14.j, U.15);
 *  5. Caso contrario, tenta validar como captura; se nao for uma
 *     captura valida, tenta validar como movimento simples.
 *
 * Retorna JOGADA_INVALIDA, JOGADA_SIMPLES ou JOGADA_CAPTURA. Em caso
 * de captura valida, preenche '*coluna_capturada'/'*linha_capturada'
 * com a posicao da peca adversaria a ser removida pelo Pilar 3.
 */
ResultadoJogada validar_jogada(char tabuleiro[TAM][TAM], int coluna_inicial,
                                int linha_inicial, int coluna_final, int linha_final,
                                char jogador, int *coluna_capturada, int *linha_capturada) {
    /* 1. Limites e casas jogaveis (U.7, U.14.h) */
    if (!posicao_dentro_limites(coluna_inicial, linha_inicial) ||
        !posicao_dentro_limites(coluna_final, linha_final)) {
        return JOGADA_INVALIDA;
    }

    if (!casa_jogavel(tabuleiro, coluna_inicial, linha_inicial) ||
        !casa_jogavel(tabuleiro, coluna_final, linha_final)) {
        return JOGADA_INVALIDA;
    }

    /* 2. Origem nao pode ser igual ao destino (U.14.h) */
    if (linha_inicial == linha_final && coluna_inicial == coluna_final) {
        return JOGADA_INVALIDA;
    }

    /* 3. Origem deve conter peca do jogador da vez (U.14.a) */
    char peca_origem = tabuleiro[linha_inicial][coluna_inicial];
    if (!peca_pertence_ao_jogador(peca_origem, jogador)) {
        return JOGADA_INVALIDA;
    }

    /* 4. Obrigatoriedade de captura (U.14.j, U.15): se HOUVER captura
     * disponivel para o jogador em QUALQUER peca, a jogada atual
     * precisa ser uma captura valida - nao basta ser uma captura
     * com a mesma peca, mas tambem nao pode ser um movimento simples */
    int captura_obrigatoria = existe_captura_obrigatoria(tabuleiro, jogador);

    if (validar_captura(tabuleiro, coluna_inicial, linha_inicial, coluna_final,
                         linha_final, jogador, coluna_capturada, linha_capturada)) {
        return JOGADA_CAPTURA;
    }

    if (captura_obrigatoria) {
        /* Havia captura disponivel em algum lugar do tabuleiro, mas a
         * jogada informada nao foi uma captura valida -> invalida */
        return JOGADA_INVALIDA;
    }

    if (validar_movimento_simples(tabuleiro, coluna_inicial, linha_inicial,
                                   coluna_final, linha_final, jogador)) {
        return JOGADA_SIMPLES;
    }

    return JOGADA_INVALIDA;
}

/* 
 * Fim de partida */

/*
 * Conta quantas pecas (normais + damas) o jogador ainda possui no
 * tabuleiro.
 */
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

/*
 * Retorna 1 se o jogador da vez NAO possui nenhuma jogada legal
 * (nem captura, nem movimento simples) disponivel em nenhuma de suas
 * pecas.
 */
int jogador_sem_jogadas(char tabuleiro[TAM][TAM], char jogador) {
    int linha, coluna;

    /* Se ha captura obrigatoria disponivel, entao ha jogada legal */
    if (existe_captura_obrigatoria(tabuleiro, jogador)) {
        return 0;
    }

    for (linha = 0; linha < TAM; linha++) {
        for (coluna = 0; coluna < TAM; coluna++) {
            char peca = tabuleiro[linha][coluna];

            if (!peca_pertence_ao_jogador(peca, jogador)) {
                continue;
            }

            /* Testa movimento simples nas 4 diagonais adjacentes e,
             * para damas, em qualquer distancia */
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