# =====================================================
# Makefile template para estudos de C
# Uso: copie para a pasta do exercício e renomeie para 'Makefile'
# =====================================================

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g -O0
LDFLAGS =

SRC = $(wildcard *.c)
BIN = $(SRC:.c=)

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run-%: %
	./$

memcheck-%: %
	valgrind --leak-check=full --show-leak-kinds=all ./$

debug-%: %
	gdb ./$

clean:
	rm -f $(BIN)

.PHONY: all clean
