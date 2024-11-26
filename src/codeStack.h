/**
 * @file stack.h
 * @author Denys Pylypenko (xpylypd00), Polina Ustiuzhantseva (xustiup00)
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

typedef enum {
    T_COUNTER,
    T_WHILE_IS_NULLABLE
} ELEM_TYPE;


typedef struct StackItem {
    void *data;    
    ELEM_TYPE t;
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
void CodeStack_Init(Stack **s);


/**
 * @brief Funkce pro přidání dat na vrchol zásobníku
 * 
 * @param s ukazatel na zásobník
 * @param data data
 * @param t typ zapisovane promennych dat (bud int nebo pole znaku)
 * @return true pokud operace byla úspěšná
 * @return false pokud operace selhala
 */
bool CodeStack_Push(Stack *s, void *data, ELEM_TYPE t);


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
StackItem *CodeStack_Top(Stack *s);


/**
 * @brief Funkce pro kontrolu, zda je zásobník prázdný
 * 
 * @param s ukazatel na zásobník
 * @return true pokud je zásobník prázdný
 * @return false pokud zásobník není prázdný
 */
bool CodeStack_IsEmpty(Stack *s);

/**
 * @brief Funkce uvolni pamet stacku
 * 
 * @param s ukazatel na zásobník
 */
void CodeStack_destroy(Stack *s);

#endif //_STACK_H_
