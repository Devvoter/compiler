# Kompilátor a přepínače
CC = gcc
CFLAGS = -std=c11

# Název výsledného programu
TARGET = ifj24_compiler

# Seznam zdrojových a hlavičkových souborů
SRC = buffer.c error.c parser.c scanner.c stack.c symtable.c codeStack.c
OBJ = $(SRC:.c=.o)
HEADERS = buffer.h error.h parser.h scanner.h stack.h precedence.h symtable.h codeStack.h precedence_token.h semantic.h

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
	zip xpylypd00.zip buffer.c buffer.h error.c error.h parser.c parser.h scanner.c scanner.h codeStack.h codeStack.c stack.c stack.h precedence.h symtable.c symtable.h Makefile dokumentace.pdf rozdeleni precedence_token.h semantic.h

# Speciální cíle (aby nebyly zaměněny za soubory)
.PHONY: all clean rebuild zip
