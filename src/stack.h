/**
 * @file stack.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief Implementace zásobníku, který ukládá data typu void *
 * 
 * @date 2024-10-21
 * 
 */

#ifndef _STACK_H_
#define _STACK_H_

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


#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Struktura prvka
 * 
 * 
 */
typedef struct StackItem {

    void             *data;  // < ukazatel na data 
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
void S_Init(Stack *s);


/**
 * @brief Funkce pro přidání dat na vrchol zásobníku
 * 
 * @param s ukazatel na zásobník
 * @param data ukazatel na data
 * @return true pokud operace byla úspěšná
 * @return false pokud operace selhala
 */
bool S_Push(Stack *s, void *data);


/**
 * @brief Funkce pro odstranění prvku na vrcholu zásobníku
 * 
 * @param s ukazatel na zásobník
 */
void S_Pop(Stack *s);


/**
 * @brief Funkce pro získání dat z vrcholu zásobníku
 * 
 * @param s ukazatel na zásobník
 * @return void* ukazatel na data uložená ve vrcholu zásobníku nebo NULL pokud je zásobník prázdný
 */
void *S_Top(Stack *s);


/**
 * @brief Funkce pro kontrolu, zda je zásobník prázdný
 * 
 * @param s ukazatel na zásobník
 * @return true pokud je zásobník prázdný
 * @return false pokud zásobník není prázdný
 */
bool S_IsEmpty(Stack *s);

#endif //_STACK_H_


