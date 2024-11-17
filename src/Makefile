# Kompilátor a přepínače
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

# Název výsledného programu
TARGET = ifj24_compiler

# Seznam zdrojových a hlavičkových souborů
SRC = buffer.c error.c generator.c parser.c scanner.c stack.c symtable.c
OBJ = $(SRC:.c=.o)
HEADERS = buffer.h error.h generator.h parser.h scanner.h stack.h precedence.h symtable.h

# Výchozí cíl - kompilace celého projektu
all: $(TARGET)

# Sestavení výsledného programu
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Překlad jednotlivých objektových souborů
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Vyčištění objektových souborů a výsledného programu
clean:
	rm -f $(OBJ) $(TARGET)

# Překompilování projektu
rebuild: clean all

# Vytvoření archivu pro odevzdání
zip:
	zip xpylypd00.zip rozdeleni buffer.c buffer.h error.c error.h generator.c generator.h parser.c parser.h scanner.c scanner.h stack.c stack.h precedence.h symtable.c symtable.h Makefile dokumentace.pdf

# Vytvoření archivu pro pokusné odevzdání
pokusne:
	zip xpylypd00.zip buffer.c buffer.h error.c error.h generator.c generator.h parser.c parser.h scanner.c scanner.h stack.c stack.h precedence.h symtable.c symtable.h Makefile

# Speciální cíle (aby nebyly zaměněny za soubory)
.PHONY: all clean rebuild zip
