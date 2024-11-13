#define NUM_OPERATORS 14
#define EMPTY ' '          

#include "scanner.h"

typedef struct {
    Token token;
    bool isTerminal;
    bool reduction;
} PrecedenceToken;

char precedenceTable[NUM_OPERATORS][NUM_OPERATORS] = {
    // Fill in the table based on your image
    // Example:
    //        +    -    *    /    ==   !=   <    >    <=   >=   (    )    id   ;    
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
    /* id */ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', EMPTY, '>', EMPTY, '>'},
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
        case T_SEMICOLON: return 13;
        default: return -1;         // Invalid operator
    }
}

PrecedenceToken tokenWrapper(Token token) {
    PrecedenceToken pt;
    pt.token = token;
    pt.isTerminal = true;
    pt.reduction = false;
    return pt;
}

void addArrrowToTop(Stack *stack) {
    PrecedenceToken *tokenPtr = (PrecedenceToken *) S_Top(&stack);
    tokenPtr->reduction = true;
}

void ruleReduce(Stack *stack) {
    PrecedenceToken *tokenTop = (PrecedenceToken *) S_Top(&stack);
    if (tokenTop->isTerminal) {
        if (tokenTop->token.type == T_ID) {                                 // E -> id
            Token expr;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.data.u8 = tokenTop->token.data.u8;
            PrecedenceToken reducedTop;
            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Pop(&stack);
            S_Push(&stack, &reducedTop);
        }
        else if (tokenTop->token.type == T_CLOSE_PARENTHESES) {             // E -> (E)
            S_Pop(&stack);
            PrecedenceToken *exprInParenth = (PrecedenceToken *) S_Top(&stack);
            if (exprInParenth->isTerminal) {
                exitWithError(&exprInParenth->token, ERR_SYNTAX_ANALYSIS);
            }
            S_Pop(&stack);
            PrecedenceToken *openParenth = (PrecedenceToken *) S_Top(&stack);
            if (openParenth->token.type != T_OPEN_PARENTHESES) {
                exitWithError(&openParenth->token, ERR_SYNTAX_ANALYSIS);
            }
            S_Pop(&stack);

            Token expr;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.data.u8 = tokenTop->token.data.u8; //? not sure
            PrecedenceToken reducedTop;
            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Push(&stack, &reducedTop);
        }
    }
    else {
        if (tokenTop->token.type == T_EXPRESSION_NONTERMINAL) {             // E -> E op E
            S_Pop(&stack);
            PrecedenceToken *op = (PrecedenceToken *) S_Top(&stack);
            if (!op->isTerminal) {
                exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
            }
            if (op->token.type != T_ADD && op->token.type != T_SUB && op->token.type != T_MUL && op->token.type != T_DIV &&
                op->token.type != T_EQUALS && op->token.type != T_NOT_EQUALS && op->token.type != T_LESS_THAN && op->token.type != T_GREATER_THAN &&
                op->token.type != T_LESS_OR_EQUAL && op->token.type != T_GREATER_OR_EQUAL) {
                exitWithError(&op->token, ERR_SYNTAX_ANALYSIS);
            }
            S_Pop(&stack);
            PrecedenceToken *left = (PrecedenceToken *) S_Top(&stack);
            if (left->isTerminal) {
                exitWithError(&left->token, ERR_SYNTAX_ANALYSIS);
            }
            S_Pop(&stack);

            Token expr;
            expr.type = T_EXPRESSION_NONTERMINAL;
            expr.data.u8 = "E"; //? not sure maybe E op E?
            PrecedenceToken reducedTop;
            reducedTop.token = expr;
            reducedTop.isTerminal = false;
            reducedTop.reduction = false;

            S_Push(&stack, &reducedTop);
        }
    }
}

PrecedenceToken *getTopTerminal(Stack stack) {
    while (!S_IsEmpty(&stack)) {
        PrecedenceToken *token = (PrecedenceToken *) S_Top(&stack);
        if (token->isTerminal) {
            return token;
        }
        S_Pop(&stack);
    }
    return NULL;
}

bool checkExprEnd(Stack *stack) {
    PrecedenceToken *checkEnd = getTopTerminal(*stack);
    if (checkEnd->token.type != T_EXPRESSION_NONTERMINAL) {
        exitWithError(&checkEnd->token, ERR_SYNTAX_ANALYSIS);
    }
    S_Pop(stack);
    checkEnd = getTopTerminal(*stack);
    if (checkEnd->token.type != T_SEMICOLON) {
        exitWithError(&checkEnd->token, ERR_SYNTAX_ANALYSIS);
    }
    S_Pop(stack);
    if (!S_IsEmpty(stack)) {
        exitWithError(&checkEnd->token, ERR_SYNTAX_ANALYSIS);
    }
    return true;
}