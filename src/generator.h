/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice funkci pro generovani kodu
 * 
 * @date 1.11.2024
 */

#include <stdbool.h>
#include <stdio.h>

#include "scanner.h"

typedef enum {
    T_WRITE,
    T_READSTR,
    T_READI32,
    T_READF64,
    T_STRING,
    T_LENGTH,
    T_CONCAT,
    T_SUBSTRING,
    T_STRCMP,
    T_ORD,
    T_CHR,
    T_I2F,
    T_F2I,
} standFunc_t;


/**
 * @brief vytvori zacatek kodu IFJcode24
 */
bool startGen();

/**
 * @brief vytvori konec kodu IFJcode24 
 */
bool endGen();

/**
 * @brief uvolni veskerou pamet, alokovanou pro generator, zachodi vytvoreny kod
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
 * @param LF v pripade true, promenna se zapise do lokalniho framu
 */
bool defVarGen(char *ID, bool LF);

/**
 * @brief definuje promennou
 * @param ID nazev promenne
 * @param t typ hodnoty
 * @param value hodnota
 * @param fromLF v pripade true pripisuje hodnotu z LF
 * @param toLF v pripade true pripisuje do LF
 */
bool assignVarGen(char *ID, TokenType t, char *value, bool fromLF, bool toLF);

/**
 * @brief volani vestavene funkce
 * 
 * @param t typ volane funkce 
 * @param assign vesledek funkce se priradi promenne pokud true
 * @param pushOnStack zapise vysledek na datovy zasobnik
 * @param ID promenna, kam se priradi vysledek(pripadne NULL)
 * @param param1 prvni parametr standardni funkce(pripadne NULL)
 * @param param2 druhy parametr standardni funkce(pripadne NULL)
 */
bool callStandFuncGen(standFunc_t t ,bool assign, bool pushOnStack, char *ID, char *param1, char *param2, char *param3);

/**
 * @brief volani pomocne funkce
 * @param name nazev funkce
 */
bool callFuncGen(char *name); //TODO doplnit parametry(parametry funkce, jejich pocet)

/**
 * @brief pripsani navratove hodnoty z pomocne funkce do promenne, pripadne na zasobnik
 * @param ID nazev promenne, kam se ma ulozit hodnota
 * @param pushOnStack v pripade true, prida se hodnota na vrchol zasobniku
 */
bool retValGen(char *ID, bool pushOnStack);

/**
 * @brief vygeneruje zacatek pomocne funkce 
 * @param 
 */
bool FuncStartGen(char *name); //TODO parametrs

/**
 * @brief vygeneruje konec pomocne funkce 
 */
bool FuncEndGen();

/**
 * @brief prida na datovy zasobnik hodnotu
 * 
 * @param ID hodnota 
 * @param t typ posilane hodnoty
 */
bool pushOnStackGen(char *ID, TokenType t);  //TODO S na dne zasobniku?

/**
 * @brief provede operaci s hodnotami na zasobniku 
 * 
 * @param t typ operace, ktera se ma provest
 */
bool makeOperationStackGen(TokenType t);

/**
 * @brief konec zpracovani vyrazu, pripsani hodnoty z datoveho zasobniku do promenne
 */
bool endAssignGen(char *ID);




