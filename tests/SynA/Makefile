CC=gcc
CFLAGS=-Wall -std=c11 -g
SRC_DIR = ../../src
FILES=$(SRC_DIR)/parser.c $(SRC_DIR)/scanner.c $(SRC_DIR)/stack.c $(SRC_DIR)/buffer.c $(SRC_DIR)/error.c

.PHONY: test clean

all: test

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test