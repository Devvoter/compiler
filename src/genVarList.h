/**
 * @file genVarList.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief deklarace listu a operaci s nim pro ukladani definovanych promennych pro generator
 * 
 * @date 04.12.2024
 */

#include <stdbool.h>

/**
 * @brief struktura elementu pro ukladani definovane promenne
 */
typedef struct varListElem {
    char *varName;
    struct varListElem *next;
} *varListElemPtr;

/**
 * @brief struktura jednosmerneho seznamu pro ukladani definovanych promennych
 */
typedef struct {
    varListElemPtr first;
    varListElemPtr active;
} varList;

/**
 * @brief inicializuje list s definovanymi parametry
 * 
 * @param l ukazatel na strukturu listu
 */
bool varListInit(varList** l);

/**
 * @brief prida nazev parametru do listu
 * 
 * @param l ukazatel na strukturu listu
 * @param varName ukazatel na nazev promenne
 */
bool varListInsert(varList** l, char *varName);

/**
 * @brief overi, zda dany list obsahuje nazev urcite promenne, v pripade true obsahuje
 * 
 * @param l ukazatel na strukturu listu
 * @param varName ukazatel na nazev promenne
 */
bool isInVarList(varList** l, char *varName);

/**
 * @brief uvolni alokovanou pamet a smaze list
 */
void destroyVarList(varList* l);
