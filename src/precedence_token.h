#ifndef _PRECEDENCE_TOKEN_H_
#define _PRECEDENCE_TOKEN_H_

#include "scanner.h"

typedef struct {
    Token token;
    bool isTerminal;
    bool reduction;
    int type;
    bool isLiteral;
} PrecedenceToken;

#endif //_PRECEDENCE_TOKEN_H_