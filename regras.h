/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

//Todos os comentários relacionados a explicações estarão no arquivo regras.c para evitar redundância desnecessária;

#ifndef REGRAS_H
#define REGRAS_H

#include "damas.h"

int converter_coluna(char c);

int converter_linha(char c);

int parsear_jogada(const char *entrada, int *coluna_inicial, int *linha_inicial,
                    int *coluna_final, int *linha_final);

int posicao_dentro_limites(int coluna, int linha);

int casa_jogavel(char tabuleiro[TAM][TAM], int coluna, int linha);

int peca_pertence_ao_jogador(char peca, char jogador);

int eh_dama(char peca);

int validar_movimento_simples(char tabuleiro[TAM][TAM], int coluna_inicial,
                               int linha_inicial, int coluna_final,
                               int linha_final, char jogador);

void verificar_promocao(char tabuleiro[TAM][TAM], int coluna_final, int linha_final);

int validar_captura(char tabuleiro[TAM][TAM], int coluna_inicial, int linha_inicial,
                     int coluna_final, int linha_final, char jogador,
                     int *coluna_capturada, int *linha_capturada);


int peca_possui_captura(char tabuleiro[TAM][TAM], int coluna, int linha, char jogador);

int existe_captura_obrigatoria(char tabuleiro[TAM][TAM], char jogador);

typedef enum {
    JOGADA_INVALIDA = 0,
    JOGADA_SIMPLES = 1,
    JOGADA_CAPTURA = 2
} ResultadoJogada;

ResultadoJogada validar_jogada(char tabuleiro[TAM][TAM], int coluna_inicial,
                                int linha_inicial, int coluna_final, int linha_final,
                                char jogador, int *coluna_capturada, int *linha_capturada);

int contar_pecas_jogador(char tabuleiro[TAM][TAM], char jogador);

int jogador_sem_jogadas(char tabuleiro[TAM][TAM], char jogador);

int obter_sugestao_jogada(char tabuleiro[TAM][TAM], char jogador, char *sugestao_str);

#endif
