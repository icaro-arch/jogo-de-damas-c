/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#ifndef REGRAS_H
#define REGRAS_H

#include "damas.h"

/*
 * 2.1 - Validacao de sintaxe e limites */

/* Converte um caractere de coluna ('A'-'J', maiusculo ou minusculo)
 * para o indice correspondente (0-9). Retorna -1 se invalido. */
int converter_coluna(char c);

/* Converte um caractere de linha ('0'-'9') para o indice (0-9).
 * Retorna -1 se invalido. */
int converter_linha(char c);

/* Faz o parsing de uma string no formato "B0--C1" (sem espacos) e
 * preenche, por ponteiro, as 4 coordenadas convertidas. Retorna 1 se
 * o FORMATO esta correto (sintaxe, "--", limites A-J e 0-9), 0 caso
 * contrario. Nao valida regras de jogo aqui, apenas sintaxe/formato. */
int parsear_jogada(const char *entrada, int *coluna_inicial, int *linha_inicial,
                    int *coluna_final, int *linha_final);

/* Verifica se uma coordenada (coluna, linha) esta dentro dos limites
 * do tabuleiro (0 <= coluna <= 9 e 0 <= linha <= 9). */
int posicao_dentro_limites(int coluna, int linha);

/* Verifica se a posicao corresponde a uma casa jogavel (nao '#'). */
int casa_jogavel(char tabuleiro[TAM][TAM], int coluna, int linha);

/*2.2 - Validacao de movimentos comuns e promocao  */

/* Retorna 1 se a peca no tabuleiro pertence ao jogador informado
 * ('C' para topo: 'o'/'O', 'B' para baixo: '@'/'&'). */
int peca_pertence_ao_jogador(char peca, char jogador);

/* Retorna 1 se o caractere representa uma dama */
int eh_dama(char peca);

/* Valida um movimento simples (nao-captura) de uma casa na diagonal,
 * respeitando a direcao permitida para pecas normais e o alcance
 * livre para damas (com caminho 100% livre). Nao verifica obrigacao
 * de captura (isso e feito em outra funcao). */
int validar_movimento_simples(char tabuleiro[TAM][TAM], int coluna_inicial,
                               int linha_inicial, int coluna_final,
                               int linha_final, char jogador);

/* Verifica se a peca que chegou a (coluna_final, linha_final) deve
 * ser promovida (linha 9 para 'o' -> 'O', linha 0 para '@' -> '&') e
 * realiza a promocao diretamente na matriz, se for o caso. */
void verificar_promocao(char tabuleiro[TAM][TAM], int coluna_final, int linha_final);

/*2.3 - Captura e captura obrigatoria */

/* Valida um movimento de captura (peao ou dama) da origem para o
 * destino informados. Se valido, preenche '*coluna_capturada' e
 * '*linha_capturada' com a posicao da peca adversaria que seria
 * removida. Retorna 1 se valido, 0 caso contrario. Nao altera o
 * tabuleiro (apenas verifica). */
int validar_captura(char tabuleiro[TAM][TAM], int coluna_inicial, int linha_inicial,
                     int coluna_final, int linha_final, char jogador,
                     int *coluna_capturada, int *linha_capturada);

/* Retorna 1 se a peca em (coluna, linha) possui pelo menos uma
 * captura possivel a partir da posicao atual (usado tanto para
 * checar obrigatoriedade quanto para combo de capturas U.12). */
int peca_possui_captura(char tabuleiro[TAM][TAM], int coluna, int linha, char jogador);

/* Retorna 1 se o jogador da vez possui QUALQUER captura disponivel
 * em qualquer uma de suas pecas no tabuleiro (varredura completa). */
int existe_captura_obrigatoria(char tabuleiro[TAM][TAM], char jogador);

/*Validacao geral (usada pelo Pilar 3 como ponto de entrada unico)*/

/* Codigos de retorno para indicar o TIPO de jogada validada,
 * permitindo ao Pilar 3 saber se deve tratar como captura
 * (remover peca, manter turno) ou movimento simples (passar turno). */
typedef enum {
    JOGADA_INVALIDA = 0,
    JOGADA_SIMPLES = 1,
    JOGADA_CAPTURA = 2
} ResultadoJogada;

/* Funcao principal de validacao chamada pelo motor do jogo (Pilar 3).
 * Verifica formato, posse da peca, obrigatoriedade de captura e
 * regras de movimento/captura especificas. Se a jogada for uma
 * captura valida, preenche '*coluna_capturada' e '*linha_capturada'
 * com a posicao da peca adversaria a ser removida. Nao altera o
 * tabuleiro. */
ResultadoJogada validar_jogada(char tabuleiro[TAM][TAM], int coluna_inicial,
                                int linha_inicial, int coluna_final, int linha_final,
                                char jogador, int *coluna_capturada, int *linha_capturada);

/*
 * Fim de partida - varreduras */

/* Conta quantas pecas (normais + damas) o jogador ainda possui no
 * tabuleiro. Retorna 0 se nao restam pecas. */
int contar_pecas_jogador(char tabuleiro[TAM][TAM], char jogador);

/* Retorna 1 se o jogador da vez NAO possui nenhuma jogada legal
 * (nem simples, nem captura) disponivel em nenhuma de suas pecas. */
int jogador_sem_jogadas(char tabuleiro[TAM][TAM], char jogador);

// Funcao para encontrar uma jogada valida e sugerir ao jogador - icaro
int obter_sugestao_jogada(char tabuleiro[TAM][TAM], char jogador, char *sugestao_str);

#endif
