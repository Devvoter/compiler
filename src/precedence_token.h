#ifndef _PRECEDENCE_TOKEN_H_
#define _PRECEDENCE_TOKEN_H_

#include "scanner.h"

/**
 * @brief Struktura s tokenem doplnena informaci pro precedenční analýzu
 */
typedef struct {
    Token token;
    bool isTerminal;
    bool reduction;
    int type;
    bool isLiteral;
} PrecedenceToken;

#endif //_PRECEDENCE_TOKEN_H_