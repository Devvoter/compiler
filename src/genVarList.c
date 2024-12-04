/**
 * @file genVarList.c
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief implementace operaci s nim pro ukladani definovanych promennych pro generator
 * 
 * @date 04.12.2024
 */

#include "genVarList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool varListInit(varList** l) {
    *l = malloc(sizeof(varList));
    if (l == NULL) {
        return false;
    }
    (*l)->active = NULL;
    (*l)->first = NULL;
    return true;
}

bool varListInsert(varList** l, char *varName) {
    varListElemPtr elem = malloc(sizeof(struct varListElem));  
    if (elem == NULL) {
        return false;
    }
    int length = strlen(varName);
    elem->varName = malloc(length + 1);
    if (elem->varName == NULL) {
        return false;
    }
    strcpy(elem->varName, varName);
    elem->next = NULL;
    if ((*l)->first == NULL) {
        (*l)->first = elem;
        (*l)->active = elem;
    } else {
        (*l)->active->next = elem;
        (*l)->active = elem;
    }
    return true;
}

bool isInVarList(varList** l, char *varName) {
    if ((*l)->active == NULL || (*l)->first == NULL) {
        return false;
    } else {
        varListElemPtr tmp = (*l)->first;
        while (tmp != NULL) {
            if (strcmp(tmp->varName, varName) != 0) {
                continue;                              //pokud stringy nemaji stejny pocet znaku
            }
            bool theSame = true;
            bool lastChar = true;
            int i = 0;
            while (tmp->varName[i] != '\0') {
                if (tmp->varName[i] == varName[i]) {
                    continue;
                } else {
                    lastChar = false;
                    break;
                }
                i++;
            }
            theSame = theSame && lastChar;
            if (theSame) {
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }
}

void destroyVarList(varList* l) {
    if (l == NULL) 
        return;

    varListElemPtr elem = l->first;
    while (elem != NULL) {
        varListElemPtr tmp = elem;
        elem = elem->next;
        free(tmp->varName);
        free(tmp);
    }
    free(l);
}