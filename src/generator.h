/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice funkci pro generovani kodu
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
} t_Type;

typedef enum {
    plus,
    minus,
    mul,
    div
} t_operationType;

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
 * @param ID nazev promenne
 */
bool defVarGen(char *ID);

/**
 * @brief definuje promennou
 * @param ID nazev promenne
 * @param t typ hodnoty
 * @param value hodnota
 */
bool assignVarGen(char *ID, t_Type t, char *value);

/**
 * @brief volani vestavene funkce
 * 
 * @param assign vesledek funkce se priradi promenne pokud true
 * @param ID promenna, kam se priradi vysledek(pripadne NULL)
 * @param param1 prvni parametr standardni funkce(pripadne NULL)
 * @param param2 druhy parametr standardni funkce(pripadne NULL)
 */
bool callStandFuncGen(bool assign, char *ID, char *param1, char *param2);


/**
 * @brief prida na datovy zasobnik hodnotu
 * 
 * @param ID hodnota 
 * @param t typ posilane hodnoty
 */
bool pushOnStackGen(char *ID, t_Type t);

/**
 * @brief provede operaci s hodnotami na zasobniku 
 * 
 * @param t typ operace, ktera se ma provest
 */
bool makeOperationStackGen(t_operationType t);

/**
 * @brief konec zpracovani vyrazu, pripsani hodnoty z datoveho zasobniku promenne
 */
bool endAssignGen(char *ID);




