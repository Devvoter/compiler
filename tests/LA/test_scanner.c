/**
 * @file test_scanner.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-10-16
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/scanner.h" 

void print_token_type(Token newToken) {
    // výpis tokena
    switch (newToken.type) {
        case T_IMPORT:
            printf("Token type: T_IMPORT, value: %s\n", newToken.data.u8->data);
            break;
        case T_IFJ:
            printf("Token type: T_IFJ, value: %s\n", newToken.data.u8->data);
            break;
        case T_CONST:
            printf("Token type: T_CONST, value: %s\n", newToken.data.u8->data);
            break;
        case T_ELSE:
            printf("Token type: ELSE, value: %s\n", newToken.data.u8->data);
            break;
        case T_FN:
            printf("Token type: T_FN, value: %s\n", newToken.data.u8->data);
            break;
        case T_IF:
            printf("Token type: T_IF, value: %s\n", newToken.data.u8->data);
            break;
        case T_I32_ID:
            printf("Token type: T_I32_ID, value: %ld\n", newToken.data.i32);
            break;
        case T_F64_ID:
            printf("Token type: T_F64_ID, value: %f\n", newToken.data.f64);
            break;
        case T_NULL:
            printf("Token type: T_NULL\n");
            break;
        case T_PUB:
            printf("Token type: T_PUB, value: %s\n", newToken.data.u8->data);
            break;
        case T_RETURN:
            printf("Token type: T_RETURN, value: %s\n", newToken.data.u8->data);
            break;
        case T_U8_ID:
            printf("Token type: T_U8_ID, value: %s\n", newToken.data.u8->data);
            break;
        case T_VAR:
            printf("Token type: T_VAR, value: %s\n", newToken.data.u8->data);
            break;
        case T_VOID:
            printf("Token type: T_VOID, value: %s\n", newToken.data.u8->data);
            break;
        case T_WHILE:
            printf("Token type: T_WHILE, value: %s\n", newToken.data.u8->data);
            break;
        case T_I32_NULLABLE:
            printf("Token type: T_I32_NULLABLE\n");
            break;
        case T_F64_NULLABLE:
            printf("Token type: T_F64_NULLABLE\n");
            break;
        case T_U8_NULLABLE:
            printf("Token type: T_U8_NULLABLE\n");
            break;
        case T_ID:
            printf("Token type: T_ID, value: %s\n", newToken.data.u8->data);
            break;
        case T_ASSIGN:
            printf("Token type: T_ASSIGN, value: =\n");
            break;
        case T_AT:
            printf("Token type: T_AT, value: %s\n", newToken.data.u8->data);
            break;
        case T_STRING_TYPE:
            printf("Token type: STRING TYPE ([]u8), value: %s\n", newToken.data.u8->data);
            break;
        case T_OPEN_PARENTHESES:
            printf("Token type: T_OPEN_PARENTHESES, value: %s\n", newToken.data.u8->data);
            break;
        case T_CLOSE_PARENTHESES:
            printf("Token type: T_CLOSE_PARENTHESES, value: %s\n", newToken.data.u8->data);
            break;
        case T_OPEN_BRACKET:
            printf("Token type: T_OPEN_BRACKET, value: %s\n", newToken.data.u8->data);
            break;
        case T_CLOSE_BRACKET:
            printf("Token type: T_CLOSE_BRACKET, value: %s\n", newToken.data.u8->data);
            break;
        case T_SEMICOLON:
            printf("Token type: T_SEMICOLON, value: ;\n");
            break;
        case T_COLON:
            printf("Token type: T_COLON, value: %s\n", newToken.data.u8->data);
            break;
        case T_ERROR:
            printf("Token type: ERROR, value: %s\n", newToken.data.u8->data);
        break;
        default:
            printf("Token type: UNKNOWN\n");
            break;
    }
}


int main() {
    // čtení ze soubora
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    // 
    scanner_init(file);

    // čteme a výpisujeme tokeny 
    Token newToken;
    while ((newToken = scanner_get_next_token()).type != T_EOF) {
        print_token_type(newToken);
    }

    fclose(file);
    return EXIT_SUCCESS;
}