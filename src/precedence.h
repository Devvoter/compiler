#define NUM_OPERATORS 14
#define EMPTY ' '          

#ifndef _PRECEDENCE_H_
#define _PRECEDENCE_H_

#include "scanner.h"
#include "precedence_token.h"
#include "stack.h"
#include "error.h"
#include "generator.h"

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
        case T_IFJ: return 12;
        case T_SEMICOLON: return 13;
        default: return -1;         // Invalid operator
    }
}

PrecedenceToken tokenWrapper(Token token) {
    PrecedenceToken pt;
    pt.token = token;
    pt.isTerminal = true;
    pt.reduction = false;
    pt.type = token.type;
    return pt;
}

void ruleReduce(Stack *stack, tFrameStack *symtable) {
    PrecedenceToken *tokenTop = S_Top(stack);
    if (tokenTop->isTerminal) {
        if (tokenTop->token.type == T_ID ||
        tokenTop->token.type == T_I32_VAR ||
        tokenTop->token.type == T_F64_VAR ||
        tokenTop->token.type == T_NULL) {                                 // E -> id
            Token expr;
            PrecedenceToken reducedTop;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.line = tokenTop->token.line;
            if (tokenTop->token.type == T_ID) {
                expr.data.u8 = tokenTop->token.data.u8;
                tSymTabNode *idTS = search_symbol(symtable, tokenTop->token.data.u8->data);
                if (idTS->isFun) {
                    reducedTop.type = idTS->funData->retType;
                }
                else {
                    reducedTop.type = idTS->varData->dataType;
                    pushOnStackGen(expr.data.u8, variable_t);
                }
            }
            else if (tokenTop->token.type == T_I32_VAR) {
                expr.data.i32 = tokenTop->token.data.i32;
                reducedTop.type = T_I32_VAR;
                pushOnStackGen(expr.data.i32, int_t);
            }
            else if (tokenTop->token.type == T_F64_VAR) {
                expr.data.f64 = tokenTop->token.data.f64;
                reducedTop.type = T_F64_VAR;
                pushOnStackGen(expr.data.i32, float_t);
            }
            else if (tokenTop->token.type == T_NULL) {
                reducedTop.type = T_NULL;
                pushOnStackGen(expr.data.i32, null_t);
            }
            else if (tokenTop->token.type == T_IFJ) {
                tSymTabNode *idTS = search_symbol(symtable, tokenTop->token.data.u8->data);
                reducedTop.type = idTS->funData->retType;
                if (reducedTop.type == T_VOID) {
                    exitWithError(&tokenTop->token, ERR_SEM_TYPE_COMPATIBILITY);
                }
            }
            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Pop(stack);
            S_Top(stack)->reduction = false;
            S_Push(stack, reducedTop);
            return;
        }
        else if (tokenTop->token.type == T_CLOSE_PARENTHESES) {             // E -> (E)
            PrecedenceToken reducedTop;
            S_Pop(stack);
            PrecedenceToken *exprInParenth = S_Top(stack);
            if (exprInParenth->isTerminal) {
                exitWithError(&exprInParenth->token, ERR_SYNTAX_ANALYSIS);
            }
            reducedTop.type = exprInParenth->type;
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
    else if (tokenTop->token.type == T_EXPRESSION_NONTERMINAL) {             // E -> E op E
        int operandType1 = tokenTop->type;
        S_Pop(stack);
        PrecedenceToken *op = S_Top(stack);
        if (!op->isTerminal) {
            exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
        }
        // pridat nejakou kontrolu jestli lze operator pouzit pro tento typ operandu ?
        if (op->token.type != T_ADD && op->token.type != T_SUB && op->token.type != T_MUL && op->token.type != T_DIV &&
            op->token.type != T_EQUALS && op->token.type != T_NOT_EQUALS && op->token.type != T_LESS_THAN && op->token.type != T_GREATER_THAN &&
            op->token.type != T_LESS_OR_EQUAL && op->token.type != T_GREATER_OR_EQUAL) {
            exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
        }
        t_operationType operation;
        switch (op->token.type) {
            case T_ADD:
                operation = plus;
                break;
            case T_SUB:
                operation = minus;
                break;
            case T_MUL:
                operation = mul;
                break;
            case T_DIV:
                operation = div;
                break;
            case T_EQUALS:
                operation = EQ;
                break;
            case T_NOT_EQUALS:
                operation = NEQ;
                break;
            case T_LESS_THAN:
                operation = LT;
                break;
            case T_GREATER_THAN:
                operation = GT;
                break;
            case T_LESS_OR_EQUAL:
                operation = LEQ;
                break;
            case T_GREATER_OR_EQUAL:
                operation = GEQ;
                break;
            default:
                exitWithError(&op->token, ERR_INTERNAL_COMPILER);
        }
        makeOperationStackGen(op->token.type);
        S_Pop(stack);
        PrecedenceToken *secondOperand = S_Top(stack);
        int operandType2 = secondOperand->type;                            // typ 2. vyrazu
        if (secondOperand->isTerminal) {
            exitWithError(&secondOperand->token, ERR_SYNTAX_ANALYSIS);
        }
        S_Pop(stack);

        if (operandType1 != operandType2) {
            // PRIDAT LEPSI KONTROLU VCETNE PRIPADNYCH KONVERZI !!!
            exitWithError(&tokenTop->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        Token expr;
        expr.type = T_EXPRESSION_NONTERMINAL;
        // expr.data.u8 = bufferInit();                           //idk what is this
        // expr.data.u8->data = "E"; //? not sure maybe E op E?
        PrecedenceToken reducedTop;
        reducedTop.type = operandType1;
        reducedTop.token = expr;
        reducedTop.isTerminal = false;
        reducedTop.reduction = false;

        S_Top(stack)->reduction = false;    
        S_Push(stack, reducedTop);
        return;
    }
    exitWithError(&tokenTop->token, ERR_SYNTAX_ANALYSIS);
}

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