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
#include "stack_precedence.h"
#include "precedence.h"

Token CurrentToken;

Token getCurrentToken() {
    Token token = getNextToken();
    CurrentToken = token;
    return token;
}

void syntax_analysis() {
    if (parse_prolog() != 0) {
        fprintf(stderr, "Syntax error: invalid prologue\n");
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());
    if (token.type != T_EOF) {
        exitWithError(&token, ERR_SYNTAX_ANALYSIS);
    }
}

void expression() {
    Stack stack;
    S_Init(&stack);
    Token Semicolon;
    Semicolon.type = T_SEMICOLON;
    PrecedenceToken bottom = tokenWrapper(Semicolon);
    S_Push(&stack, bottom);
    int parentheses = 0;        // pokud pocitadlo zavorek = -1, jsme na konci vyrazu
    bool endToken = false;      // posledni token
    bool exprEnd = false;       // konec vyrazu
    Token next_token;
    bool alreadyRead = false;

    // pokud jsme uz precetli prvni token vyrazu, zpracujeme ho
    if (CurrentToken.type != T_ASSIGN) {
        next_token = CurrentToken;
    }
    else {
        next_token = getCurrentToken();
    }

    while(!exprEnd) {
        // PrecedenceToken *topToken = S_Top(&stack);
        // printf("TopToken: %d\n", topToken->isTerminal);
        if (S_IsEmpty(&stack)) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        PrecedenceToken *tokenTop = S_getTopTerminal(&stack);           // zjistime posledni terminal na zasobniku
        // pokud na zasobniku neni zadny terminal, overujeme zda jsme narazili na konec vyrazu
        if (tokenTop == NULL) {
            exprEnd = checkExprEnd(&stack);
            break;
        }
        if (alreadyRead == false && endToken == false) {                                             // pokud nenarazime na posledni token ve vyrazu overujeme zda precteny token je posledni
            if (next_token.type == T_OPEN_PARENTHESES) {
                PrecedenceToken *function_call_check = S_getTopTerminal(&stack);
                if (function_call_check->token.type == T_ID) {              // pokud pred zavorkou je ID, jedna se o volani funkce
                    parse_function_call();                                  // prepneme se na ll1 analyzu
                    next_token = getCurrentToken();
                    alreadyRead = false;
                    continue;                                               // na zasobniku zustane pouze id funkce a nikoliv zavorka a jeji argumenty
                }
                parentheses++;                                              // pokud narazime na token "(", zvysime pocitadlo zavorek 
            } else if (next_token.type == T_CLOSE_PARENTHESES) {            // pokud narazime na token ")", zmensime pocitadlo zavorek
                parentheses--;
            }
            if (parentheses < 0 || next_token.type == T_SEMICOLON || next_token.type == T_COMMA || next_token.type == T_OPEN_BRACKET) { // pokud je pocitadlo zavorek mensi nez 0, jedna se o konec vyrazu
                endToken = true;
                next_token.type = T_SEMICOLON;
            }
        }
        else {
            if (next_token.type == T_SEMICOLON) {                      // overime zda mame zredukovany cely vyraz
                if(checkExprEnd(&stack)) {
                    break;
                }
            }
        }
        
        // porovnavame priority posledniho terminalu na zasobniku s prave nactenym tokenem
        if (getOperatorIndex(next_token)==-1) {
            exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
        }
        char relation = precedenceTable[getOperatorIndex(tokenTop->token)][getOperatorIndex(next_token)];
        if (relation==EMPTY) {
            exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
        }
        if (relation == '>') {
            if (S_topTerminalReduce(&stack)) {       // vrati znak pred poslednim terminalem na zasobniku pro overeni redukce
                ruleReduce(&stack);
                alreadyRead = true;
                // PrecedenceToken *topToken = S_Top(&stack);
                // printf("TopToken: %d\n", topToken->isTerminal);
                continue;                                       // pokud lze provest redukci, pokracujeme dal se stejnym tokenem na vstupu
            }
            else {
                exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
            }
        }
        else if (relation == '=') {
            PrecedenceToken pushToken = tokenWrapper(next_token);
            S_Push(&stack, pushToken);
        } 
        else if (relation == '<') {
            tokenTop->reduction = true;
            PrecedenceToken pushToken = tokenWrapper(next_token);
            S_Push(&stack, pushToken);
        } 
        next_token = getCurrentToken();
        alreadyRead = false;
    }
}

