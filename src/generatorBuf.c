/**
 * @file generatorBuf.c
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief implementace funkci pro buffer kodu IFJcode24
 * 
 * @date 18.11.2024
 */

#include "generatorBuf.h"
#include "error.h"

#include <stdio.h>
#include <stdbool.h>

bool bufInit(codeBuf **buffer)
{
    *buffer = malloc(sizeof(codeBuf));
    if (buffer == NULL)
    {
        return false;
    }
    // pridani uvodniho radku
    codeBufElemPtr firstElem = malloc(sizeof(struct codeBufElem));
    if (firstElem == NULL)
    {
        return false;
    }
    firstElem->code = ".IFJcode24";
    firstElem->print_t = T_OTHERS;
    firstElem->next = NULL;
    firstElem->previous = NULL;
    (*buffer)->first = firstElem;
    (*buffer)->active = firstElem;
    return true;
}

void bufPrint(codeBuf **buffer) {
    (*buffer)->active = (*buffer)->first;
        while ((*buffer)->active != NULL) {
            if ((*buffer)->active->print_t == T_FLOAT) {
                double f = atof((*buffer)->active->code);
                fprintf(stdout, "%a", f);
            } else if ((*buffer)->active->print_t == T_INT) {
                fprintf(stdout, "%d", *(int*)(*buffer)->active->code);
            } else {
                fprintf(stdout, "%s", (char*)(*buffer)->active->code);
            }
            (*buffer)->active = (*buffer)->active->next;
        }
}

void bufDestroy(codeBuf *buffer)
{
    if (buffer == NULL || buffer->first == NULL)
        return;

    codeBufElemPtr elem = buffer->first;
    while (elem != NULL)
    {
        codeBufElemPtr tmp = elem;
        elem = elem->next;
        if (tmp->print_t == T_STRING || tmp->print_t == T_INT || tmp->print_t == T_STRING_FROM_PARSER || tmp->print_t == T_FLOAT) {
            free(tmp->code);
        }
        free(tmp);
    }
    free(buffer);
}

bool addCodeToBuf(codeBuf **buffer, void *str, PRINT_TYPE print_t, bool while_t) {
    codeBufElemPtr elem = malloc(sizeof(struct codeBufElem));
    if (elem == NULL)
    {
        return false;
    }
    if (print_t == T_INT) {
        elem->code = malloc(sizeof(int));
        if (elem->code == NULL) {
            return false;
        }
        *(int *)elem->code = *((int *)str);
    } else {
        elem->code = str;
    }
    elem->print_t = print_t;
    elem->while_t = while_t;
    elem->next = NULL;
    elem->previous = (*buffer)->active;
    (*buffer)->active->next = elem;
    (*buffer)->active = elem;
    return true;
}

bool addVarBeforeWhile(codeBuf **buffer, void *str, PRINT_TYPE print_t) {
    codeBufElemPtr foundWhile = (*buffer)->active;
    //najdeme kod se zacatkem aktualniho while
    while (foundWhile != NULL) {
        if (foundWhile->while_t == true) {
            break;
        } else {
            foundWhile = foundWhile->previous;
        }
    }
    if (foundWhile == NULL) {
        return false;
    }
    codeBufElemPtr elem = malloc(sizeof(struct codeBufElem));
    if (elem == NULL)
    {
        return false;
    }
    elem->code = str;
    elem->print_t = print_t;
    elem->while_t = false;
    elem->next = foundWhile;
    elem->previous = foundWhile->previous;
    foundWhile->previous->next = elem;
    foundWhile->previous = elem;
}


