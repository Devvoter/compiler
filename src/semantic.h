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
extern tFrameStack *Symtable;
extern tSymTabNode *CurrentSymbol;

bool semcheck_in_global() {
    if (Symtable->current == Symtable->global) {
        return true;
    }
    return false;
}

bool semcheck_is_const() {
    if (CurrentSymbol->varData->isConst == true) return true;
    return false;
}

bool semcheck_assign_string() {
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

//bool data_type_compatible(tVar var, Token *token) {}


/* Konec souboru symtable.c */