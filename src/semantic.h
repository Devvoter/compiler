/**
 * Project : Prekladac imperativniho jazyka IFJ24
 * @file semantic.h
 * @brief Funkce pro sémantickou analýzu
 * 
 * @author Michaela Capíková (xcapikm00)
 * @date 2024-11-17
 */

#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>

extern Token CurrentToken;
extern tFrameStack symtable;
extern tSymTabNode *CurrentSymbol;

/**
 * @brief Funkce kontroluje, zda se právě nacházime v globálním bloku
 * @return True když se právě nacházime v globálním bloku, jinak false
 */
bool semcheck_in_global() {
    if (symtable.current == symtable.global) {
        return true;
    }
    return false;
}

/**
 * @brief Funkce kontroluje, zda je proměnná konstantní
 * @return True, když je proměnná konstantní
 */
bool semcheck_is_const(tSymTabNode *symNode) {
    if (symNode->varData->isConst == true) return true;
    return false;
}

// /**
//  * @brief Funkce kontroluje přiřazení řeťezce do proměnné
//  * @return False, když datové typy nesedí, jinak true
//  */
// bool semcheck_define_string(tSymTabNode *symNode) {
//     tVar *var = symNode->varData;
//     if (var->dataType == T_UNKNOW) {
//         var->dataType = T_U8_ID;
//     }
//     if (var->dataType == T_U8_ID || var->dataType == T_U8_NULLABLE) {
//         return true;
//     }
//     else return false;
// }

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
 * @return True při úspěšném vložení. False když se našel duplicitní ID
 */
bool init_insert_argument(tSymTabNode *symNode) {
    if(!insert_symbol(&symtable, symNode)) return false;
    symNode->varData->isConst = true;
    symNode->varData->dataType = CurrentToken.type;
    return true;
}

/**
 * @brief Funkce kontroluje, zda byly všechny proměnné v daném bloku použite (příznak isUsed)
 * @param root Ukazovatel na tabulku symbolú 1 bloku
 * @return False, když je v bloku nevyužitá proměnná, jiak true
 */
bool semcheck_var_usage(tSymTabNode *root) {
    if(root != NULL) {
        if(!root->varData->isUsed) return false;
        if(!semcheck_var_usage(root->left)) return false;
        if(!semcheck_var_usage(root->right)) return false;
    }
    return true;
}

/**
 * @brief Funkce pro kontrolu datových typú u přiřazení hodnoty výrazu/volání funkce
 * !!! Neošetřuje, případ když DT proměnné není známy
 * @param expected Datový typ proměnné, do které přiřazujeme, nebo očekávaný DT příkazu return
 * @param assigned DT hodnoty výrazu/výsledku volání funkce
 * @return True když jsou DT kompatibilní
 */
bool semcheck_compare_dtypes(TokenType expected, TokenType assigned) {
    //case T_I32_NULLABLE:
    //case T_F64_NULLABLE:
    //case T_U8_NULLABLE:
    //case T_VOID:
    //case T_COMPARISON:
    if(expected == assigned) return true; // Vše zhodné automaticky true
    switch (assigned) {
        case T_I32_ID:
            if(expected == T_I32_NULLABLE) return true;
        case T_F64_ID:
            if(expected == T_F64_NULLABLE) return true;
        case T_U8_ID:
            if(expected == T_U8_NULLABLE) return true;
        case T_NULL: // Toto neviem ci je spravne...
            if(expected == T_I32_NULLABLE || expected == T_F64_NULLABLE || expected == T_U8_NULLABLE) return true;
            else return false;
        case T_I32_VAR:
            if(expected == T_I32_ID || expected == T_U8_NULLABLE) return true;
        case T_F64_VAR:
            if(expected == T_F64_ID || expected == T_F64_NULLABLE) return true;
        case T_STRING_TYPE:
            if(expected == T_U8_ID || expected == T_U8_NULLABLE) return true;
        case T_STRING_TYPE_EMPTY:
            if(expected == T_U8_ID || expected == T_U8_NULLABLE) return true;
        default:
            return false;
    }
}
/* Konec souboru semantic.h */

#endif