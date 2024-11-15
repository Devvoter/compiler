# Makefile pro test tabulky symbolú

# Kompilátor a příznaky
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

# Výchozí soubor
EXEC = test_symtable

# Zdrojové soubory
SRC = symtable.c test_symtable.c
HEADERS = symtable.h

# Výchozí cíl
all: $(EXEC)

# Vytvoření spustitelného souboru
$(EXEC): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Vyčištení
clean:
	rm -f $(EXEC)

# Překompilování projektu
rebuild: clean all