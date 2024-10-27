/**
 * @file parser.c
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Implementace parseru
 * 
 * @date 2024-10-04
 * 
 */

#include "parser.h"
#include "error.h"

Token CurrentToken;

Token getCurrentToken() {
    Token token = getNextToken();
    CurrentToken = token;
    return token;
}

void parse() {
    if (parse_prolog() != 0) {
        fprintf(stderr, "Syntax error: invalid prologue\n");
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());
    if (token.type != T_EOF) {
        exitWithError(&token, ERR_SYNTAX_ANALYSIS);
    }
}

int parse_prolog() {
    if (getCurrentToken().type != T_CONST ||
        getCurrentToken().type != T_IFJ || 
        getCurrentToken().type != T_ASSIGN ||
        getCurrentToken().type != T_IMPORT ||
        getCurrentToken().type != T_OPEN_PARENTHESES) {
            return 1;
    }
    Token token = getCurrentToken();
    if (token.type != T_STRING_TYPE || (strcmp(token.data.u8, "ifj24.zig") != 0)) {
        return 1;
    }
    if (getCurrentToken().type != T_CLOSE_PARENTHESES || getCurrentToken().type != T_SEMICOLON) {
        return 1;
    }
    return 0;
}

// code() bude volat konkrétní funkci dle typu tokenu
Token code(Token token) {
    switch (token.type) {
        case T_IF:
            return code(parse_if());
        case T_WHILE:
            return code(parse_while());
        case T_VAR:
            return code(parse_variable_definition());
        case T_CONST:
            return code(parse_variable_definition());
        case T_RETURN:
            return code(parse_return());
        case T_ID: // přiřazení nebo volání funkce
            return code(parse_assignment_or_function_call());
        case T_IFJ: // volani vestavene funkce
            return code(parse_standard_function_call());
        case T_PUB:
            return code(parse_function_definition());
        default:
            return token;
    }
}

// Funkce pro parsování IF konstrukce
Token parse_if() {
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    expression();  // Parsování výrazu uvnitř závorek
    if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());  // Parsování těla if-bloku
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }

    // Zpracování volitelného else
    Token token = getCurrentToken();
    if (token.type == T_ELSE) {
        if (getCurrentToken().type != T_OPEN_BRACKET) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        Token token_2 = code(getCurrentToken());  // Parsování else-bloku
        if (token_2.type != T_CLOSE_BRACKET) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        return getCurrentToken();  // Vrátíme token pro další zpracování
    } else {
        return token;  // Vrátíme token pro další zpracování
    }
}

// Funkce pro parsování WHILE konstrukce
Token parse_while() {
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
       exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    //expression();  
    if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());  // Parsování těla while smyčky
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS); 
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

Token parse_variable_definition() {
    Token token = getCurrentToken();
    if (token.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    token = getCurrentToken();
    if (token.type != T_COLON && token.type != T_ASSIGN) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    else if (token.type == T_COLON) {
        if (getCurrentToken().type != T_I32_ID || 
        getCurrentToken().type != T_F64_ID || 
        getCurrentToken().type != T_U8_ID ||
        getCurrentToken().type != T_I32_NULLABLE ||
        getCurrentToken().type != T_F64_NULLABLE ||
        getCurrentToken().type != T_U8_NULLABLE) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_ASSIGN) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    }
    expression();  // Parsování výrazu na pravé straně přiřazení
    if (getCurrentToken().type != T_SEMICOLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

Token parse_return() {
    expression();  // Parsování výrazu, který se má vrátit
    if (getCurrentToken().type != T_SEMICOLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

Token parse_assignment_or_function_call() {
    Token token = getCurrentToken();
    if (token.type != T_ASSIGN && token.type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (token.type == T_ASSIGN) {
        expression();  // Parsování výrazu na pravé straně přiřazení
        if (getCurrentToken().type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    } else {
        // Parsování volání funkce
        function();
        if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (getCurrentToken().type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

Token parse_standard_function_call() {
    if (getCurrentToken().type != T_DOT) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = getCurrentToken();
    if (token.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (!strcmp(token.data.u8, "readstr") &&
        !strcmp(token.data.u8, "readi32") &&
        !strcmp(token.data.u8, "readf64") &&
        !strcmp(token.data.u8, "write") &&
        !strcmp(token.data.u8, "i2f") &&
        !strcmp(token.data.u8, "f2i") &&
        !strcmp(token.data.u8, "string") &&
        !strcmp(token.data.u8, "length") &&
        !strcmp(token.data.u8, "concat") &&
        !strcmp(token.data.u8, "substrin") &&
        !strcmp(token.data.u8, "strcmp") &&
        !strcmp(token.data.u8, "ord") &&
        !strcmp(token.data.u8, "chr")) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (!strcmp(token.data.u8, "write") ||
        !strcmp(token.data.u8, "i2f") ||
        !strcmp(token.data.u8, "f2i") ||
        !strcmp(token.data.u8, "string") ||
        !strcmp(token.data.u8, "length") ||
        !strcmp(token.data.u8, "ord") ||
        !strcmp(token.data.u8, "chr")) {
        if (getCurrentToken().type != T_OPEN_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        expression();  // Parsování argumentu funkce
        if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    } else if (!strcmp(token.data.u8, "concat") || !strcmp(token.data.u8, "substrin") || !strcmp(token.data.u8, "strcmp")) {
        if (getCurrentToken().type != T_OPEN_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        expression();  // Parsování prvního argumentu funkce
        if (getCurrentToken().type != T_COMMA) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        expression();  // Parsování druhého argumentu funkce
        if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        // add optional comma?
    }
    else {
        if (getCurrentToken().type != T_OPEN_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    if (getCurrentToken().type != T_SEMICOLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

void arguments(Token token) {
    if (token.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_COLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    if (token.type != T_I32_ID && 
        token.type != T_F64_ID && 
        token.type != T_U8_ID &&
        token.type != T_I32_NULLABLE &&
        token.type != T_F64_NULLABLE &&
        token.type != T_U8_NULLABLE) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    while (getCurrentToken().type != T_CLOSE_PARENTHESES) {
        if (getCurrentToken().type != T_COMMA) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        getCurrentToken();
        if (CurrentToken.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else if (CurrentToken.type != T_ID) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (getCurrentToken().type != T_COLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (token.type != T_I32_ID && 
            token.type != T_F64_ID && 
            token.type != T_U8_ID &&
            token.type != T_I32_NULLABLE &&
            token.type != T_F64_NULLABLE &&
            token.type != T_U8_NULLABLE) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
}

Token parse_function_definition() {
    if (getCurrentToken().type != T_FN) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
        arguments(CurrentToken);  // Parsování argumentů funkce
    }
    if (getCurrentToken().type != T_I32_ID || 
        getCurrentToken().type != T_F64_ID || 
        getCurrentToken().type != T_U8_ID ||
        getCurrentToken().type != T_I32_NULLABLE ||
        getCurrentToken().type != T_F64_NULLABLE ||
        getCurrentToken().type != T_U8_NULLABLE) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    code(getCurrentToken());  // Parsování těla funkce
    if (getCurrentToken().type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

int main() {
    parse();  // Spustí se syntaktická analýza
    return 0;
}