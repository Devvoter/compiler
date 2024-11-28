/**
 * @file stack.c
 * @author Denys Pylypenko (xpylypd00), Polina Ustiuzhantseva (xustiup00)
 * @brief
 * 
 * @date 2024-11-20
 * 
 */

#include "codeStack.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CodeStack_Init(Stack **s) {
    *s = malloc(sizeof(Stack));
    (*s)->top = NULL;
}

bool CodeStack_Push(Stack *s, void *data, ELEM_TYPE t) {
    StackItem *new_item = (StackItem *) malloc(sizeof(StackItem));
    if (new_item == NULL) return false;

    if (t == T_WHILE_IS_NULLABLE) {                                 //zapisujeme char *
        new_item->data = malloc(sizeof(strlen((char *)data)) + 1);
        if (new_item->data == NULL) {
            return false;
        }
        strcpy(new_item->data, (char *) data);
    } else {                                                        //zapisujeme int 
        new_item->data = malloc(sizeof(int));
        if (new_item->data == NULL) {
            return false;
        }
        *(int *)new_item->data = *((int *)data);
    }
    new_item->next = s->top; // posunout aktuální vrchol jako další prvek
    s->top = new_item; // nastavit nový prvek jako vrchol zásobníku
    return true;
}

void CodeStack_Pop(Stack *s) {
    StackItem *tmp;
    if (s->top != NULL) { // pokud prvek existuje
        tmp = s->top;
        s->top = s->top->next; // nastavit další prvek jako nový vrchol zásobníku
        free(tmp->data);
        free(tmp);
    }
}

StackItem *CodeStack_Top(Stack *s) {
    if (s->top != NULL) return (s->top);
    return NULL;
}

bool CodeStack_IsEmpty(Stack *s) {
    return (s->top == NULL);
}

void CodeStack_destroy(Stack *s) {
    while (!CodeStack_IsEmpty(s)) {
        CodeStack_Pop(s);
    }
    free(s);
}