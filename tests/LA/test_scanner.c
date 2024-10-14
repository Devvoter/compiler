#include <stdio.h>
#include <stdlib.h>
#include "../src/scanner.h"  // Подключаем заголовочный файл

void print_token_type(Token newToken) {
    // Выводим тип токена
    switch (newToken.type) {
        case T_CONST:
            printf("Token type: CONST, value: %s\n", newToken.data.u8->data);
            break;
        case T_ELSE:
            printf("Token type: ELSE, value: else\n");
            break;
        case T_FN:
            printf("Token type: FUNCTION (fn), value: fn\n");
            break;
        case T_IF:
            printf("Token type: IF, value: if\n");
            break;
        case T_I32_ID:
            printf("Token type: I32, value: i32\n");
            break;
        case T_F64_ID:
            printf("Token type: F64, value: f64\n");
            break;
        case T_NULL:
            printf("Token type: NULL, value: null\n");
            break;
        case T_PUB:
            printf("Token type: PUB, value: pub\n");
            break;
        case T_RETURN:
            printf("Token type: RETURN, value: return\n");
            break;
        case T_U8_ID:
            printf("Token type: U8, value: u8\n");
            break;
        case T_VAR:
            printf("Token type: VAR, value: var\n");
            break;
        case T_VOID:
            printf("Token type: VOID, value: void\n");
            break;
        case T_WHILE:
            printf("Token type: WHILE, value: while\n");
            break;
        case T_I32_NULLABLE:
            printf("Token type: I32 NULLABLE, value: ?i32\n");
            break;
        case T_F64_NULLABLE:
            printf("Token type: F64 NULLABLE, value: ?f64\n");
            break;
        case T_U8_NULLABLE:
            printf("Token type: U8 NULLABLE, value: ?[]u\n");
            break;
        case T_ID:
            printf("Token type: IDENTIFIER, value: %s\n", newToken.data.u8->data);
            break;
        case T_ASSIGN:
            printf("Token type: ASSIGN, value: =\n");
            break;
        case T_AT:
            printf("Token type: AT (@), value: @\n");
            break;
        case T_STRING_TYPE:
            printf("Token type: STRING TYPE ([]u8), value: %s\n", newToken.data.u8->data);
            break;
        case T_OPEN_PARENTHESES:
            printf("Token type: OPEN PARENTHESIS, value: (\n");
            break;
        case T_CLOSE_PARENTHESES:
            printf("Token type: CLOSE PARENTHESIS, value: )\n");
            break;
        case T_OPEN_BRACKET:
            printf("Token type: OPEN BRACKET, value: {\n");
            break;
        case T_CLOSE_BRACKET:
            printf("Token type: CLOSE BRACKET, value: }\n");
            break;
        case T_SEMICOLON:
            printf("Token type: SEMICOLON, value: ;\n");
            break;
        case T_COLON:
            printf("Token type: COLON, value: :\n");
            break;
        default:
            printf("Token type: UNKNOWN\n");
            break;
    }
}


int main() {
    // Открываем файл для чтения
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    // Инициализируем сканер для работы с файлом
    scanner_init(file);

    // Читаем токены и выводим их типы
    Token newToken;
    while ((newToken = scanner_get_next_token()).type != T_EOF) {
        print_token_type(newToken);
    }

    fclose(file);
    return EXIT_SUCCESS;
}