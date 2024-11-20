/**
 * Project : Prekladac imperativniho jazyka IFJ24
 * @file semantic.c
 * @brief Funkce pro sémantickou analýzu
 * 
 * @author Michaela Capíková (xcapikm00)
 * @date 2024-11-17
 */

#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

extern Token CurrentToken;
extern tFrameStack symtable;
extern tSymTabNode *CurrentSymbol;

bool semcheck_in_global() {
    if (symtable.current == symtable.global) {
        return true;
    }
    return false;
}

bool semcheck_is_const() {
    if (CurrentSymbol->varData->isConst == true) return true;
    return false;
}

bool semcheck_define_string() {
    tVar *var = CurrentSymbol->varData;
    if (var->dataType == T_UNKNOW) {
        var->dataType = T_U8_ID;
    }
    if (var->dataType == T_U8_ID || var->dataType == T_U8_NULLABLE) {
        var->isDef = true;
        return true;
    }
    else return false;
}

/**
 * @brief Když je ve výrazu porovnávací operátor, jeho hodnota bude boolovského typu
 * @param opIdx getOperatorIndex(Token token)
 * @return True když je prvek výrazu porovnávací operátor
 */
bool semcheck_bool_expr(int opIdx) {
    if (opIdx >= 4 && opIdx <= 9) return true;
    return false;
}

/**
 * @brief Funkce inicializuje symbol pro argument v hlavičke funkce a vloží ho do tabulky
 * @return True při úspěšném vložení. False 
 */
bool init_insert_argument() {
    if(!insert_symbol(&symtable, CurrentSymbol)) return false;
    CurrentSymbol->varData->isConst = true;
    CurrentSymbol->varData->isDef = true;
    CurrentSymbol->varData->dataType = CurrentToken.type;
    return true;
}

bool semcheck_var_usage(tSymTabNode *root) {
    if(root != NULL) {
        if(!root->varData->isUsed) return false;
        if(!semcheck_var_usage(root->left)) return false;
        if(!semcheck_var_usage(root->right)) return false;
    }
    return true;
}

/* Konec souboru semantic.h */