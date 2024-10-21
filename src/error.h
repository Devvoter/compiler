/**
 * @file error.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-10-21
 * 
 */

#ifndef _ERROR_H_
#define _ERROR_H_

#include "scanner.h"

#define ERR_LEXICAL_ANALYSIS          1         // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
#define ERR_SYNTAX_ANALYSIS           2         // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.).
#define ERR_SEM_UNDEFINED_FUNC_VAR    3         // sémantická chyba v programu – nedefinovaná funkce či proměnná.
#define ERR_SEM_INVALID_FUNC_PARAMS   4         // sémantická chyba v programu – špatný počet/typ parametrů u volání funkce; špatný typ či nepovolené zahození návratové hodnoty z funkce.
#define ERR_SEM_REDEFINITION          5         // sémantická chyba v programu – redefinice proměnné nebo funkce; přiřazení do nemodifikovatelné proměnné.
#define ERR_SEM_RETURN_EXPRESSION     6         // sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce.
#define ERR_SEM_TYPE_COMPATIBILITY    7         // sémantická chyba v programu – typová nekompatibilita v aritmetických, řetězcových a relačních výrazech; nekompatibilní typ výrazu (např. při přiřazení).
#define ERR_SEM_TYPE_DERIVATION       8         // sémantická chyba v programu – typ proměnné není uveden a nelze odvodit od použitého výrazu.
#define ERR_SEM_UNUSED_VAR            9         // sémantická chyba v programu – nevyužitá proměnná v jejím rozsahu platnosti; modifikovatelná proměnná bez možnosti změny po její inicializaci.
#define ERR_SEM_OTHER                10         // ostatní sémantické chyby.
#define ERR_INTERNAL_COMPILER        99         // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.).



/**
 * @brief Funkce, ktera ukonci program s chybovym kodem a vypise na stderr zpravu, kde se chyba stala
 * 
 * @param token ukazatel na token pro zobrazeni radku
 * @param code  chybovy kod
 */
void exitWithError(Token *token, int code);

#endif // _ERROR_H_