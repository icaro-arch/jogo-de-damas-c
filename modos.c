#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "damas.h"

//Funcao para o modo de usuario contra usuario
void modo_usuario_contra_usuario(char tabuleiro[TAM][TAM]) {
  
  char jogador_da_vez; // identifica apartir da entrada de texto qual sera o jogador (C ou B)
  char jogadas[100]; // armazenar jogadas apartir da entrada de texto (formato: “B0--C1” e “I7--H6”)

  printf("Qual jogador fara a primeira jogada? (Digite: C para o de cima e B para o de baixo);
  scanf("%c", jogador_da_vez"); //entrada para o jogador que vai iniciar a partida






}
//Funcao para o modo offline   
void modo_offline(char tabuleiro[TAM][TAM], const char *arquivo_com_comandos) {
    
  FILE *arquivo = fopen(arquivo_com_comandos, "r"); //abre o arquivo
  if(!arquivo) {
    return 1; //retorna a funcao caso o arquivo nao exista ou caso de erro ao abrir
  }
  
  char jogador_da_vez; //jogador indicado no arquivo par comecar
  fscanf(arquivo, "%c", &jogador_da_vez); //le o caractere na primeira linha do arquivo "C" ou "B"
  



  
}
