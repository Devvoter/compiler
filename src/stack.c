/**
 * @file stack.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-10-21
 * 
 */

/*************************************************************************************************
 * Příklad použití zásobníku s prvky různých datových typů                                       *
 *                                                                                               *
 *   int integerValue = 42;                 // hodnota celého čísla                              *
 *   S_Push(&stack, &integerValue);         // přidání hodnoty do zásobníku                      *
 *   int *intPtr = (int *) S_Top(&stack);   // převést ukazatel na (int *)  !!!!!                *
 *   S_Pop(&stack);                         // Odebrání prvku z vrcholu zásobníku                *
 *                                                                                               *
 * !!! Pro vyjmutí dat je třeba provést přetypování ukazatele (void *) na původní datový typ.    *
 *                                                                                               *
 *************************************************************************************************/


#include "stack.h"

void S_Init(Stack *s) {
    s->top = NULL;
}

bool S_Push(Stack *s, void *data) {
    StackItem *new_item = (StackItem *) malloc(sizeof(StackItem));
    if (new_item == NULL) return false;

    new_item->data = data;
    new_item->next = s->top; // posunout aktuální vrchol jako další prvek
    s->top = new_item; // nastavit nový prvek jako vrchol zásobníku

    return true;
}

void S_Pop(Stack *s) {
    StackItem *tmp;
    if (s->top != NULL) { // pokud prvek existuje
        tmp = s->top;
        s->top = s->top->next; // nastavit další prvek jako nový vrchol zásobníku
        free(tmp);
    }
}

void *S_Top(Stack *s) {
    if (s->top != NULL) return (s->top->data);
    return NULL;
}

bool S_IsEmpty(Stack *s) {
    return (s->top == NULL);
}