CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -O2
SRC=hash.c
BIN=main

all: $(BIN)

main: $(SRC)
	$(CC) $(CFLAGS) main.c $^ -o $@ #-lm

lib: $(SRC)
	$(CC) $(CFLAGS) -O -g -c $^

test: $(SRC)
	$(CC) $(CFLAGS) testing.c $^ -o $@ -lm

.PHONY: main lib test

clean:
	rm -f *.o $(BIN)