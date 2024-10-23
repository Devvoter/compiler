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

void unexpected_token() {
    fprintf(stderr, "Syntax error: unexpected token\n");
    exit(ERROR_SYNTAX);
}

void parse() {
    if (parse_prolog() != 0) {
        fprintf(stderr, "Syntax error: invalid prologue\n");
        exit(ERROR_SYNTAX);
    }
    Token token = getNextToken();
    while (token.type != T_EOF) {
        Token new_token = code(token);
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

// code() bude volat konkrétní funkci dle typu tokenu
Token code(Token token) {
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
        case T_PUB:
            parse_function_definition();
            break;
        case T_EOF:
            return token;
        default:
            fprintf(stderr, "Syntax error: unexpected token\n");
            exit(ERROR_SYNTAX);
            break;
    }
}

// Funkce pro parsování IF konstrukce
Token parse_if() {
    if (getNextToken().type != T_OPEN_PARENTHESES) {
        unexpected_token();
    }
    expression();  // Parsování výrazu uvnitř závorek
    if (getNextToken().type != T_CLOSE_PARENTHESES) {
        unexpected_token();
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        unexpected_token();
    }
    code(getNextToken());  // Parsování těla if-bloku
    if (getNextToken().type != T_CLOSE_BRACKET) {
        unexpected_token();
    }

    // Zpracování volitelného else
    Token token = getNextToken();
    if (token.type == T_ELSE) {
        if (getNextToken().type != T_OPEN_BRACKET) {
            unexpected_token();
        }
        code(getNextToken());  // Parsování else-bloku
        if (getNextToken().type != T_CLOSE_BRACKET) {
            unexpected_token();
        }
        return getNextToken();  // Vrátíme token pro další zpracování
    } else {
        return token;  // Vrátíme token pro další zpracování
    }
}

// Funkce pro parsování WHILE konstrukce
Token parse_while() {
    if (getNextToken().type != T_OPEN_PARENTHESES) {
       unexpected_token();
    }
    //expression();  
    if (getNextToken().type != T_CLOSE_PARENTHESES) {
        unexpected_token();
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        unexpected_token();
    }
    code(getNextToken());  // Parsování těla while smyčky
    if (getNextToken().type != T_CLOSE_BRACKET) {
        unexpected_token(); 
    }
}

Token parse_variable_definition() {
    Token token = getNextToken();
    if (token.type != T_ID) {
        unexpected_token();
    }
    Token token = getNextToken();
    if (token.type != T_COLON && token.type != T_ASSIGN) {
        unexpected_token();
    }
    if (token.type == T_COLON) {
        if (getNextToken().type != T_I32_ID || 
        getNextToken().type != T_F64_ID || 
        getNextToken().type != T_U8_ID ||
        getNextToken().type != T_I32_NULLABLE ||
        getNextToken().type != T_F64_NULLABLE ||
        getNextToken().type != T_U8_NULLABLE) {
        unexpected_token();
    }
        if (getNextToken().type != T_ASSIGN) {
        unexpected_token();
    }
    }
    expression();  // Parsování výrazu na pravé straně přiřazení
    if (getNextToken().type != T_SEMICOLON) {
        unexpected_token();
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_return() {
    expression();  // Parsování výrazu, který se má vrátit
    if (getNextToken().type != T_SEMICOLON) {
        unexpected_token();
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_assignment_or_function_call() {
    Token token = getNextToken();
    if (token.type != T_ASSIGN && token.type != T_OPEN_PARENTHESES) {
        unexpected_token();
    }
    if (token.type == T_ASSIGN) {
        expression();  // Parsování výrazu na pravé straně přiřazení
        if (getNextToken().type != T_SEMICOLON) {
            unexpected_token();
        }
    } else {
        // Parsování volání funkce
        function();
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            unexpected_token();
        }
        if (getNextToken().type != T_SEMICOLON) {
            unexpected_token();
        }
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

Token parse_standard_function_call() {
    if (getNextToken().type != T_DOT) {
        unexpected_token();
    }
    Token token = getNextToken();
    if (token.type != T_ID) {
        unexpected_token();
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
        unexpected_token();
    }
    if (!strcmp(token.data, "write") ||
        !strcmp(token.data, "i2f") ||
        !strcmp(token.data, "f2i") ||
        !strcmp(token.data, "string") ||
        !strcmp(token.data, "length") ||
        !strcmp(token.data, "ord") ||
        !strcmp(token.data, "chr")) {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            unexpected_token();
        }
        expression();  // Parsování argumentu funkce
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            unexpected_token();
        }
    } else if (!strcmp(token.data, "concat") || !strcmp(token.data, "substrin") || !strcmp(token.data, "strcmp")) {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            unexpected_token();
        }
        expression();  // Parsování prvního argumentu funkce
        if (getNextToken().type != T_COMMA) {
            unexpected_token();
        }
        expression();  // Parsování druhého argumentu funkce
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            unexpected_token();
        }
        // add optional comma?
    }
    else {
        if (getNextToken().type != T_OPEN_PARENTHESES) {
            unexpected_token();
        }
        if (getNextToken().type != T_CLOSE_PARENTHESES) {
            unexpected_token();
        }
    }
    if (getNextToken().type != T_SEMICOLON) {
        unexpected_token();
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

void arguments(Token token) {
    if (getNextToken().type != T_ID) {
        unexpected_token();
    }
    if (getNextToken().type != T_COLON) {
        unexpected_token();
    }
    if (token.type != T_I32_ID && 
        token.type != T_F64_ID && 
        token.type != T_U8_ID &&
        token.type != T_I32_NULLABLE &&
        token.type != T_F64_NULLABLE &&
        token.type != T_U8_NULLABLE) {
        unexpected_token();
    }
    while (getNextToken().type != T_CLOSE_PARENTHESES) {
        if (getNextToken().type != T_COMMA) {
            unexpected_token();
        }
        Token token = getNextToken();
        if (token.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else if (token.type != T_ID) {
            unexpected_token();
        }
        if (getNextToken().type != T_COLON) {
            unexpected_token();
        }
        if (token.type != T_I32_ID && 
            token.type != T_F64_ID && 
            token.type != T_U8_ID &&
            token.type != T_I32_NULLABLE &&
            token.type != T_F64_NULLABLE &&
            token.type != T_U8_NULLABLE) {
            unexpected_token();
        }
    }
}

Token parse_function_definition() {
    if (getNextToken().type != T_FN) {
        unexpected_token();
    }
    Token token = getNextToken();
    if (token.type != T_ID) {
        unexpected_token();
    }
    if (getNextToken().type != T_OPEN_PARENTHESES) {
        unexpected_token();
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
        unexpected_token();
    }
    if (getNextToken().type != T_OPEN_BRACKET) {
        unexpected_token();
    }
    code(getNextToken());  // Parsování těla funkce
    if (getNextToken().type != T_CLOSE_BRACKET) {
        unexpected_token();
    }
    return getNextToken();  // Vrátíme token pro další zpracování
}

// Hlavní program
int main() {
    parse();  // Spustí se parsování
    return 0;
}