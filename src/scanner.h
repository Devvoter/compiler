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
#include <stdio.h>
#include <ctype.h>

enum automat_state {
    S_ERROR = 0,
    S_START,
    S_PROLOG,
    //

    S_MUL,                  // *
    S_DIV,                  // /
    S_ADD,                  // +
    S_SUB,                  // -

    // Relační operátory
    S_ASSIGN,               // =
    S_EQUALS,               // == 
    S_NOT_EQUALS,           // !=
    S_GREATER_THAN,         // >
    S_GREATER_OR_EQUAL,     // >=
    S_LESS_THAN,            // <
    S_LESS_OR_EQUAL,        // <=

    //
    S_AT,                   // @
    S_COLON,                // :
    S_DOT,                  // .
    S_COMMA,                // ,
    S_SEMICOLON,            // ;
    S_CLOSE_BRACKET,        // }
    S_OPEN_BRACKET,         // {
    S_CLOSE_PARENTHESES,    // )
    S_OPEN_PARENTHESES,     // (

    //
    S_LINE_COMMENT,         // //
    S_SLASH,                // /
    S_EXCLA,                // !
    //  
    S_QUOTE,                // "
    S_ID,                   // _a-zA-Z 
    S_QUESTIONER,           // ?
    S_TYPE_ID,              // ?i32 | ?[]u8 | ?f64
    S_LETTER,
    S_INT_NUM,
    S_FLOAT_NUM,
    S_EXP_NUM,

};

/**
 * @brief Možné typy tokenu
 * 
 */
typedef enum {

    // klíčová slova
    T_CONST = 0,   T_ELSE,        T_FN,
    T_IF,        T_I32_ID,    T_F64_ID,
    T_NULL,         T_PUB,    T_RETURN,
    T_U8_ID,        T_VAR,      T_VOID,
    T_WHILE,

    // typy
    T_STRING_TYPE,


    T_ID,

    // s přeponou ?
    T_I32_NULLABLE, T_F64_NULLABLE, T_U8_NULLABLE,

    // Aritmetické operátory
    T_MUL,                  // *
    T_DIV,                  // /
    T_ADD,                  // +
    T_SUB,                  // -

    // Relační operátory
    T_ASSIGN,               // =
    T_EQUALS,               // == 
    T_NOT_EQUALS,           // !=
    T_GREATER_THAN,         // >
    T_GREATER_OR_EQUAL,     // >=
    T_LESS_THAN,            // <
    T_LESS_OR_EQUAL,        // <=

    //
    T_AT,                   // @
    T_COLON,                // :
    T_DOT,                  // .
    T_COMMA,                // ,
    T_SEMICOLON,            // ;
    T_CLOSE_BRACKET,        // }
    T_OPEN_BRACKET,         // {
    T_CLOSE_PARENTHESES,    // )
    T_OPEN_PARENTHESES,     // (

    // Ostatní
    T_IMPORT,
    T_IFJ,
    T_EOF,
    T_UNKNOW,
    T_ERROR,
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
    
} Token;




/**
 * @brief Struktura pro mapování klíčových slov na jejich typy
 * 
 */
typedef struct {
    const char* keyword;
    TokenType token_type;
} KeywordTokenPair;



/**
 * @brief 
 * 
 */
void load_string(Token* token, char c, unsigned long *init_count);

/**
 * @brief 
 * 
 */
void load_symbol(Token* token, char c, unsigned long *init_count);

/**
 * @brief 
 * 
 */
void string_to_num(Token* token);

/**
 * @brief 
 * 
 */
void scanner_init(FILE* sourse);

/**
 * @brief 
 * 
 */
TokenType get_token_type(const char* word);

/**
 * @brief 
 * 
 */
TokenType isKeyWord(const char* word);

/**
 * @brief 
 * 
 */
Token scanner_get_next_token();


#endif // _SCANNER_H_