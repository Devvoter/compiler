/**
 * @file stack.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-11-20
 * 
 */

#ifndef _STACK_H_
#define _STACK_H_


#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Struktura prvka
 * 
 * 
 */
typedef struct StackItem {

    int data;    // struktura s tokenem
    struct StackItem *next;  // < ukazatel na dálší prvek

} StackItem;

/**
 * @brief Struktura zásobníku
 * 
 * 
 */
typedef struct Stack {
    StackItem *top;          // < ukazatel na první prvek
} Stack;


/**
 * @brief Inicializace nového zásobníku
 * 
 * @param s ukazatel na zásobník
 */
void CodeStack_Init(Stack *s);


/**
 * @brief Funkce pro přidání dat na vrchol zásobníku
 * 
 * @param s ukazatel na zásobník
 * @param data data
 * @return true pokud operace byla úspěšná
 * @return false pokud operace selhala
 */
bool CodeStack_Push(Stack *s, int data);


/**
 * @brief Funkce pro odstranění prvku na vrcholu zásobníku
 * 
 * @param s ukazatel na zásobník
 */
void CodeStack_Pop(Stack *s);


/**
 * @brief Funkce pro získání dat z vrcholu zásobníku
 * 
 * @param s ukazatel na zásobník
 * @return ukazatel na data uložená ve vrcholu zásobníku nebo NULL pokud je zásobník prázdný
 */
int *CodeStack_Top(int *s);


/**
 * @brief Funkce pro kontrolu, zda je zásobník prázdný
 * 
 * @param s ukazatel na zásobník
 * @return true pokud je zásobník prázdný
 * @return false pokud zásobník není prázdný
 */
bool CodeStack_IsEmpty(Stack *s);

#endif //_STACK_H_


