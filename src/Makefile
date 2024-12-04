# Kompilátor a přepínače
CC = gcc
CFLAGS = -std=c11

# Název výsledného programu
TARGET = ifj24_compiler

# Seznam zdrojových a hlavičkových souborů
SRC = buffer.c error.c parser.c scanner.c stack.c symtable.c codeStack.c list.c generatorBuf.c generator.c genVarList.c
OBJ = $(SRC:.c=.o)
HEADERS = buffer.h error.h parser.h scanner.h stack.h symtable.h codeStack.h list.h generatorBuf.h generator.h precedence_token.h semantic.h precedence.h genVarList.h

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
<<<<<<< Updated upstream
	zip xpylypd00.zip rozdeleni buffer.c buffer.h error.c error.h generator.c generator.h parser.c parser.h scanner.c scanner.h stack.c stack.h precedence.h symtable.c symtable.h genVarList.h genVarList.c Makefile dokumentace.pdf
=======
	zip xpylypd00.zip rozdeleni rozsireni buffer.c buffer.h codeStack.c codeStack.h error.c error.h generator.c generator.h generatorBuf.h generatorBuf.c list.c list.h Makefile parser.c parser.h precedence_token.h precedence.h scanner.h scanner.c semantic.h stack.h stack.c symtable.c symtable.h genVarList.h genVarList.h dokumentace.pdf
>>>>>>> Stashed changes

# Vytvoření archivu pro pokusné odevzdání
pokusne:
	zip xpylypd00.zip buffer.c buffer.h error.c error.h parser.c parser.h scanner.c scanner.h codeStack.h codeStack.c stack.c stack.h precedence.h symtable.c symtable.h Makefile dokumentace.pdf rozdeleni precedence_token.h semantic.h

# Speciální cíle (aby nebyly zaměněny za soubory)
.PHONY: all clean rebuild zip
