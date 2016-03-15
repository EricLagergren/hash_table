CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -O2
SRC=main.c hash.c
BIN=main

all: $(BIN)

main: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ #-lm

.PHONY: main

clean:
	rm -f *.o $(BIN)