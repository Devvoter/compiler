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
        case T_IFJ: return 12;
        case T_STRING_TYPE: return 12;
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

TokenType typeConversion(TokenType operation, PrecedenceToken *operand1, PrecedenceToken *operand2, tFrameStack *symtable) {
    if (operand1->type == T_STRING_TYPE || operand1->type == T_U8_ID || operand1->type == T_U8_NULLABLE || // nelze provadet operace s typy zahrnujici null 
        operand2->type == T_STRING_TYPE || operand2->type == T_U8_ID || operand2->type == T_U8_NULLABLE) {
        exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
    }
    else if (operation == T_LESS_THAN || operation == T_GREATER_THAN ||        // nelze porovnavat typy zahrnujici null
        operation == T_LESS_OR_EQUAL || operation == T_GREATER_OR_EQUAL) {
        if (operand1->type == T_I32_NULLABLE || operand1->type == T_F64_NULLABLE ||
            operand2->type == T_I32_NULLABLE || operand2->type == T_F64_NULLABLE ||
            operand1->type == T_NULL || operand2->type == T_NULL) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && (operand1->isLiteral || search_symbol(symtable, operand1->token.data.u8->data)->varData->isConst)) {
            return T_F64_VAR;
        }
        else if (operand1->type == T_F64_VAR && operand2->type == T_I32_VAR && (operand2->isLiteral || search_symbol(symtable, operand2->token.data.u8->data)->varData->isConst)) {
            return T_F64_VAR;
        }
        else if (operand1->type != operand2->type) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        else {
            return T_COMPARASION;
        }
    }
    else if (operation == T_ADD || operation == T_SUB || operation == T_MUL) {
        if ((operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && operand1->isLiteral) ||
            (operand2->type == T_F64_VAR && operand2->type == T_I32_VAR && operand2->isLiteral)) {
            return T_F64_VAR; 
        }
        else if (operand1->type != operand2->type) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    else if (operation == T_DIV) {         // whats with null types?
        if (operand1->type != operand2->type) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (operand1->type != T_I32_VAR && operand1->type != T_F64_VAR) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    else if (operation == T_EQUALS || operation == T_NOT_EQUALS) {
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
        if (operand1->type == T_I32_VAR && operand2->type == T_F64_VAR && (operand1->isLiteral || search_symbol(symtable, operand1->token.data.u8->data)->varData->isConst)) {
            return T_F64_VAR;
        }
        else if (operand1->type == T_F64_VAR && operand2->type == T_I32_VAR && (operand2->isLiteral || search_symbol(symtable, operand2->token.data.u8->data)->varData->isConst)) {
            return T_F64_VAR;
        }
        else if (operand1->type != operand2->type) {
            exitWithError(&operand1->token, ERR_SEM_TYPE_COMPATIBILITY);
        }
        else {
            return T_COMPARASION;
        }
    }
    return operand1->type;
}

void ruleReduce(Stack *stack, tFrameStack *symtable) {
    PrecedenceToken *tokenTop = S_Top(stack);
    if (tokenTop->isTerminal) {
        if (tokenTop->token.type == T_ID ||
        tokenTop->token.type == T_I32_VAR ||
        tokenTop->token.type == T_F64_VAR ||
        tokenTop->token.type == T_NULL ||
        tokenTop->token.type == T_STRING_TYPE) {                                 // E -> id
            Token expr;
            PrecedenceToken reducedTop;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.line = tokenTop->token.line;
            if (tokenTop->token.type == T_ID) {
                expr.data.u8 = tokenTop->token.data.u8;
                reducedTop.isLiteral = false;
                tSymTabNode *idTS = search_symbol(symtable, tokenTop->token.data.u8->data);
                if (idTS->isFun) {
                    reducedTop.type = idTS->funData->retType;
                }
                else {
                    reducedTop.type = idTS->varData->dataType;
                    //pushOnStackGen(expr.data.u8, variable_t);
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
            else if (tokenTop->token.type == T_IFJ) {
                tSymTabNode *idTS = search_symbol(symtable, tokenTop->token.data.u8->data);
                reducedTop.type = idTS->funData->retType;
                reducedTop.isLiteral = false;
                if (reducedTop.type == T_VOID) {
                    exitWithError(&tokenTop->token, ERR_SEM_TYPE_COMPATIBILITY);
                }
            }
            else if (tokenTop->token.type == T_STRING_TYPE) {
                expr.data.u8 = tokenTop->token.data.u8;
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
        else if (tokenTop->token.type == T_CLOSE_PARENTHESES) {             // E -> (E)
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
    else if (tokenTop->token.type == T_EXPRESSION_NONTERMINAL) {             // E -> E op E
        PrecedenceToken *firstOperand = tokenTop;
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

        //makeOperationStackGen(op->token.type);
        S_Pop(stack);
        PrecedenceToken *secondOperand = S_Top(stack);
        if (secondOperand->isTerminal) {
            exitWithError(&secondOperand->token, ERR_SYNTAX_ANALYSIS);
        }
        S_Pop(stack);

        // vytvarime token pro zredukovany vyraz!!
        Token expr;
        expr.type = T_EXPRESSION_NONTERMINAL;
        // expr.data.u8 = bufferInit();                           //idk what is this
        // expr.data.u8->data = "E"; //? not sure maybe E op E?
        PrecedenceToken reducedTop;
        reducedTop.type = typeConversion(op->token.type, firstOperand, secondOperand, symtable);
        reducedTop.isLiteral = firstOperand->isLiteral && secondOperand->isLiteral;
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