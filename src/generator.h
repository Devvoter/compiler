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
 */
bool assignVarGen(char *ID, TokenType t, char *value);

/**
 * @brief volani vestavene funkce
 * 
 * @param assign vesledek funkce se priradi promenne pokud true
 * @param ID promenna, kam se priradi vysledek(pripadne NULL)
 * @param param1 prvni parametr standardni funkce(pripadne NULL)
 * @param param2 druhy parametr standardni funkce(pripadne NULL)
 */
bool callStandFuncGen(bool assign, char *ID, char *param1, char *param2, char *param3);


/**
 * @brief prida na datovy zasobnik hodnotu
 * 
 * @param ID hodnota 
 * @param t typ posilane hodnoty
 */
bool pushOnStackGen(char *ID, TokenType t);

/**
 * @brief provede operaci s hodnotami na zasobniku 
 * 
 * @param t typ operace, ktera se ma provest
 */
bool makeOperationStackGen(TokenType t);

/**
 * @brief konec zpracovani vyrazu, pripsani hodnoty z datoveho zasobniku promenne
 */
bool endAssignGen(char *ID);




