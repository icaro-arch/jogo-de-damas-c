# Makefile - Jogo de Damas (CK0211 - 2026.1)
# Responsavel: Integrante 3 (Pilar 3 / Main)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Nome do executavel final, exigido pelo enunciado
EXEC = damas

# Lista de arquivos objeto, um por modulo .c do projeto
OBJS = main.o tabuleiro.o regras.o

# Regra padrao: "make" sem argumentos constroi o executavel
all: $(EXEC)

# Linkagem final: junta todos os .o no executavel
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Cada .c vira um .o. O make ja sabe compilar regras.c -> regras.o
# sozinho por causa da regra implicita, mas declarar as dependencias
# de header explicitamente garante recompilacao quando um .h muda.
main.o: main.c damas.h regras.h
	$(CC) $(CFLAGS) -c main.c

tabuleiro.o: tabuleiro.c damas.h
	$(CC) $(CFLAGS) -c tabuleiro.c

regras.o: regras.c regras.h damas.h
	$(CC) $(CFLAGS) -c regras.c

# Limpa os arquivos gerados (uteis para reentregar sem .o/.exe no zip)
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean