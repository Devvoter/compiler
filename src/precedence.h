#define NUM_OPERATORS 14
#define EMPTY ' '          

#ifndef _PRECEDENCE_H_
#define _PRECEDENCE_H_

#include "scanner.h"
#include "precedence_token.h"
#include "stack.h"
#include "error.h"
#include "symtable.h"
//#include "generator.h"

char precedenceTable[NUM_OPERATORS][NUM_OPERATORS] = {
    //        +    -    *    /    ==   !=   <    >    <=   >=   (    )   term   ;    
    /* +  */ {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* -  */ {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* *  */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* /  */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /* == */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* != */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* <  */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* >  */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* <= */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* >= */ {'<', '<', '<', '<', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, '<', '>', '<', '>'},
    /* (  */ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', EMPTY},
    /* )  */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', EMPTY, '>', EMPTY, '>'},
    /*term*/ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', EMPTY, '>', EMPTY, '>'},
    /* ;  */ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', EMPTY, '<', EMPTY}
};

int getOperatorIndex(Token token) {
    switch (token.type) {
        case T_ADD: return 0;
        case T_SUB: return 1;
        case T_MUL: return 2;
        case T_DIV: return 3;
        case T_EQUALS: return 4;
        case T_NOT_EQUALS: return 5;
        case T_LESS_THAN: return 6;
        case T_GREATER_THAN: return 7;
        case T_LESS_OR_EQUAL: return 8;
        case T_GREATER_OR_EQUAL: return 9;
        case T_OPEN_PARENTHESES: return 10;
        case T_CLOSE_PARENTHESES: return 11;
        case T_ID: return 12;
        case T_I32_VAR: return 12;
        case T_F64_VAR: return 12;
        case T_NULL: return 12;
        case T_STRING_TYPE: return 12;
        case T_STRING_TYPE_EMPTY: return 12;
        case T_SEMICOLON: return 13;
        default: return -1;         // Invalid operator
    }
}

/**
 * @brief Funkce pro uložení aktualního tokenu a užitečných informací pro precedenční analýzu do struktury
 * 
 * @param token Aktuální token
 * @return Struktura s tokenem doplnena informaci pro precedenční analýzu
 */
PrecedenceToken tokenWrapper(Token token) {
    PrecedenceToken pt;
    pt.token = token;
    pt.isTerminal = true;
    pt.reduction = false;
    pt.type = token.type;
    return pt;
}

/**
 * @brief Funkce pro implicitní konverzi a kontrolu kompatibility datových typů operandů
 * 
 * @param operator Operátor
 * @param operand1 Ukazatel na první operand
 * @param operand2 Ukazatel na druhý operand
 * @param symtable Ukazatel na tabulku symbolů
 * 
 * @return Vrací výsledný datový typ výrazu
 */
TokenType typeConversion(TokenType operation, PrecedenceToken *operand1, PrecedenceToken *operand2, tFrameStack *symtable) {
    if (operand1->type == T_STRING_TYPE || operand1->type == T_U8_ID || operand1->type == T_U8_NULLABLE || // nelze provadet operace s řetězci a řezy
        operand2->type == T_STRING_TYPE || operand2->type == T_U8_ID || operand2->type == T_U8_NULLABLE) {
        exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
    }
    if (operation == T_LESS_THAN || operation == T_GREATER_THAN ||                                         // nelze porovnavat s typy zahrnující null
        operation == T_LESS_OR_EQUAL || operation == T_GREATER_OR_EQUAL) {
        if (operand1->type == T_I32_NULLABLE || operand1->type == T_F64_NULLABLE ||
            operand2->type == T_I32_NULLABLE || operand2->type == T_F64_NULLABLE ||
            operand1->type == T_NULL || operand2->type == T_NULL) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        // mužeme udělat implicitní konverzi literálu nebo konstanty typu i32 na f64
        if (operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && (operand1->isLiteral || search_symbol(symtable, operand1->token.data.u8->data)->varData->isConst)) {
            operand1->type = T_F64_VAR;
            return T_COMPARASION;
        }
        else if (operand1->type == T_F64_VAR && operand2->type == T_I32_VAR && (operand2->isLiteral || search_symbol(symtable, operand2->token.data.u8->data)->varData->isConst)) {
            operand2->type = T_F64_VAR;
            return T_COMPARASION;
        }
        // jinak pokud jsou oba operandy stejného typu, jsou kompatibilní a typ výrazu je T_COMPARASION
        else if (!semcheck_compare_dtypes(operand1->type, operand2->type)) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        else {
            return T_COMPARASION;
        }
    }
    else if (operation == T_ADD || operation == T_SUB || operation == T_MUL) {
        // mužeme udělat implicitní konverzi literálu typu i32 na f64
        if ((operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && operand1->isLiteral) ||
            (operand2->type == T_F64_VAR && operand2->type == T_I32_VAR && operand2->isLiteral)) {
            return T_F64_VAR; 
        }
        // jinak pokud jsou oba operandy stejného typu, jsou kompatibilní a typ výrazu je operand1->type
        else if (!semcheck_compare_dtypes(operand1->type, operand2->type)) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    else if (operation == T_DIV) {
        // máme pouze celočíselné a desetinné dělení, proto se typy operandů musí shodovat
        if (!semcheck_compare_dtypes(operand1->type, operand2->type)) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (operand1->type != T_I32_VAR && operand1->type != T_F64_VAR) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    else if (operation == T_EQUALS || operation == T_NOT_EQUALS) {
        // s null můžeme porovnávat pouze typy zahrnující null
        if (operand1->type == T_NULL) {
            if (!(operand2->type == T_I32_NULLABLE || operand2->type == T_F64_NULLABLE || operand2->type == T_NULL)) {
                exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
            }
        }
        else if (operand2->type == T_NULL) {
            if (!(operand1->type == T_I32_NULLABLE || operand1->type == T_F64_NULLABLE || operand1->type == T_NULL)) {
                exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
            }
        }
        // mužeme udělat implicitní konverzi literálu nebo konstanty typu i32 na f64
        if (operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && (operand1->isLiteral || search_symbol(symtable, operand1->token.data.u8->data)->varData->isConst)) {
            operand1->type = T_F64_VAR;
            return T_COMPARASION;
        }
        else if (operand1->type == T_F64_VAR && operand2->type == T_I32_VAR && (operand2->isLiteral || search_symbol(symtable, operand2->token.data.u8->data)->varData->isConst)) {
            operand2->type = T_F64_VAR;
            return T_COMPARASION;
        }
        // jinak pokud jsou oba operandy stejného typu, jsou kompatibilní a typ výrazu je T_COMPARASION
        else if (!semcheck_compare_dtypes(operand1->type, operand2->type)) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        else {
            return T_COMPARASION;
        }
    }
    // nemůžeme provádět operace s výrazy, které jsou porovnávací
    else if (operand1->type == T_COMPARASION || operand2->type == T_COMPARASION) {
        exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
    }
    return operand1->type;
}

/**
 * @brief Funkce pro redukci tokenů na zásobníku podle pravidel
 * 
 * @param stack Ukazatel na zásobník precedenční analýzy
 * @param symtable Ukazatel na tabulku symbolů
 */
void ruleReduce(Stack *stack, tFrameStack *symtable) {
    PrecedenceToken *tokenTop = S_Top(stack);
    // pokud token na vrcholu zásobníku je terminál
    if (tokenTop->isTerminal) {
        // E -> term
        if (tokenTop->token.type == T_ID ||
        tokenTop->token.type == T_I32_VAR ||
        tokenTop->token.type == T_F64_VAR ||
        tokenTop->token.type == T_NULL ||
        tokenTop->token.type == T_STRING_TYPE ||
        tokenTop->token.type == T_STRING_TYPE_EMPTY) {                                 // E -> id
            Token expr;
            PrecedenceToken reducedTop;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.line = tokenTop->token.line;
            if (tokenTop->token.type == T_ID) {
                expr.data.u8 = malloc(sizeof(*tokenTop->token.data.u8));
                if (expr.data.u8 == NULL) {
                    exitWithError(&tokenTop->token, ERR_INTERNAL_COMPILER);
                }
                *expr.data.u8 = *tokenTop->token.data.u8;
                reducedTop.isLiteral = false;
                tSymTabNode *idTS = search_symbol(symtable, tokenTop->token.data.u8->data);
                if (idTS->isFun) {
                    reducedTop.type = idTS->funData->retType;
                }
                else {
                    reducedTop.type = idTS->varData->dataType;
                    //pushOnStackGen(expr.data.u8, variable_t);
                    if(idTS->varData->isConstExpr && idTS->varData->isConst) {
                        reducedTop.isLiteral = true;
                    }
                }
                if (reducedTop.type == T_VOID) {
                    exitWithError(&tokenTop->token, ERR_SEM_TYPE_COMPATIBILITY);
                }
                if (reducedTop.type == T_I32_ID) {
                    reducedTop.type = T_I32_VAR;
                    if(reducedTop.isLiteral) {
                        expr.data.u8->data = idTS->varData->value.u8->data;
                    }
                }
                else if (reducedTop.type == T_F64_ID) {
                    reducedTop.type = T_F64_VAR;
                    if(reducedTop.isLiteral) {
                        expr.data.u8->data = idTS->varData->value.u8->data;
                    }
                }
            }
            else if (tokenTop->token.type == T_I32_VAR) {
                expr.data.i32 = tokenTop->token.data.i32;
                reducedTop.type = T_I32_VAR;
                reducedTop.isLiteral = true;
                //pushOnStackGen(expr.data.i32, int_t);
            }
            else if (tokenTop->token.type == T_F64_VAR) {
                expr.data.f64 = tokenTop->token.data.f64;
                reducedTop.type = T_F64_VAR;
                reducedTop.isLiteral = true;
                //pushOnStackGen(expr.data.i32, float_t);
            }
            else if (tokenTop->token.type == T_NULL) {
                reducedTop.type = T_NULL;
                reducedTop.isLiteral = false;
                //pushOnStackGen(expr.data.i32, null_t);
            }
            else if (tokenTop->token.type == T_STRING_TYPE || tokenTop->token.type == T_STRING_TYPE_EMPTY) {
                expr.data.u8 = malloc(sizeof(*tokenTop->token.data.u8));
                if (expr.data.u8 == NULL) {
                    exitWithError(&tokenTop->token, ERR_INTERNAL_COMPILER);
                }
                reducedTop.type = T_STRING_TYPE;
                reducedTop.isLiteral = false;
                //pushOnStackGen(expr.data.u8, string_t);
            }
            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Pop(stack);
            S_Top(stack)->reduction = false;
            S_Push(stack, reducedTop);
            return;
        }
        // E -> (E)
        else if (tokenTop->token.type == T_CLOSE_PARENTHESES) {
            PrecedenceToken reducedTop;
            S_Pop(stack);
            PrecedenceToken *exprInParenth = S_Top(stack);
            if (exprInParenth->isTerminal) {
                exitWithError(&exprInParenth->token, ERR_SYNTAX_ANALYSIS);
            }
            reducedTop.type = exprInParenth->type;
            reducedTop.isLiteral = exprInParenth->isLiteral;
            S_Pop(stack);
            PrecedenceToken *openParenth = S_Top(stack);
            if (openParenth->token.type != T_OPEN_PARENTHESES) {
                exitWithError(&openParenth->token, ERR_SYNTAX_ANALYSIS);
            }
            S_Pop(stack);

            Token expr;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.data.u8 = tokenTop->token.data.u8; //? not sure

            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Top(stack)->reduction = false;
            S_Push(stack, reducedTop);
            return;
        }
    }
    // pokud token na vrcholu zásobníku je neterminál (E)
    else if (tokenTop->token.type == T_EXPRESSION_NONTERMINAL) {
        // E -> E op E
        PrecedenceToken *firstOperand = malloc(sizeof(PrecedenceToken));
        if (firstOperand == NULL) {
            exitWithError(&tokenTop->token, ERR_INTERNAL_COMPILER);
        }
        *firstOperand = *tokenTop;
        S_Pop(stack);
        PrecedenceToken *op = S_Top(stack);
        if (!op->isTerminal) {
            exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
        }
        // kontrola, zda je token operátor
        if (op->token.type != T_ADD && op->token.type != T_SUB && op->token.type != T_MUL && op->token.type != T_DIV &&
            op->token.type != T_EQUALS && op->token.type != T_NOT_EQUALS && op->token.type != T_LESS_THAN && op->token.type != T_GREATER_THAN &&
            op->token.type != T_LESS_OR_EQUAL && op->token.type != T_GREATER_OR_EQUAL) {
            exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
        }

        int operation = op->token.type;
        S_Pop(stack);
        PrecedenceToken *secondOperand = malloc(sizeof(PrecedenceToken));
        *secondOperand = *S_Top(stack);
        if (secondOperand->isTerminal) {
            exitWithError(&secondOperand->token, ERR_SYNTAX_ANALYSIS);
        }
        S_Pop(stack);

        // vytvarime token pro zredukovany vyraz
        Token expr;
        expr.type = T_EXPRESSION_NONTERMINAL;
        expr.line = firstOperand->token.line;
        PrecedenceToken reducedTop;
        // provedeme implicitni konverzi a kontrolu kompatibility datovych typu operandu
        reducedTop.type = typeConversion(operation, firstOperand, secondOperand, symtable);
        reducedTop.isLiteral = firstOperand->isLiteral && secondOperand->isLiteral;
        reducedTop.token = expr;
        reducedTop.isTerminal = false;
        reducedTop.reduction = false;

        S_Top(stack)->reduction = false;    
        S_Push(stack, reducedTop);
        free (firstOperand);
        free (secondOperand);
        return;
    }
    exitWithError(&tokenTop->token, ERR_SYNTAX_ANALYSIS);
}

/**
 * @brief Funkce pro získání posledního terminálu na zásobníku
 * 
 * @param s Ukazatel na zásobník precedenční analýzy
 * 
 * @return Vrací poslední terminál na zásobníku
 */
PrecedenceToken *S_getTopTerminal(Stack *s) {
    StackItem *tmp = s->top;
    while (tmp != NULL) {
        PrecedenceToken token = tmp->data;
        if (token.isTerminal) {
            return &(tmp->data);
        }
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * @brief Funkce pro ověření, zda je na zásobníku redukovatelný výraz
 * 
 * @param s Ukazatel na zásobník precedenční analýzy
 * 
 * @return true pokud na zásobníku je redukovatelný výraz, jinak false
 */
bool S_topTerminalReduce(Stack *s) {
    StackItem *tmp = s->top;
    while (tmp != NULL) {
        PrecedenceToken token = tmp->data;
        if (token.reduction == true) {
            return &(tmp->data);
        }
        tmp = tmp->next;
    }
    return false;
}

/**
 * @brief Funkce pro ověření, zda byl výraz celý zpracován a zredukován do jednoho neterminálu E
 * 
 * @param stack Ukazatel na zásobník precedenční analýzy
 * 
 * @return true pokud byl výraz zredukován do jednoho neterminálu E, jinak false
 */
bool checkExprEnd(Stack *stack) {
    PrecedenceToken checkEnd = stack->top->data;
    if (checkEnd.token.type != T_EXPRESSION_NONTERMINAL) {
        return false;
    }
    checkEnd = stack->top->next->data;
    if (checkEnd.token.type != T_SEMICOLON) {
        return false;
    }
    if (stack->top->next->next != NULL) {
        return false;
    }
    return true;
}

#endif //_PRECEDENCE_H_