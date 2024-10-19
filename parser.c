/**
 * @file parser.c
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Implementace parseru
 * 
 * @date 2024-10-04
 * 
 */

enum ErrorCodes {
    ERROR_LEXICAL = 1,
    ERROR_SYNTAX = 2,
    ERROR_NOT_DEFINED_FUNC = 3,
    ERROR_FUNC_WRONG_TYPE = 4,
    ERROR_REDEFINITION = 5,
    ERROR_RETURN = 6,
    ERROR_WRONG_DATA_TYPE = 7,
    ERROR_UNDEFINED_TYPE = 8,
    ERROR_NOT_USED_VAR = 9,
    ERROR_SEMANTIC = 10,
    ERROR_INTERNAL = 99,
};

#include "parser.h"
#include "scanner.h"

void parse() {
    if (parse_prolog() != 0) {
        return ERROR_SYNTAX;
    }
    Token token = getNextToken();
    while (token.type != T_EOF) {
        Token new_token = analyze(token);
        token = new_token;
    }
}

int parse_prolog() {
    int return_code = 0;
    if (getNextToken().type != T_CONST ||
        getNextToken().type != T_IFJ || 
        getNextToken().type != T_ASSIGN ||
        getNextToken().type != T_IMPORT ||
        getNextToken().type != T_OPEN_PARENTHESES) {
            return_code = 1;
    }
    Token token = getNextToken();
    if (token.type != T_STRING_TYPE || (strcmp(token.data, "ifj24.zig") != 0)) {
        return_code = 1;
    }
    if (getNextToken().type != T_CLOSE_PARENTHESES || getNextToken().type != T_SEMICOLON) {
        return_code = 1;
    }

    return return_code;
}

// analyze() bude volat konkrétní funkci dle typu tokenu
Token analyze(Token token) {
    switch (token.type) {
        case T_IF:
            return parse_if();
        case T_WHILE:
            return parse_while();
        case T_VAR:
            return parse_variable_definition();
        case T_CONST:
            return parse_variable_definition();
        case T_RETURN:
            parse_return();
            break;
        case T_ID: // Could be an assignment or a function call
            parse_assignment_or_function_call();
            break;
        case T_IFJ: // Standard function calls (starting with "ifj")
            parse_standard_function_call();
            break;
        case T_PUB: // Function definition starts with "pub fn"
            parse_function_definition();
            break;
        case T_EOF:
            return token;
        default:
            // Chyba: neočekávaný token
            break;
    }
}

// Funkce pro parsování IF konstrukce
Token parse_if() {
    if (getNextToken().type != T_OPEN_PARENTHESES) {
        // chyba: očekávané otevření závorek
    }
    expression();  // Parsování výrazu uvnitř závorek
    if (getNextToken().type != T_CLOSE_PARENTHESES) {
        // chyba: očekávané uzavření závorek
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        // chyba: očekávané otevření bloku
    }
    analyze(getNextToken());  // Parsování těla if-bloku
    if (getNextToken().type != T_CLOSE_BRACKET) {
        // chyba: očekávané uzavření bloku
    }

    // Zpracování volitelného else
    Token token = getNextToken();
    if (token.type == T_ELSE) {
        if (getNextToken().type != T_OPEN_BRACKET) {
            // chyba: očekávané otevření bloku
        }
        analyze(getNextToken());  // Parsování else-bloku
        if (getNextToken().type != T_CLOSE_BRACKET) {
            // chyba: očekávané uzavření else bloku
        }
        return getNextToken();  // Vrátíme token pro další zpracování
    } else {
        return token;  // Vrátíme token pro další zpracování
    }
}

// Funkce pro parsování WHILE konstrukce
Token parse_while() {
    if (getNextToken().type != T_OPEN_PARENTHESES) {
        // chyba: očekávané otevření závorek
    }
    //expression();  // Parsování podmínky ve while smyčce
    if (getNextToken().type != T_CLOSE_PARENTHESES) {
        // chyba: očekávané uzavření závorek
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        // chyba: očekávané otevření bloku
    }
    analyze(getNextToken());  // Parsování těla while smyčky
    if (getNextToken().type != T_CLOSE_BRACKET) {
        // chyba: očekávané uzavření bloku
    }
}

