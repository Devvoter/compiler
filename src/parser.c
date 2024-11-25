/**
 * @file parser.c
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Implementace parseru
 * 
 * @date 2024-10-04
 * 
 */

#include "parser.h"
#include "error.h"
#include "stack.h"
#include "precedence.h"
#include "symtable.h"
#include "semantic.h"

Token CurrentToken;
ListOfTokens LIST;
tFrameStack symtable;

Token getCurrentToken() {
    Token token = get_token_from_list(&LIST);
    CurrentToken = token;
    return token;
}

void addStandardFunctionsToTS() {
    tSymTabNode *node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "readstr";
    node->funData->retType = T_U8_NULLABLE;
    node->funData->paramCnt = 0;
    node->funData->paramTypes = NULL;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "readi32";
    node->funData->retType = T_I32_NULLABLE;
    node->funData->paramCnt = 0;
    node->funData->paramTypes = NULL;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "readf64";
    node->funData->retType = T_F64_NULLABLE;
    node->funData->paramCnt = 0;
    node->funData->paramTypes = NULL;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "write";
    node->funData->retType = T_VOID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = NULL;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "i2f";
    node->funData->retType = T_F64_ID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = malloc(sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_I32_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "f2i";
    node->funData->retType = T_I32_ID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = malloc(sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_F64_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "string";
    node->funData->retType = T_U8_ID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = NULL;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "length";
    node->funData->retType = T_I32_ID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = malloc(sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_U8_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "concat";
    node->funData->retType = T_U8_ID;
    node->funData->paramCnt = 2;
    node->funData->paramTypes = malloc(2 * sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_U8_ID;
    node->funData->paramTypes[1] = T_U8_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "substring";
    node->funData->retType = T_U8_ID;
    node->funData->paramCnt = 3;
    node->funData->paramTypes = malloc(3 * sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_U8_ID;
    node->funData->paramTypes[1] = T_I32_ID;
    node->funData->paramTypes[2] = T_I32_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "strcmp";
    node->funData->retType = T_I32_ID;
    node->funData->paramCnt = 2;
    node->funData->paramTypes = malloc(2 * sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_U8_ID;
    node->funData->paramTypes[1] = T_U8_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "ord";
    node->funData->retType = T_I32_ID;
    node->funData->paramCnt = 2;
    node->funData->paramTypes = malloc(2 * sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_U8_ID;
    node->funData->paramTypes[1] = T_I32_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }

    node = create_fun_node();
    if (node == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->id = "chr";
    node->funData->retType = T_U8_ID;
    node->funData->paramCnt = 1;
    node->funData->paramTypes = malloc(sizeof(int));
    if (node->funData->paramTypes == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    node->funData->paramTypes[0] = T_I32_ID;
    if (!insert_symbol(&symtable, node)) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
}

void global_symtable() {
    addStandardFunctionsToTS();
    Token token = getNextToken(&LIST);
    bool mainPresent = false;
    bool inMain = false;
    while (token.type != T_EOF) {
        if (token.type == T_FN) {
            token = getNextToken(&LIST);
            if (token.type != T_ID) {
                exitWithError(&token, ERR_SYNTAX_ANALYSIS);
            }
            if (strcmp(token.data.u8->data, "main")==0) {
                mainPresent = true;
                inMain = true;
            }
            tSymTabNode *node = create_fun_node();
            if (node == NULL) {
                exitWithError(&token, ERR_INTERNAL_COMPILER);
            }
            node->id = token.data.u8->data;
            getNextToken(&LIST);
            token = getNextToken(&LIST);
            int numOfParams = 0;
            int *paramTypes = malloc(sizeof(int));
            // if (token.type != T_OPEN_PARENTHESES) {
            //     exitWithError(&token, ERR_SYNTAX_ANALYSIS);
            // }
            while (token.type != T_CLOSE_PARENTHESES && token.type != T_EOF) {
                if (token.type == T_I32_ID || token.type == T_I32_NULLABLE ||
                    token.type == T_F64_ID || token.type == T_F64_NULLABLE ||
                    token.type == T_U8_ID || token.type == T_U8_NULLABLE) {
                    paramTypes = realloc(paramTypes, (numOfParams + 1) * sizeof(int));
                    if (paramTypes == NULL) {
                        exitWithError(&token, ERR_INTERNAL_COMPILER);
                    }
                    paramTypes[numOfParams] = token.type;
                    numOfParams++;
                    }
                token = getNextToken(&LIST);
            }
            if (token.type == T_EOF) {
                exitWithError(&token, ERR_SYNTAX_ANALYSIS);
            }

            token = getNextToken(&LIST);
            if (inMain) {
                if (token.type != T_VOID) {
                    exitWithError(&token, ERR_SEM_INVALID_FUNC_PARAMS);
                }
                inMain = false;
            }
            
            switch (token.type)
            {
            case T_I32_ID:
                node->funData->retType = T_I32_VAR;
                break;
            case T_F64_ID:
                node->funData->retType = T_F64_VAR;
                break;
            case T_U8_ID:
                node->funData->retType = T_U8_ID;
                break;
            case T_I32_NULLABLE:
                node->funData->retType = T_I32_NULLABLE;
                break;
            case T_F64_NULLABLE:
                node->funData->retType = T_F64_NULLABLE;
                break;
            case T_U8_NULLABLE:
                node->funData->retType = T_U8_NULLABLE;
                break;
            case T_VOID:
                node->funData->retType = T_VOID;
                break;
            default:
                exitWithError(&token, ERR_SYNTAX_ANALYSIS);
                break;
            }
            node->funData->paramCnt = numOfParams;
            node->funData->paramTypes = paramTypes;
            if (!insert_symbol(&symtable, node)) {
                exitWithError(&token, ERR_SEM_REDEFINITION);
            }
        }
        token = getNextToken(&LIST);
    }
    if (!mainPresent) {
        exitWithError(&token, ERR_SEM_UNDEFINED_FUNC_VAR);
    }
}

void syntax_analysis() {
    if (parse_prolog() != 0) {
        fprintf(stderr, "Syntax error: invalid prologue\n");
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }

    Token token = code(getCurrentToken());
    if (token.type != T_EOF) {
        exitWithError(&token, ERR_SYNTAX_ANALYSIS);
    }
}

int parse_prolog() {
    if (getCurrentToken().type != T_CONST) {
        return 1;
    }
    if (getCurrentToken().type != T_IFJ) {
        return 1;
    }
    if (getCurrentToken().type != T_ASSIGN) {
        return 1;
    }
    if (getCurrentToken().type != T_AT) {
        return 1;
    }
    if (getCurrentToken().type != T_IMPORT) {
        return 1;
    }
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        return 1;
    }
    Token token = getCurrentToken();
    if (token.type != T_STRING_TYPE || (strcmp(token.data.u8->data, "ifj24.zig") != 0)) {
        return 1;
    }
    if (getCurrentToken().type != T_CLOSE_PARENTHESES || getCurrentToken().type != T_SEMICOLON) {
        return 1;
    }
    return 0;
}

// code() bude volat konkrétní funkci dle typu tokenu
Token code(Token token) {
    switch (token.type) {
        case T_IF:
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_if());
        case T_WHILE:
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_while());
        case T_VAR:
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_variable_definition());
        case T_CONST:
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_variable_definition());
        case T_RETURN:
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_return());
        case T_ID: // přiřazení nebo volání funkce
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_assignment_or_function_call());
        case T_IFJ: // volani vestavene funkce
            if(semcheck_in_global()) {
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_assignment_or_function_call());
        case T_PUB:
            if(!semcheck_in_global()){
                exitWithError(&token, ERR_SEM_OTHER);
            }
            return code(parse_function_definition());
        default:
            return token;
    }
}

// Funkce pro parsování IF konstrukce
Token parse_if() {
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    TokenType exprType = expression();                                       // Parsování výrazu uvnitř závorek
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    tFrame *frameIf = push_frame (&symtable, false);                        // Začátek nového bloku IF
    if(frameIf == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    getCurrentToken();
    if (CurrentToken.type == T_VERTICAL_BAR) {                               // if (...) | id_bez_null |
            if (getCurrentToken().type != T_ID) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        tSymTabNode *newNode = create_var_node(false);
        if (newNode == NULL) {
            exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
        }
        newNode->id = CurrentToken.data.u8->data;
        switch (exprType) {
            case T_I32_NULLABLE:
                newNode->varData->dataType = T_I32_VAR;
                break;
            case T_F64_NULLABLE:
                newNode->varData->dataType = T_F64_VAR;
                break;
            case T_U8_NULLABLE:
                newNode->varData->dataType = T_U8_ID;
                break;
            default:
                exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
        newNode->varData->isConst = false;
        newNode->varData->isUsed = false;
        if (!insert_symbol(&symtable, newNode)) {
            exitWithError(&CurrentToken, ERR_SEM_REDEFINITION);
        }
        if (getCurrentToken().type != T_VERTICAL_BAR) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        getCurrentToken();
    }
    else {
        if (exprType != T_COMPARASION) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    if (CurrentToken.type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());                                  // Parsování těla if-bloku
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    //VYSTUP Z IF: 
    tFrame *tmpFrameIf = pop_frame(&symtable);
    if(!semcheck_var_usage(tmpFrameIf->symTable)) {
        exitWithError(&token, ERR_SEM_UNUSED_VAR);
    }
    dispose_frame(tmpFrameIf);

    // Zpracování volitelného else
    token = getCurrentToken();
    if (token.type == T_ELSE) {
        tFrame *frameElse = push_frame(&symtable, false);   // Nová úroveň tabulky symbolú
        if(frameElse == NULL) {
            exitWithError(&token, ERR_INTERNAL_COMPILER);
        }
        if (getCurrentToken().type != T_OPEN_BRACKET) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        Token token_2 = code(getCurrentToken());                            // Parsování else-bloku
        if (token_2.type != T_CLOSE_BRACKET) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        // VÝSTUP Z ELSE bloku: 
        tFrame *tmpFrameElse = pop_frame(&symtable);
        if(!semcheck_var_usage(tmpFrameElse->symTable)) {
            exitWithError(&token_2, ERR_SEM_UNUSED_VAR);
        }
        dispose_frame(tmpFrameElse);
        return getCurrentToken();                                           // Načteme token pro další zpracování
    } else {
        return token;
    }
}

// Funkce pro parsování WHILE konstrukce
Token parse_while() {
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
       exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    TokenType exprType = expression();                                   // Parsování výrazu uvnitř závorek
    // if (exprType != T_COMPARASION) {
    //     exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
    // }
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    tFrame *frameTS = push_frame(&symtable, false);                     // Začátek nového bloku WHILE
    if(frameTS == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    getCurrentToken();
    if (CurrentToken.type == T_VERTICAL_BAR) {                           // if (...) | id_bez_null |
        if (getCurrentToken().type != T_ID) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        tSymTabNode *newNode = create_var_node(false);
        if (newNode == NULL) {
            exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
        }
        newNode->id = CurrentToken.data.u8->data;
        switch (exprType) {
            case T_I32_NULLABLE:
                newNode->varData->dataType = T_I32_VAR;
                break;
            case T_F64_NULLABLE:
                newNode->varData->dataType = T_F64_VAR;
                break;
            case T_U8_NULLABLE:
                newNode->varData->dataType = T_U8_ID;
                break;
            default:
                exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
        newNode->varData->isConst = false;              // ???
        newNode->varData->isUsed = false;
        if (!insert_symbol(&symtable, newNode)) {
            exitWithError(&CurrentToken, ERR_SEM_REDEFINITION);
        }

        if (getCurrentToken().type != T_VERTICAL_BAR) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        getCurrentToken();
    }
    else {
        if (exprType != T_COMPARASION) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    if (CurrentToken.type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());                              // Parsování těla while smyčky
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS); 
    }
    // VÝSTUP Z WHILE: 
    tFrame *tmpFrame = pop_frame(&symtable);
    if(!semcheck_var_usage(tmpFrame->symTable)) {
        exitWithError(&token, ERR_SEM_UNUSED_VAR);
    }
    dispose_frame(tmpFrame);
    return getCurrentToken();                                           // Vrátíme token pro další zpracování
}

Token parse_variable_definition() {
    tSymTabNode *newNode;
    if (CurrentToken.type == T_CONST) {
        newNode = create_var_node(true);
    }
    else {
        newNode = create_var_node(false);
    }
    Token id = getCurrentToken();
    if (id.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    newNode->id = id.data.u8->data;
    newNode->varData->isUsed = false;
    Token token = getCurrentToken();
    if (token.type != T_COLON && token.type != T_ASSIGN) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    bool autoType = true;
    if (token.type == T_COLON) {
        autoType = false;
        token = getCurrentToken();
        if (token.type != T_I32_ID && 
        token.type != T_F64_ID &&
        token.type != T_U8_ID &&
        token.type != T_I32_NULLABLE &&
        token.type != T_F64_NULLABLE &&
        token.type != T_U8_NULLABLE) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        newNode->varData->dataType = token.type;
        if (getCurrentToken().type != T_ASSIGN) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    getCurrentToken();
    // if (CurrentToken.type == T_STRING_TYPE || CurrentToken.type == T_STRING_TYPE_EMPTY) {
    //     // if(!semcheck_define_string()) {
    //     //     exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
    //     // }
    //     if (getCurrentToken().type != T_SEMICOLON) {
    //         exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    //     }
    //     return getCurrentToken();   // Vrátíme token pro další zpracování
    // }
    TokenType exprType = expression();                   // Parsování výrazu na pravé straně přiřazení
    if (exprType == T_NULL) {
        if (autoType) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_DERIVATION);
        }
        if (newNode->varData->dataType != T_F64_NULLABLE &&
            newNode->varData->dataType != T_I32_NULLABLE &&
            newNode->varData->dataType != T_U8_NULLABLE) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    else if (autoType) {
        newNode->varData->dataType = exprType;
    }
    else {
        if (!semcheck_compare_dtypes(newNode->varData->dataType, exprType)) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
    }
    // else if (!semcheck_compare_dtypes(newNode->varData->dataType, exprType)) {
    //     exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
    // }
    if (!insert_symbol(&symtable, newNode)) {
        exitWithError(&CurrentToken, ERR_SEM_REDEFINITION);
    }
    if (CurrentToken.type != T_SEMICOLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return getCurrentToken();       // Vrátíme token pro další zpracování
}

Token parse_return() {
    TokenType expRetVal = symtable.current->funDecl->funData->retType;
    getCurrentToken();
    if (CurrentToken.type == T_SEMICOLON) {
        if(expRetVal != T_VOID) {
            exitWithError(&CurrentToken, ERR_SEM_RETURN_EXPRESSION);
        }
        symtable.current->funDecl->funData->hasReturned = true;
        return getCurrentToken();   // Vrátíme token pro další zpracování
    }
    else {
        TokenType retVal = expression();               // Parsování výrazu, který se má vrátit
        if(!semcheck_compare_dtypes(expRetVal, retVal)) {
            exitWithError(&CurrentToken, ERR_SEM_INVALID_FUNC_PARAMS);
        }
        if (CurrentToken.type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        symtable.current->funDecl->funData->hasReturned = true;
        return getCurrentToken();   // Vrátíme token pro další zpracování
    }
}

Token parse_assignment_or_function_call() {
    if (CurrentToken.type == T_IFJ) {
        parse_standard_function_call();
        if (getCurrentToken().type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        return getCurrentToken();   // Vrátíme token pro další zpracování
    }
    char *id = CurrentToken.data.u8->data;
    if (search_symbol(&symtable, id) == NULL) {
        exitWithError(&CurrentToken, ERR_SEM_UNDEFINED_FUNC_VAR);
    }
    if (!search_symbol(&symtable, id)->isFun) {
        if (search_symbol(&symtable, id)->varData->isConst) {
            exitWithError(&CurrentToken, ERR_SEM_REDEFINITION);
        }
    }

    Token token = getCurrentToken();
    if (token.type != T_ASSIGN && token.type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (token.type == T_ASSIGN) {
        getCurrentToken();
        TokenType exprType = expression();               // Parsování výrazu na pravé straně přiřazení
        if (!semcheck_compare_dtypes(search_symbol(&symtable, id)->varData->dataType, exprType)) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (CurrentToken.type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    else {
        parse_function_call(id);
        if (CurrentToken.type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (getCurrentToken().type != T_SEMICOLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
    }
    return getCurrentToken();       // Vrátíme token pro další zpracování
}

char *parse_standard_function_call() {
    if (getCurrentToken().type != T_DOT) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = getCurrentToken();
    if (token.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    char *functionName = token.data.u8->data;
    if (!(strcmp(token.data.u8->data, "readstr") == 0 ||
        strcmp(token.data.u8->data, "readi32") == 0 ||
        strcmp(token.data.u8->data, "readf64") == 0 ||
        strcmp(token.data.u8->data, "write") == 0 ||
        strcmp(token.data.u8->data, "i2f") == 0 ||
        strcmp(token.data.u8->data, "f2i") == 0 ||
        strcmp(token.data.u8->data, "string") == 0 ||
        strcmp(token.data.u8->data, "length") == 0 ||
        strcmp(token.data.u8->data, "concat") == 0 ||
        strcmp(token.data.u8->data, "substrin") == 0 ||
        strcmp(token.data.u8->data, "strcmp") == 0 ||
        strcmp(token.data.u8->data, "ord") == 0 ||
        strcmp(token.data.u8->data, "chr") == 0)) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    parse_function_call(functionName);
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    return functionName;
}

void arguments(Token token) {
    if (token.type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_COLON) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    if (CurrentToken.type != T_I32_ID && 
        CurrentToken.type != T_F64_ID && 
        CurrentToken.type != T_U8_ID &&
        CurrentToken.type != T_I32_NULLABLE &&
        CurrentToken.type != T_F64_NULLABLE &&
        CurrentToken.type != T_U8_NULLABLE) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    tSymTabNode *newNode = create_var_node(true);
    if(newNode == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    newNode->id = token.data.u8->data;
    if (!init_insert_argument(newNode)) {
        exitWithError(&token, ERR_SEM_REDEFINITION);
    }
    getCurrentToken();
    while (CurrentToken.type != T_CLOSE_PARENTHESES) {
        if (CurrentToken.type != T_COMMA) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        getCurrentToken();
        if (CurrentToken.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else if (CurrentToken.type != T_ID) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        tSymTabNode *newNode = create_var_node(true);
        if(newNode == NULL) {
            exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
        }
        newNode->id = CurrentToken.data.u8->data;
        newNode->varData->isConst = true;
        newNode->varData->isUsed = false;
        if (!insert_symbol(&symtable, newNode)) {
            exitWithError(&CurrentToken, ERR_SEM_REDEFINITION);
        }

        if (getCurrentToken().type != T_COLON) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        getCurrentToken();
        if (CurrentToken.type != T_I32_ID && 
            CurrentToken.type != T_F64_ID && 
            CurrentToken.type != T_U8_ID &&
            CurrentToken.type != T_I32_NULLABLE &&
            CurrentToken.type != T_F64_NULLABLE &&
            CurrentToken.type != T_U8_NULLABLE) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        newNode->varData->dataType = CurrentToken.type;
        getCurrentToken();
    }
}

Token parse_function_definition() {
    //bool inMain = false;
    if (getCurrentToken().type != T_FN) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if (getCurrentToken().type != T_ID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    // if (CurrentToken.data.u8->data == "main") {
    //     //startMainGen();
    //     inMain = true;
    // }
    tFrame *frameTS = push_frame(&symtable, true);  // Začátek nového bloku - FUNKCE
    if(frameTS == NULL) {
        exitWithError(&CurrentToken, ERR_INTERNAL_COMPILER);
    }
    symtable.current->funDecl = search_symbol(&symtable, CurrentToken.data.u8->data);
    if (symtable.current->funDecl == NULL) {
        printf("where is the funcction?\n");
        exit(99);
    }
    if (getCurrentToken().type != T_OPEN_PARENTHESES) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    getCurrentToken();
    if (CurrentToken.type != T_CLOSE_PARENTHESES) {
        arguments(CurrentToken);            // Parsování argumentů funkce
    }
    getCurrentToken();
    if (CurrentToken.type != T_I32_ID && 
        CurrentToken.type != T_F64_ID && 
        CurrentToken.type != T_U8_ID &&
        CurrentToken.type != T_I32_NULLABLE &&
        CurrentToken.type != T_F64_NULLABLE &&
        CurrentToken.type != T_U8_NULLABLE &&
        CurrentToken.type != T_VOID) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    if(CurrentToken.type == T_VOID) {   // Void funkce nepotřebuje volat return
        symtable.current->funDecl->funData->hasReturned = true;
    }
    if (getCurrentToken().type != T_OPEN_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    Token token = code(getCurrentToken());  // Parsování těla funkce
    if (token.type != T_CLOSE_BRACKET) {
        exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
    }
    // if (inMain) {
    //     endMainGen();
    // }
    /**
     * VYSTUP Z FUNKCE => OVĚŘIT: 
     * zda byl volaný return
     * zda byly všechny proměnné použity
     */
    tFrame *tmpFrame = pop_frame(&symtable);
    if(!tmpFrame->funDecl->funData->hasReturned) {
        exitWithError(&token, ERR_SEM_OTHER); // Chybi navrat z funkce
    }
    if(!semcheck_var_usage(tmpFrame->symTable)) {
        exitWithError(&token, ERR_SEM_UNUSED_VAR);
    }
    dispose_frame(tmpFrame);
    return getCurrentToken();               // Vrátíme token pro další zpracování
}

void parse_function_call(char *id) {
    if (search_symbol(&symtable, id) == NULL) {
        exitWithError(&CurrentToken, ERR_SEM_UNDEFINED_FUNC_VAR);
    }
    if (!search_symbol(&symtable, id)->isFun) {
        exitWithError(&CurrentToken, ERR_SEM_OTHER);    // Volání funkce s id proměnné
    }
    getCurrentToken();
    int currentArgument = 0;
    if (strcmp(id, "write") == 0) {
        TokenType exprType = expression();
        if (exprType == T_NULL) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (CurrentToken.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else {
            if (CurrentToken.type != T_COMMA) {
                exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
            }
            if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
                expression();
                exitWithError(&CurrentToken, ERR_SEM_INVALID_FUNC_PARAMS);
            }
            return;
        }
    }
    else if (strcmp(id, "string") == 0) {
        TokenType exprType = expression();
        if (exprType != T_STRING_TYPE && exprType != T_STRING_TYPE_EMPTY && exprType != T_U8_ID) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        if (CurrentToken.type == T_CLOSE_PARENTHESES) {
            return;
        }
        else {
            if (CurrentToken.type != T_COMMA) {
                exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
            }
            if (getCurrentToken().type != T_CLOSE_PARENTHESES) {
                expression();
                exitWithError(&CurrentToken, ERR_SEM_INVALID_FUNC_PARAMS);
            }
            return;
        }
    }
    while (CurrentToken.type != T_CLOSE_PARENTHESES) {
        TokenType exprType = expression();                   // Parsování argumentů funkce
        if (!semcheck_compare_dtypes(search_symbol(&symtable, id)->funData->paramTypes[currentArgument], exprType)) {
            exitWithError(&CurrentToken, ERR_SEM_TYPE_COMPATIBILITY);
        }
        if (CurrentToken.type != T_COMMA && CurrentToken.type != T_CLOSE_PARENTHESES) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        else if (CurrentToken.type == T_COMMA) {
            getCurrentToken();
        }
        currentArgument++;
    }
    if (currentArgument != search_symbol(&symtable, id)->funData->paramCnt) {
        exitWithError(&CurrentToken, ERR_SEM_INVALID_FUNC_PARAMS);
    }
}

TokenType expression() {
    Stack stack;
    S_Init(&stack);
    Token Semicolon;
    Semicolon.type = T_SEMICOLON;
    PrecedenceToken bottom = tokenWrapper(Semicolon);
    S_Push(&stack, bottom);
    int parentheses = 0;        // pokud pocitadlo zavorek = -1, jsme na konci vyrazu
    bool endToken = false;      // posledni token
    bool exprEnd = false;       // konec vyrazu
    Token next_token;
    bool alreadyRead = false;

    next_token = CurrentToken;

    while(!exprEnd) {
        if (S_IsEmpty(&stack)) {
            exitWithError(&CurrentToken, ERR_SYNTAX_ANALYSIS);
        }
        PrecedenceToken *tokenTop = S_getTopTerminal(&stack);           // zjistime posledni terminal na zasobniku
        // pokud na zasobniku neni zadny terminal, overujeme zda jsme narazili na konec vyrazu
        // if (tokenTop == NULL) {
        //     if (checkExprEnd(&stack));
        //     return S_Top(&stack)->type;
        // }
        if (next_token.type == T_IFJ) {
            next_token.data.u8->data = parse_standard_function_call();
            next_token.type = T_ID;
            alreadyRead = false;
            continue;
        }
        if (alreadyRead == false && endToken == false) {                  // pokud nenarazime na posledni token ve vyrazu overujeme zda precteny token je posledni
            if (next_token.type == T_OPEN_PARENTHESES) {
                PrecedenceToken *function_call_check = S_getTopTerminal(&stack);
                if (function_call_check->token.type == T_ID) {            // pokud pred zavorkou je ID, jedna se o volani funkce
                    parse_function_call(function_call_check->token.data.u8->data);       // prepneme se na ll1 analyzu
                    next_token = getCurrentToken();
                    alreadyRead = false;
                    continue;                                             // na zasobniku zustane pouze id funkce a nikoliv zavorka a jeji argumenty
                }
                parentheses++;                                            // pokud narazime na token "(", zvysime pocitadlo zavorek 
            } else if (next_token.type == T_CLOSE_PARENTHESES) {          // pokud narazime na token ")", zmensime pocitadlo zavorek
                parentheses--;
            }
            if (parentheses < 0 || next_token.type == T_SEMICOLON || next_token.type == T_COMMA || next_token.type == T_OPEN_BRACKET) { // pokud je pocitadlo zavorek mensi nez 0, jedna se o konec vyrazu
                endToken = true;
                next_token.type = T_SEMICOLON;
            }
        }
        else {
            if (next_token.type == T_SEMICOLON) {                         // overime zda mame zredukovany cely vyraz
                if(checkExprEnd(&stack)) {
                    return S_Top(&stack)->type;
                }
            }
        }
        
        if (next_token.type == T_ID) {
            tSymTabNode *idTS = search_symbol(&symtable, next_token.data.u8->data);
            if (idTS == NULL) {
                exitWithError(&next_token, ERR_SEM_UNDEFINED_FUNC_VAR);
            }
            else {
                if (!idTS->isFun) {
                    idTS->varData->isUsed = true;
                }
            }
        }
        
        // porovnavame priority posledniho terminalu na zasobniku s prave nactenym tokenem
        if (getOperatorIndex(next_token)==-1) {
            exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
        }
        char relation = precedenceTable[getOperatorIndex(tokenTop->token)][getOperatorIndex(next_token)];
        if (relation==EMPTY) {
            exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
        }
        if (relation == '>') {
            if (S_topTerminalReduce(&stack)) {       // vrati znak pred poslednim terminalem na zasobniku pro overeni redukce
                ruleReduce(&stack, &symtable);       // provede redukci
                alreadyRead = true;
                continue;                            // pokud lze provest redukci, pokracujeme dal se stejnym tokenem na vstupu
            }
            else {
                exitWithError(&next_token, ERR_SYNTAX_ANALYSIS);
            }
        }
        else if (relation == '=') {
            PrecedenceToken pushToken = tokenWrapper(next_token);
            S_Push(&stack, pushToken);
        } 
        else if (relation == '<') {
            tokenTop->reduction = true;
            PrecedenceToken pushToken = tokenWrapper(next_token);
            S_Push(&stack, pushToken);
        } 
        next_token = getCurrentToken();
        alreadyRead = false;
    }
    return S_Top(&stack)->type;
}

int main() {
    init_list_of_tokens(&LIST);

    init_frame_stack(&symtable);
    push_frame(&symtable, false);
    global_symtable();  

    i_want_to_get_tokens(&LIST);

    // start_gen();        // OVERIT USPESNOST ALOKACE

    syntax_analysis();  // Spustí se syntaktická analýza

    // end_gen();
    free_list_of_tokens(&LIST);
    return 0;
}