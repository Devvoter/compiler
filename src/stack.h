/**
 * @file stack.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief Implementace zásobníku, který ukládá data typu PrecedenceToken
 * 
 * @date 2024-10-21
 * 
 */

#ifndef _STACK_H_
#define _STACK_H_

/*************************************************************************************************
 * Příklad použití zásobníku s prvky různých datových typů                                       *
 *                                                                                               *
 *   PrecedenceToken token;                      // hodnota celého čísla                         *
 *   S_Push(&stack, token);                      // přidání hodnoty do zásobníku                 *
 *   PrecedenceToken *tokenPtr = S_Top(&stack);  // Vratí ukazatel na prvek z vrcholu zásobníku  *
 *   S_Pop(&stack);                              // Odebrání prvku z vrcholu zásobníku           *
 *                                                                                               *
 *************************************************************************************************/


#include <stdlib.h>
#include <stdbool.h>
#include "precedence_token.h"

/**
 * @brief Struktura prvka
 * 
 * 
 */
typedef struct StackItem {

    PrecedenceToken data;    // struktura s tokenem
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
 * @param data data
 * @return true pokud operace byla úspěšná
 * @return false pokud operace selhala
 */
bool S_Push(Stack *s, PrecedenceToken data);


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
 * @return ukazatel na data uložená ve vrcholu zásobníku nebo NULL pokud je zásobník prázdný
 */
PrecedenceToken *S_Top(Stack *s);


/**
 * @brief Funkce pro kontrolu, zda je zásobník prázdný
 * 
 * @param s ukazatel na zásobník
 * @return true pokud je zásobník prázdný
 * @return false pokud zásobník není prázdný
 */
bool S_IsEmpty(Stack *s);

#endif //_STACK_H_