int parse_prolog() {
    if (getCurrentToken().type != T_CONST) {
        return 1;
    }
    if (getCurrentToken().type != T_IFJ) {
        return 1;
    }
    if (getCurrentToken().type != T_ASSIGN) {
        return 1;
    }
    if (getCurrentToken().type != T_AT) {
        return 1;
    }
    if (getCurrentToken().type != T_IMPORT) {
        return 1;
    }
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        return 1;
    }
    Token token = getCurrentToken();
    if (token.type != T_STRING_TYPE || (strcmp(token.data.u8->data, "ifj24.zig") != 0)) {
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
    getCurrentToken();
    expression();  // Parsování výrazu uvnitř závorek
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
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
    token = getCurrentToken();
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
    getCurrentToken();
    expression();  
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
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
        token = getCurrentToken();
        if (token.type != T_I32_ID && 
        token.type != T_F64_ID &&
        token.type != T_U8_ID &&
        token.type != T_I32_NULLABLE &&
        token.type != T_F64_NULLABLE &&
        token.type != T_U8_NULLABLE) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (getCurrentToken().type != T_ASSIGN) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    if (getCurrentToken().type == T_STRING_TYPE) {
        if (getCurrentToken().type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        return getCurrentToken();  // Vrátíme token pro další zpracování
    }
    expression();  // Parsování výrazu na pravé straně přiřazení
    if (CurrentToken.type != T_SEMICOLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

Token parse_return() {
    getCurrentToken();
    if (CurrentToken.type == T_SEMICOLON) {
        return getCurrentToken();  // Vrátíme token pro další zpracování
    }
    else {
        expression();  // Parsování výrazu, který se má vrátit
        if (CurrentToken.type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        return getCurrentToken();  // Vrátíme token pro další zpracování
    }
}

Token parse_assignment_or_function_call() {
    Token token = getCurrentToken();
    if (token.type != T_ASSIGN && token.type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (token.type == T_ASSIGN) {
        expression();  // Parsování výrazu na pravé straně přiřazení
        if (CurrentToken.type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    else {
        parse_function_call();
        if (CurrentToken.type != T_CLOSE_PARENTHESES) {
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
    if (!strcmp(token.data.u8->data, "readstr") &&
        !strcmp(token.data.u8->data, "readi32") &&
        !strcmp(token.data.u8->data, "readf64") &&
        !strcmp(token.data.u8->data, "write") &&
        !strcmp(token.data.u8->data, "i2f") &&
        !strcmp(token.data.u8->data, "f2i") &&
        !strcmp(token.data.u8->data, "string") &&
        !strcmp(token.data.u8->data, "length") &&
        !strcmp(token.data.u8->data, "concat") &&
        !strcmp(token.data.u8->data, "substrin") &&
        !strcmp(token.data.u8->data, "strcmp") &&
        !strcmp(token.data.u8->data, "ord") &&
        !strcmp(token.data.u8->data, "chr")) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (!strcmp(token.data.u8->data, "write") ||
        !strcmp(token.data.u8->data, "i2f") ||
        !strcmp(token.data.u8->data, "f2i") ||
        !strcmp(token.data.u8->data, "string") ||
        !strcmp(token.data.u8->data, "length") ||
        !strcmp(token.data.u8->data, "ord") ||
        !strcmp(token.data.u8->data, "chr")) {
        if (getCurrentToken().type != T_OPEN_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        // THIS MAY NOT WORK WELL DEBUG LATER !!
        expression();  // Parsování argumentu funkce
        if (CurrentToken.type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    } else if (!strcmp(token.data.u8->data, "concat") || !strcmp(token.data.u8->data, "substrin") || !strcmp(token.data.u8->data, "strcmp")) {
        if (getCurrentToken().type != T_OPEN_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        expression();  // Parsování prvního argumentu funkce
        if (CurrentToken.type != T_COMMA) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        expression();  // Parsování druhého argumentu funkce
        if (CurrentToken.type != T_CLOSE_PARENTHESES) {
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
    getCurrentToken();
    while (CurrentToken.type != T_CLOSE_PARENTHESES) {
        if (CurrentToken.type != T_COMMA) {
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
        getCurrentToken();
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
    else { 
        getCurrentToken();
    }
    if (CurrentToken.type != T_I32_ID && 
        CurrentToken.type != T_F64_ID && 
        CurrentToken.type != T_U8_ID &&
        CurrentToken.type != T_I32_NULLABLE &&
        CurrentToken.type != T_F64_NULLABLE &&
        CurrentToken.type != T_U8_NULLABLE &&
        CurrentToken.type != T_VOID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());  // Parsování těla funkce
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();  // Vrátíme token pro další zpracování
}

void parse_function_call() {
    //Token token = CurrentToken;
    getCurrentToken();
    while (CurrentToken.type != T_CLOSE_PARENTHESES) {
        expression();  // Parsování argumentů funkce
        if (CurrentToken.type != T_COMMA && CurrentToken.type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        else if (CurrentToken.type == T_COMMA) {
            getCurrentToken();
        }
    }
    //return getCurrentToken();  // Vrátíme token pro další zpracování
}

int main() {
    syntax_analysis();  // Spustí se syntaktická analýza
    return 0;
}