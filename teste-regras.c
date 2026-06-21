#include "damas.h"
#include "regras.h"

static void montar_tabuleiro_vazio(char t[TAM][TAM]) {
    int i, j;
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            t[i][j] = ((i + j) % 2 == 0) ? '#' : ' ';
        }
    }
}

int main(void) {
    char t[TAM][TAM];
    int coluna_capturada, linha_capturada;
    ResultadoJogada r;

    /* Teste 1: movimento simples de peao 'o' para frente (C avanca +linha) */
    montar_tabuleiro_vazio(t);
    t[1][0] = 'o'; /* A1 (coluna=0, linha=1) */
    r = jogada_valida(t, 0, 1, 1, 2, 'C', &coluna_capturada, &linha_capturada);
    printf("Teste 1 (peao C move 1 casa p/ frente diagonal): %s (esperado SIMPLES=1) -> %d\n",
           r == JOGADA_SIMPLES ? "OK" : "FALHOU", r);

    /* Teste 2: peao nao pode mover para tras sem captura */
    montar_tabuleiro_vazio(t);
    t[2][1] = 'o';
    r = jogada_valida(t, 1, 2, 0, 1, 'C', &coluna_capturada, &linha_capturada);
    printf("Teste 2 (peao C move p/ tras sem captura): %s (esperado INVALIDA=0) -> %d\n",
           r == JOGADA_INVALIDA ? "OK" : "FALHOU", r);

    /* Teste 3: captura de peao - 'o' em (coluna=1,linha=2) captura '@' em
       (coluna=2,linha=3), cai em (coluna=3,linha=4) */
    montar_tabuleiro_vazio(t);
    t[2][1] = 'o';
    t[3][2] = '@';
    r = jogada_valida(t, 1, 2, 3, 4, 'C', &coluna_capturada, &linha_capturada);
    printf("Teste 3 (peao C captura '@'): %s (esperado CAPTURA=2) -> %d, capturada=(col=%d,lin=%d) esperado (2,3)\n",
           r == JOGADA_CAPTURA ? "OK" : "FALHOU", r, coluna_capturada, linha_capturada);

    /* Teste 4: captura obrigatoria - jogador tem captura disponivel mas
       tenta mover outra peca de forma simples -> deve ser invalida */
    montar_tabuleiro_vazio(t);
    t[2][1] = 'o';      /* pode capturar */
    t[3][2] = '@';
    t[5][4] = 'o';      /* outra peca, sem captura disponivel */
    r = jogada_valida(t, 4, 5, 5, 6, 'C', &coluna_capturada, &linha_capturada);
    printf("Teste 4 (captura obrigatoria ignorada): %s (esperado INVALIDA=0) -> %d\n",
           r == JOGADA_INVALIDA ? "OK" : "FALHOU", r);

    /* Teste 5: dama captura a distancia e pode escolher onde parar.
       Diagonal jogavel real: (col=0,lin=1)->(1,2)->(2,3)->(3,4)->(4,5)->(5,6) */
    montar_tabuleiro_vazio(t);
    t[1][0] = 'O';   /* dama de cima em (coluna=0, linha=1) */
    t[4][3] = '@';   /* peca adversaria em (coluna=3, linha=4), mesma diagonal */
    r = jogada_valida(t, 0, 1, 5, 6, 'C', &coluna_capturada, &linha_capturada);
    printf("Teste 5 (dama captura a distancia): %s (esperado CAPTURA=2) -> %d, capturada=(col=%d,lin=%d) esperado (3,4)\n",
           r == JOGADA_CAPTURA ? "OK" : "FALHOU", r, coluna_capturada, linha_capturada);

    /* Teste 6: promocao de peao 'o' ao atingir linha 9 */
    montar_tabuleiro_vazio(t);
    t[9][0] = 'o';
    verificar_promocao(t, 0, 9);
    printf("Teste 6 (promocao 'o'->'O' na linha 9): %s -> peca agora e '%c'\n",
           t[9][0] == 'O' ? "OK" : "FALHOU", t[9][0]);

    /* Teste 7: jogador sem jogadas (bloqueio total genuino).
       Peao 'o' isolado em (coluna=1, linha=8) [B8]. Jogador C avanca
       linha+1, entao os unicos destinos possiveis sao (coluna=0,linha=9)
       e (coluna=2,linha=9). Colocamos pecas PROPRIAS ('o') em ambos,
       bloqueando o avanco simples e impedindo captura (nao se captura
       peca propria). */
    montar_tabuleiro_vazio(t);
    t[8][1] = 'o';
    t[9][0] = 'o';
    t[9][2] = 'o';
    int sem_jogadas = jogador_sem_jogadas(t, 'C');
    printf("Teste 7 (jogador completamente bloqueado): %s -> %d\n",
           sem_jogadas == 1 ? "OK" : "FALHOU", sem_jogadas);

    return 0;
}