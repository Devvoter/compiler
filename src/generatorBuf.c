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
    firstElem->next = NULL;
    (*buffer)->first = firstElem;
    (*buffer)->active = firstElem;
    return true;
}

void bufPrint(codeBuf **buffer) {
    (*buffer)->active = (*buffer)->first;
        while ((*buffer)->active != NULL) {
            if ((*buffer)->active->hex_form == true) {
                double f =atof((*buffer)->active->code);
                fprintf(stdout, "%a", f);
            } else {
                fprintf(stdout, "%s", (*buffer)->active->code);
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
        free(tmp);
    }
    free(buffer);
}

bool addCodeToBuf(codeBuf **buffer, char *str, bool f) {
    codeBufElemPtr elem = malloc(sizeof(struct codeBufElem));
    if (elem == NULL)
    {
        return false;
    }
    elem->code = str;
    elem->hex_form = f;
    (*buffer)->active->next = elem;
    (*buffer)->active = elem;
    elem->next = NULL;
    return true;
}

