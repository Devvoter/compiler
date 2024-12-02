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
#include "list.h"

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
 * 
 * @param ID nazev promenne
 */
bool defVarGen(char *ID);

/**
 * @brief funkce pro vypis, kompatibilni funkce ifj.write
 */
bool writeStandFuncGen();

/**
 * @brief funkce pro nacitani stringu
 */
bool readstrStandFuncGen();

/**
 * @brief funkce pro nacitani integeru
 */
bool readi32StandFuncGen();

/**
 * @brief funkce pro nacitani floatu
 */
bool readf64StandFuncGen();

// /**
//  * @brief funkce pro prirazeni stringu
//  */
// bool stringStandFuncGen();

/**
 * @brief funkce pro nalezeni delky retezce
 */
bool lengthStandFuncGen();

/**
 * @brief funkce pro konkatenaci retezcu 
 */
bool concatStandFuncGen();

/**
 * @brief funkce vrati ASCII kod zadaneho znaku
 */
bool ordStandFuncGen();

/**
 * @brief funkce vrati znak zadaneho ASCII kodu
 */
bool chrStandFuncGen();

/**
 * @brief funkce prevede celociselnou hodnotu na hodnotu typu float
 */
bool i2fStandFuncGen();

/**
 * @brief funkce prevede float hodnotu na hodnotu typu int
 */
bool f2iStandFuncGen();

/**
 * @brief volani funkce ifj.substring, parametry funkce jsou ulozeny na zasobniku
 */
bool substringStandFuncGen();

/**
 * @brief funkce vygeneruje pomocnou funkci substring, ktera se bude volat pri zpracovani substringu
 */
bool substringGen();

/**
 * @brief volani funkce ifj.strcmp, parametry funkce jsou ulozene na zasobniku
 */
bool strcmpFuncGen();

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
 * @param idiv v pripade true deleni je celociselne
 */
bool makeOperationStackGen(TokenType t, bool idiv);

/**
 * @brief konec zpracovani vyrazu, pripsani hodnoty z datoveho zasobniku do promenne
 * 
 * @param ID nazev promenne
 */
bool endExpAssignGen(char *ID);

/**
 * @brief vygeneruje zacatek if blocku, zpracovani podminky probiha pred touto funkci, funkce pouze posoudi, jestli podminka plati
 * 
 * @param withNull v pripade true, v blocku if probehne definice a prepsani 
 */
bool startIfGen(bool withNull, char *ID);

/**
 * @brief vygeneruje konec if a pripadne zacatek else, v pripade bez else, tato funkce se nevola
 */
bool startElseGen();

/**
 * @brief vygeneruje konec if-else statmentu
 * 
 * @param withElse v pripade false, generuje se konec pouze pro if
 */
bool endIfElseGen(bool withElse);

/**
 * @brief vygeneruje zacatek while cyklu
 */
bool startWhileGen();

/**
 * @brief vygeneruje vyhodnoceni podminky a pripadny skok na konec, jestli podminka neplati
 * 
 * @param isNulable v pripade true while loop je typu zahrnujiciho null
 */
bool endCondWhileGen(bool isNullable, char *ID);

/**
 * @brief funkce vygeneruje konec while loopu
 */
bool endWhileGen();

/**
 * @brief volani pomocne funkce
 * 
 * @param name nazev funkce
 * @param paramsCount pocet parametru
 */
bool callFuncGen(char *name, int paramsCount);

/**
 * @brief vygeneruje zacatek pomocne funkce 
 * 
 * @param name nazev pomocne funkce
 * @param l struktura s parametry funkce
 */
bool funcStartGen(char *name, ParamList *l); 

/**
 * @brief vygeneruje konec pomocne funkce 
 */
bool funcEndGen();

/**
 * @brief vygeneruje return; funkci 
 */
bool returnMainGen();

/**
 * @brief zmeni string na vhodny retezec pro IFJcode24
 * 
 * @param input vstupni string
 */
char *replace_special_characters(const char *input);

/**
 * @brief v pripade, ze jsme v cyklu a promenna se rovna aktualnimu nazvu isNullable promenne, zameni ji na nazev pomocne promenne $$while$isNullable$1
 * 
 * @param ID vstupni nazev promenne
 */
char *storeChar(char *ID);


