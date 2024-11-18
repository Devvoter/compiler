/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice struktur funkci volani pro generovani kodu
 * 
 * @date 1.11.2024
 */

#include <stdbool.h>
#include <stdio.h>


typedef enum {
    variable_t,    
    string_t,
    null_t,
    float_t,
    bool_t,
    int_t
} assignType;

/**
 * @brief vytvori zacatek kodu IFJcode24
 */
bool startGen();

/**
 * @brief vytvori konec kodu IFJcode24 
 */
bool endGen();

/**
 * @brief uvolni veskerou pamet, alokovanou pro generator,
 * zachodi vytvoreny kod
 * 
 * @param done v pripade true vypise kod na stdout
 */
void disposeGen(bool done);

/**
 * @brief vytvori zacatek funkce main 
 */
bool startMainGen();

/**
 * @brief vytvori konec funkce main
 */
bool endMainGen();

/**
 * @brief vytvori novou promennou
 */
bool defVarGen(char *ID);

/**
 * @brief definuje promennou
 */
bool assignVarGen(char *ID, assignType type, char *value);

/**
 * @brief volani vestavene funkce
 * @param assign vesledek funkce se priradi promenne pokud true
 * 
 */
bool callStandFuncGen(bool assign, char *ID, char *params);

/**
 * @brief zacatek zpracovani vyrazu, hodnota ktereho se prirazuje k promenne
 */
bool startAssignGen(char *ID);

/**
 * @brief jeden krok zpracovani vyrazu
 */
bool addTermGen(char *operator, char *operand1, char *operand2, char *result);

/**
 * @brief konec zpracovani vyrazu
 */
bool endAssignGen();


