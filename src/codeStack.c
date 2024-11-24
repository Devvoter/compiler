/**
 * @file stack.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-11-20
 * 
 */

#include "codeStack.h"

void CodeStack_Init(Stack **s) {
    *s = malloc(sizeof(Stack));
    (*s)->top = NULL;
}

bool CodeStack_Push(Stack *s, int data) {
    StackItem *new_item = (StackItem *) malloc(sizeof(StackItem));
    if (new_item == NULL) return false;

    new_item->data = data;
    new_item->next = s->top; // posunout aktuální vrchol jako další prvek
    s->top = new_item; // nastavit nový prvek jako vrchol zásobníku

    return true;
}

void CodeStack_Pop(Stack *s) {
    StackItem *tmp;
    if (s->top != NULL) { // pokud prvek existuje
        tmp = s->top;
        s->top = s->top->next; // nastavit další prvek jako nový vrchol zásobníku
        free(tmp);
    }
}

int *CodeStack_Top(Stack *s) {
    if (s->top != NULL) return &(s->top->data);
    return NULL;
}

bool CodeStack_IsEmpty(Stack *s) {
    return (s->top == NULL);
}

void CodeStack_destroy(Stack *s) {
    free(s);
}