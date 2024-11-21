/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice funkci pro generovani kodu
 * 
 * @date 1.11.2024
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "scanner.h"

typedef enum {
    T_READSTR,
    T_READI32,
    T_READF64
} readFunc_t;


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
 * @brief funkce pro vypis, kompatibilni funkce ifj.write
 * @param t typ hodnoty, ktera bude vypsana(var, int, string, float)
 * @param param hodnota, ktera se ma vypsat
 */
bool WriteStandFuncGen(TokenType t, char* param);

/**
 * @brief funkce pro nacitani znaku
 * @param t typ funkce(i32, f64, string)
 * @param ID nazev promenne, kam se zapise nactena data
 */
bool ReadStandFuncGen(readFunc_t t, char *ID);

/**
 * @brief funkce pro prirazeni stringu
 * @param t typ prirazovane hodnoty(var nebo string)
 * @param ID nazev promenne, kam se ma priradit string
 * @param param hodnota, ktera se ma pripsat
 */
bool StringStandFuncGen(char *ID, char *param);

/**
 * @brief funkce pro nalezeni delky retezce
 * @param ID promenna, kam se ulozi vysledek
 * @param param argument funkce
 * @param isVar true, pokud je argument promenna, jinak retezec
 */
bool LengthStandFuncGen(char *ID, char *param, bool isVar);

/**
 * @brief funkce pro konkatenaci retezcu 
 * @param ID nazev promenne, kam se ulozi vysledek
 * @param param1 prvni argument funkce
 * @param isVar1 v pripade true prvni argument je promenna
 * @param param2 druhy argument funkce
 * @param isVar2 v pripade true druhy argument je promenna
 */
bool ConcatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2)


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

/**
 * @brief zmeni string na vhodny retezec pro IFJcode24
 * @param input vstupni string
 */
char *replace_special_characters(const char *input);