Token parse_variable_definition() {
    Token token = getNextToken();
    if (token.type != T_ID) {
        // chyba: očekáváno jméno proměnné
    }
    Token token = getNextToken();
    if (token.type != T_COLON && token.type != T_ASSIGN) {
        // chyba: očekáváno dvojtečka nebo operátor přiřazení
    }
    if (token.type == T_COLON) {
        if (getNextToken().type != T_I32_ID || 
        getNextToken().type != T_F64_ID || 
        getNextToken().type != T_U8_ID ||
        getNextToken().type != T_I32_NULLABLE ||
        getNextToken().type != T_F64_NULLABLE ||
        getNextToken().type != T_U8_NULLABLE) {
        // chyba: očekáván typ proměnné
        }
        if (getNextToken().type != T_ASSIGN) {
        // chyba: očekáván operátor přiřazení
        }
    }
    expression();  // Parsování výrazu na pravé straně přiřazení
    if (getNextToken().type != T_SEMICOLON) {
        // chyba: očekáván středník
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_return() {
    expression();  // Parsování výrazu, který se má vrátit
    if (getNextToken().type != T_SEMICOLON) {
        // chyba: očekáván středník
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_assignment_or_function_call() {
    Token token = getNextToken();
    if (token.type != T_ASSIGN && token.type != T_OPEN_PARENTHESES) {
        // chyba: očekáván operátor přiřazení nebo závorka
    }
    if (token.type == T_ASSIGN) {
        expression();  // Parsování výrazu na pravé straně přiřazení
        if (getNextToken().type != T_SEMICOLON) {
            // chyba: očekáván středník
        }
    } else {
        // Parsování volání funkce
        function();
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            // chyba: očekávána závorka
        }
        if (getNextToken().type != T_SEMICOLON) {
            // chyba: očekáván středník
        }
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_standard_function_call() {
    if (getNextToken().type != T_DOT) {
        // chyba: očekávána tečka
    }
    Token token = getNextToken();
    if (token.type != T_ID) {
        // chyba: očekáváno jméno funkce
    }
    if (!strcmp(token.data, "readstr") &&
        !strcmp(token.data, "readi32") &&
        !strcmp(token.data, "readf64") &&
        !strcmp(token.data, "write") &&
        !strcmp(token.data, "i2f") &&
        !strcmp(token.data, "f2i") &&
        !strcmp(token.data, "string") &&
        !strcmp(token.data, "length") &&
        !strcmp(token.data, "concat") &&
        !strcmp(token.data, "substrin") &&
        !strcmp(token.data, "strcmp") &&
        !strcmp(token.data, "ord") &&
        !strcmp(token.data, "chr")) {
        // chyba: neznámá funkce
    }
    if (!strcmp(token.data, "write") ||
        !strcmp(token.data, "i2f") ||
        !strcmp(token.data, "f2i") ||
        !strcmp(token.data, "string") ||
        !strcmp(token.data, "length") ||
        !strcmp(token.data, "ord") ||
        !strcmp(token.data, "chr")) {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            // chyba: očekávána závorka
        }
        expression();  // Parsování argumentu funkce
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            // chyba: očekávána závorka
        }
    } else if (!strcmp(token.data, "concat") || !strcmp(token.data, "substrin") || !strcmp(token.data, "strcmp")) {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            // chyba: očekávána závorka
        }
        expression();  // Parsování prvního argumentu funkce
        if (getNextToken().type != T_COMMA) {
            // chyba: očekávána čárka
        }
        expression();  // Parsování druhého argumentu funkce
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            // chyba: očekávána závorka
        }
        // add optional comma?
    }
    else {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            // chyba: očekávána závorka
        }
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            // chyba: očekávána závorka
        }
    }
    if (getNextToken().type != T_SEMICOLON) {
        // chyba: očekáván středník
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

void arguments(Token token) {
    if (getNextToken().type != T_ID) {
        // chyba: očekáváno jméno argumentu
    }
    if (getNextToken().type != T_COLON) {
        // chyba: očekáváno dvojtečka
    }
    if (token.type != T_I32_ID && 
        token.type != T_F64_ID && 
        token.type != T_U8_ID &&
        token.type != T_I32_NULLABLE &&
        token.type != T_F64_NULLABLE &&
        token.type != T_U8_NULLABLE) {
        // chyba: očekáván typ argumentu
    }
    while (getNextToken().type != T_CLOSE_PARENTHESES) {
        if (getNextToken().type != T_COMMA) {
            // chyba: očekávána čárka
        }
        Token token = getNextToken();
        if (token.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else if (token.type != T_ID) {
            // chyba: očekáváno jméno argumentu
        }
        if (getNextToken().type != T_COLON) {
            // chyba: očekáváno dvojtečka
        }
        if (token.type != T_I32_ID && 
            token.type != T_F64_ID && 
            token.type != T_U8_ID &&
            token.type != T_I32_NULLABLE &&
            token.type != T_F64_NULLABLE &&
            token.type != T_U8_NULLABLE) {
            // chyba: očekáván typ argumentu
        }
    }
}

Token parse_function_definition() {
    if (getNextToken().type != T_FN) {
        // chyba: očekáváno "fn"
    }
    Token token = getNextToken();
    if (token.type != T_ID) {
        // chyba: očekáváno jméno funkce
    }
    if (getNextToken().type != T_OPEN_PARENTHESES) {
        // chyba: očekávána závorka
    }
    Token token = getNextToken();
    if (token.type != T_CLOSE_PARENTHESES) {
        arguments(token);  // Parsování argumentů funkce
    }
    if (getNextToken().type != T_I32_ID || 
        getNextToken().type != T_F64_ID || 
        getNextToken().type != T_U8_ID ||
        getNextToken().type != T_I32_NULLABLE ||
        getNextToken().type != T_F64_NULLABLE ||
        getNextToken().type != T_U8_NULLABLE) {
        // chyba: očekáván typ návratové hodnoty
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        // chyba: očekáváno otevření bloku
    }
    analyze(getNextToken());  // Parsování těla funkce
    if (getNextToken().type != T_CLOSE_BRACKET) {
        // chyba: očekáváno uzavření bloku
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

// Hlavní program
int main() {
    parse();  // Spustí se parsování
    return 0;
}