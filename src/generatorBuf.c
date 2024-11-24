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
    firstElem->t = T_OTHERS;
    firstElem->next = NULL;
    (*buffer)->first = firstElem;
    (*buffer)->active = firstElem;
    return true;
}

void bufPrint(codeBuf **buffer) {
    (*buffer)->active = (*buffer)->first;
        while ((*buffer)->active != NULL) {
            if ((*buffer)->active->t == T_FLOAT) {
                double f = atof((*buffer)->active->code);
                fprintf(stdout, "%a", f);
            } else if ((*buffer)->active->t == T_INT) {
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
        if (tmp->t == T_STRING) {
            free(tmp->code);
        }
        free(tmp);
    }
    free(buffer);
}

bool addCodeToBuf(codeBuf **buffer, void *str, PRINT_TYPE t) {
    codeBufElemPtr elem = malloc(sizeof(struct codeBufElem));
    if (elem == NULL)
    {
        return false;
    }
    elem->code = str;
    elem->t = t;
    (*buffer)->active->next = elem;
    (*buffer)->active = elem;
    elem->next = NULL;
    return true;
}

