/* Nomes e Matriculas da equipe:
PAULO ICARO MATIAS FRANCO - Matricula: 601549
DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643
*/

#ifndef DAMAS_H
#define DAMAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Tamanho do tabuleiro
#define TAM 10


// Prototipos das funcoes
// "char tabuleiro[TAM][TAM]" eh a matriz do tabuleiro
void inicializar_tabuleiro(char tabuleiro[TAM][TAM]);
void imprimir_tabuleiro(char tabuleiro[TAM][TAM]);
void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM], char primeiro_jogador);
void modo_offline(char tabuleiro[TAM][TAM], const char *nome_arquivo);
int validar_jogada(char jogada[]);



#endif


