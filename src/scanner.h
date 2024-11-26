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

/**
 * @brief Možne stavy automatu.
 * 
 */
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
    S_VERTICAL_BAR,         // |
    S_SQUQRE_BRACKET_OPEN,  // [

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

    S_MULTILINE_STRING,

};

/**
 * @brief Možné typy tokenu.
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

    T_I32_VAR,
    T_F64_VAR,

    T_STRING_TYPE_EMPTY,
    
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
    T_VERTICAL_BAR,         // |

    // Ostatní
    T_IMPORT,
    T_IFJ,
    T_EOF,
    T_UNKNOW,
    T_ERROR,

    T_EXPRESSION_NONTERMINAL,
    T_COMPARASION,
} TokenType;


/**
 * @brief Typy hodnot pro data v tokeně.
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
    unsigned long  line;    // řádek odkud býl token
    
} Token;




/**
 * @brief Struktura pro mapování klíčových slov na jejich typy.
 * 
 */
typedef struct {
    const char* keyword;
    TokenType token_type;
} KeywordTokenPair;


/**
 * @brief Funkce pro zápis symbolu do string. Pro dálší kontrolu na
 *        typ tokenu.
 * 
 */
void loadSymbol(Token* token, char c, unsigned long *init_count);

/**
 * @brief Funkce převede čislo typu string na číslo typu int nebo float.
 * 
 * @param[in] token Ukazatel na strukturu token.
 */
void stringToNum(Token* token);

/**
 * @brief Inicializace souboru z kterýho bude číst.
 * 
 * @param[in] source Ukazatel na soubor.
 * 
 */
void fileInit(FILE* sourse);

/**
 * @brief Funkce odvodi jestli je nullable typ.
 * 
 * @param[in] source Ukazatel na token.
 * 
 */
void isNullType (Token* newToken);

/**
 * @brief Funkce dostane řetězec a zkontroluje jestli je klíčové slovo.
 * 
 * @param[in] word Ukazatel na řetězec.
 * 
 * @return Přidá tokenu typ a vrátí ho. 
 * 
 */
TokenType isKeyWord(const char* word);

/** Prvek jednosměrně vázaného seznamu. */
typedef struct ListToken{
    Token                    token;   // token
    struct ListToken  *nextToken;   // dalsi token
}*ListTokenPtr;

/** Jednosměrně vázaný seznam. */
typedef struct {
    /** Ukazatel na první token seznamu. */
    ListTokenPtr     firstToken;
    /** Ukazatel na aktuální prvek seznamu. */
    ListTokenPtr    activeToken;

    ListTokenPtr     tokenToGet;
    /** Aktuální délka seznamu. */
    int          currentLength;
}ListOfTokens;

/**
 * @brief Funkce inicializuje list.
 * 
 * @param[in] list Ukazatel na list.
 * 
 */
void init_list_of_tokens(ListOfTokens *list);

/**
 * @brief Uvolni pamet prvku a vrati list do stavu inicializace.
 * 
 * @param[in] list Ukazatel na list.
 * 
 */
void free_list_of_tokens(ListOfTokens *list);

/**
 * @brief Na konec listu prida token.
 * 
 * @param[in] list Ukazatel na list.
 *  
 */
void insert_in_list_of_tokens(ListOfTokens *list, Token token);

/**
 * @brief Funkce vola se jeden krat pred tim jak budete chtit cist tokeny z listu.
 *        Nastavi potrebne ukazatele na cteni tokenu. Pred opakovanym ctenim tokenu
 *        od zacatku je potreba znov ji zavota.
 * 
 * @param[in] list Ukazatel na list.
 * 
 */
void i_want_to_get_tokens(ListOfTokens *list);

/**
 * @brief Pri volani funkce, bude vracet token z listu
 * 
 * @param[in] list Ukazatel na list.
 * 
 * @return Token
 * 
 */
Token get_token_from_list(ListOfTokens *list);


/**
 * @brief Funkce zpracuje vstup a pošle dál token.
 * 
 * @return Vrácí token.
 * 
 */
Token getNextToken(ListOfTokens *list);

#endif // _SCANNER_H_