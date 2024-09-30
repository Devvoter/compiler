/**
 * @file scanner.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-09-29
 * 
 */


#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "buffer.h"

/**
 * @brief Možné typy tokenu
 * 
 */
typedef enum {

    // klíčová slova
    TOKEN_CONST = 0, TOKEN_ELSE, TOKEN_FN,
    TOKEN_IF,        TOKEN_I32,  TOKEN_F64,
    TOKEN_NULL,      TOKEN_PUB,  TOKEN_RETURN,
    TOKEN_U8,        TOKEN_VAR,  TOKEN_VOID,
    TOKEN_WHILE,

    // Aritmetické operátory
    TOKEN_MUL,                  // *
    TOKEN_DIV,                  // /
    TOKEN_ADD,                  // +
    TOKEN_SUB,                  // -

    // Relační operátory
    TOKEN_ASSIGN,               // =
    TOKEN_EQUALS,               // == 
    TTOKEN_NOT_EQUALS,          // !=
    TOKEN_GREATER_THAN,         // >
    TTOKEN_GREATER_OR_EQUAL,    // >=
    TOKEN_LESS_THAN,            // <
    TOKEN_LESS_OR_EQUAL,        // <=

    //
    TOKEN_AT,                   // @
    TOKEN_COLON,                // :
    TOKEN_DOT,                  // .
    TOKEN_COMMA,                // ,
    TOKEN_SEMICOLON,            // ;
    TOKEN_CLOSE_BRACKET,        // }
    TOKEN_OPEN_BRACKET,         // {
    TOKEN_CLOSE_PARENTHESES,    // )
    TOKEN_OPEN_PARANTHESES,     // (

    // Ostatní
    TOKEN_IFJ,
    TOKEN_EOF,
    TOKEN_UNKNOW,
    TOKEN_ERROR,
} TokenType;


/**
 * @brief Typy hodnot pro data v tokeně
 * 
 */
typedef union {
    long       i32;
    double     f64;
    tBuffer*    u8;
} TokenValue;


/**
 * @brief Token struktura
 *
 */
typedef struct {
    TokenType      type;    // typ tokena
    TokenValue     data;    // hodnota tokena
    unsigned long  line;    // řádek odkud býl token. just for debuging
    
    int isNullable;         // příznak označující typ s možnou hodnotou Null
} Token;


#endif // _SCANNER_H_