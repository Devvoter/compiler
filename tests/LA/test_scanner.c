/**
 * @file test_scanner.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-10-16
 * 
 */


#include "../src/scanner.h"
#include "../src/error.h"

int printTokenType(Token newToken) {
    // výpis tokena
    switch (newToken.type) {
        case T_EOF:
            printf("Token type: T_EOF                 line:%ld\n", newToken.line+1);
            return -1;
            break;
        case T_IMPORT:
            printf("Token type: T_IMPORT,             line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_IFJ:
            printf("Token type: T_IFJ,                line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_CONST:
            printf("Token type: T_CONST,              line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_ELSE:
            printf("Token type: T_ELSE,               line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_FN:
            printf("Token type: T_FN,                 line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_IF:
            printf("Token type: T_IF,                 line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_I32_VAR:
            printf("Token type: T_I32_VAR,            line:%ld, value: %ld\n", newToken.line+1, newToken.data.i32);
            break;
        case T_I32_ID:
            printf("Token type: T_I32_ID              line:%ld\n", newToken.line+1+1);
            break;
        case T_F64_VAR:
            printf("Token type: T_F64_VAR,            line:%ld, value: %f\n",newToken.line+1+1, newToken.data.f64);
            break;
        case T_F64_ID:
            printf("Token type: T_F64_ID              line:%ld\n", newToken.line+1+1);
            break;
        case T_NULL:
            printf("Token type: T_NULL                line:%ld\n", newToken.line+1);
            break;
        case T_PUB:
            printf("Token type: T_PUB,                line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_RETURN:
            printf("Token type: T_RETURN,             line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_U8_ID:
            printf("Token type: T_U8_ID,              line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_VAR:
            printf("Token type: T_VAR,                line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_VOID:
            printf("Token type: T_VOID,               line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_WHILE:
            printf("Token type: T_WHILE,              line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_I32_NULLABLE:
            printf("Token type: T_I32_NULLABLE,       line:%ld\n", newToken.line+1);
            break;
        case T_F64_NULLABLE:
            printf("Token type: T_F64_NULLABLE,       line:%ld\n", newToken.line+1);
            break;
        case T_U8_NULLABLE:
            printf("Token type: T_U8_NULLABLE,        line:%ld\n", newToken.line+1);
            break;
        case T_ID:
            printf("Token type: T_ID,                 line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_ASSIGN:
            printf("Token type: T_ASSIGN,             line:%ld, value: =\n", newToken.line+1);
            break;
        case T_AT:
            printf("Token type: T_AT,                 line:%ld, value: @\n", newToken.line+1);
            break;
        case T_STRING_TYPE:
            printf("Token type: T_STRING_TYPE,        line:%ld, value: %s\n", newToken.line+1, newToken.data.u8->data);
            break;
        case T_STRING_TYPE_EMPTY:
            printf("Token type: T_STRING_TYPE_EMPTY,  line:%ld\n", newToken.line+1);
            break;
        case T_OPEN_PARENTHESES:
            printf("Token type: T_OPEN_PARENTHESES,   line:%ld, value: (  \n", newToken.line+1);
            break;
        case T_CLOSE_PARENTHESES:
            printf("Token type: T_CLOSE_PARENTHESES,  line:%ld, value: )  \n", newToken.line+1);
            break;
        case T_OPEN_BRACKET:
            printf("Token type: T_OPEN_BRACKET,       line:%ld, value: {  \n", newToken.line+1);
            break;
        case T_CLOSE_BRACKET:
            printf("Token type: T_CLOSE_BRACKET,      line:%ld, value: }  \n", newToken.line+1);
            break;
        case T_SEMICOLON:
            printf("Token type: T_SEMICOLON,          line:%ld, value: ;  \n", newToken.line+1);
            break;
        case T_COLON:
            printf("Token type: T_COLON,              line:%ld, value: :  \n", newToken.line+1);
            break;
        case T_MUL:
            printf("Token type: T_MUL,                line:%ld, value: *  \n", newToken.line+1);
            break;
        case T_DIV:
            printf("Token type: T_DIV,                line:%ld, value: /  \n", newToken.line+1);
            break;
        case T_ADD:
            printf("Token type: T_ADD,                line:%ld, value: +  \n", newToken.line+1);
            break;
        case T_SUB:
            printf("Token type: T_SUB,                line:%ld, value: -  \n", newToken.line+1);
            break;
        case T_EQUALS:
            printf("Token type: T_EQUALS,             line:%ld, value: ==  \n", newToken.line+1);
            break;
        case T_NOT_EQUALS:
            printf("Token type: T_NOT_EQUALS,         line:%ld, value: !=  \n", newToken.line+1);
            break;
        case T_GREATER_THAN:
            printf("Token type: T_GREATER_THAN,       line:%ld, value: >  \n", newToken.line+1);
            break;
        case T_GREATER_OR_EQUAL:
            printf("Token type: T_GREATER_OR_EQUAL,   line:%ld, value: >=  \n", newToken.line+1);
            break;
        case T_LESS_THAN:
            printf("Token type: T_LESS_THAN,          line:%ld, value: <  \n", newToken.line+1);
            break;
        case T_LESS_OR_EQUAL:
            printf("Token type: T_LESS_OR_EQUAL,      line:%ld, value: <=  \n", newToken.line+1);
            break;
        case T_DOT:
            printf("Token type: T_DOT,                line:%ld, value: .  \n", newToken.line+1);
            break;
        case T_COMMA:
            printf("Token type: T_COMMA,              line:%ld, value: ,  \n", newToken.line+1);
            break;
        case T_VERTICAL_BAR:
            printf("Token type: T_VERTICAL_BAR,       line:%ld, value: |  \n", newToken.line+1);
            break;
        case T_UNKNOW:
            printf("Token type: T_UNKNOW,             line:%ld\n", newToken.line+1);
            break;
        case T_ERROR:
            printf("Token type: T_ERROR,              line:%ld\n", newToken.line+1);
            return -1;
        default:
            printf("im in switch in default - it might be a error, line:%ld\n", newToken.line+1);
            return -1;
            break;
    }
}


int main() {
#ifdef USE_FILE
    // čtení ze souboru
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    // inicializace souboru
    fileInit(file);
#endif

    // čteme a výpisujeme tokeny
    Token newToken;
    do newToken = getNextToken();
    while (printTokenType(newToken) != -1);

#ifdef USE_FILE
    fclose(file);
#endif

    return EXIT_SUCCESS;
}