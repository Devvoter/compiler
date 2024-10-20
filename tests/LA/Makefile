# Kompilátor a příznaky
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = ../../src
OBJ_DIR = obj
INCLUDES = -I$(SRC_DIR)

# Přepínače pro zdroj
ifeq ($(USE_STDIN), 1)
    CFLAGS += -DUSE_STDIN
endif

# Zdrojové soubory
SRC_FILES = $(SRC_DIR)/buffer.c $(SRC_DIR)/scanner.c
TEST_FILES = test_scanner.c
OBJS = $(OBJ_DIR)/buffer.o $(OBJ_DIR)/scanner.o $(OBJ_DIR)/test_scanner.o

# Výchozí cíl
all: test_scanner

# Vytvoření spustitelného souboru
test_scanner: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o test_scanner

# Kompilace zdrojových souborů
$(OBJ_DIR)/buffer.o: $(SRC_DIR)/buffer.c $(SRC_DIR)/buffer.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/buffer.c -o $(OBJ_DIR)/buffer.o

$(OBJ_DIR)/scanner.o: $(SRC_DIR)/scanner.c $(SRC_DIR)/scanner.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/scanner.c -o $(OBJ_DIR)/scanner.o

# Kompilace testovacího souboru
$(OBJ_DIR)/test_scanner.o: $(TEST_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(TEST_FILES) -o $(OBJ_DIR)/test_scanner.o

# Vyčištění (odstranění objektových a spustitelných souborů)
clean:
	rm -rf $(OBJ_DIR) test_scanner
