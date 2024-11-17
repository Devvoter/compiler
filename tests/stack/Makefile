# Makefile for stack implementation and test program

# Kompilátor a příznaky
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC_DIR = ../../src
OBJ_DIR = obj
INCLUDES = -I$(SRC_DIR)

# Zdrojové soubory
SRCS = $(SRC_DIR)/stack.c test_stack.c
OBJS = $(OBJ_DIR)/stack.o $(OBJ_DIR)/test_stack.o

# Výchozí cíl
all: test_stack

# Vytvoření spustitelného souboru
test_stack: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o test_stack

# Kompilace zdrojových souborů
$(OBJ_DIR)/stack.o: $(SRC_DIR)/stack.c $(SRC_DIR)/stack.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/stack.c -o $(OBJ_DIR)/stack.o

$(OBJ_DIR)/test_stack.o: test_stack.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c test_stack.c -o $(OBJ_DIR)/test_stack.o

# Vyčištění 
clean:
	rm -rf $(OBJ_DIR) test_stack

.PHONY: all clean
