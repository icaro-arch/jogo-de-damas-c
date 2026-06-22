# Nomes e Matriculas da equipe:
# PAULO ICARO MATIAS FRANCO - Matricula: 601549
# DAVI OSUGI ALENCAR FERREIRA GOMES - Matricula: 604474
# VICTOR GABRIEL SANTOS MOREIRA - Matricula: 605643

# Makefile - Jogo de Damas (CK0211 - 2026.1)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Nome do executavel final
EXEC = damas

# Lista de arquivos objeto, um por modulo .c do projeto
OBJS = main.o tabuleiro.o regras.o

# Regra padrao: "make" sem argumentos constroi o executavel
all: $(EXEC)

# Linkagem final: junta todos os .o no executavel
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Cada .c vira um .o.
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
