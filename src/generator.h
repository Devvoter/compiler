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
 * @param LF v pripade true, promenna se zapise do lokalniho framu
 */
bool defVarGen(char *ID, bool LF);

/**
 * @brief definuje promennou
 * 
 * @param ID nazev promenne
 * @param t typ hodnoty
 * @param value hodnota
 * @param fromLF v pripade true pripisuje hodnotu z LF
 * @param toLF v pripade true pripisuje do LF
 */
bool assignVarGen(char *ID, TokenType t, char *value, bool fromLF, bool toLF);

/**
 * @brief funkce pro vypis, kompatibilni funkce ifj.write
 * 
 * @param t typ hodnoty, ktera bude vypsana(var, int, string, float)
 * @param param hodnota, ktera se ma vypsat
 */
bool writeStandFuncGen(TokenType t, char* param);

/**
 * @brief funkce pro nacitani znaku
 * 
 * @param t typ funkce(i32, f64, string)
 * @param ID nazev promenne, kam se zapise nactena data. V pripade zpracovani vyrazu bude se rovnat NULL
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool readStandFuncGen(readFunc_t t, char *ID, bool pushOnStack);

/**
 * @brief funkce pro prirazeni stringu
 * 
 * @param t typ prirazovane hodnoty(var nebo string)
 * @param ID nazev promenne, kam se ma priradit string
 * @param param hodnota, ktera se ma pripsat
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool stringStandFuncGen(char *ID, char *param, bool pushOnStack);

/**
 * @brief funkce pro nalezeni delky retezce
 * 
 * @param ID promenna, kam se ulozi vysledek(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param argument funkce
 * @param isVar true, pokud je argument promenna, jinak retezec
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool lengthStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack);

/**
 * @brief funkce pro konkatenaci retezcu 
 * 
 * @param ID nazev promenne, kam se ulozi vysledek(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param1 prvni argument funkce
 * @param isVar1 v pripade true prvni argument je promenna
 * @param param2 druhy argument funkce
 * @param isVar2 v pripade true druhy argument je promenna
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool concatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack);

/**
 * @brief funkce vrati ASCII kod zadaneho znaku
 * 
 * @param ID promenna, kam se zapise vysledek(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param1 vstupni string
 * @param isVar1 true v pripade, ze param1 je promenna
 * @param param2 index znaku, hodnota ktereho se ma vratit
 * @param isVar2 true v pripade, ze param2 je promenna
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool ordStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack);

/**
 * @brief funkce vrati znak zadaneho ASCII kodu
 * 
 * @param ID promenna, kam se zapise hodnota(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param ASCII hodnota
 * @param isVar pokud true, parametr je promenna
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool chrStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack);

/**
 * @brief funkce prevede celociselnou hodnotu na hodnotu typu float
 * 
 * @param ID promenna, kam bude ulozen vysledek(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param parametr funkce
 * @param isVar v pripade true zadany argument je promenna
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool i2fStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack);

/**
 * @brief funkce prevede float hodnotu na hodnotu typu int
 * 
 * @param ID promenna, kam bude ulozen vysledek(v pripade ukladani vysledku na zasobnik rovna se NULL)
 * @param param parametr funkce
 * @param isVar v pripade true zadany argument je promenna
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool f2iStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack);

/**
 * @brief funkce vrati podretezec zadaneho stringu
 * 
 * @param ID nazev promenne, kam se ulozi vysledek
 * @param param1 prvni argument(vstupni retezec)
 * @param isVar1 v pripade true prvni argument je predan ve forme promenne
 * @param param2 druhy argument(index zacatku podretezce)
 * @param isVar2 v pripade true druhy argument je predan ve forme promenne
 * @param param3 treti argument(index konce podretezce)
 * @param isVar3 v pripade true treti argument je predan ve forme promenne
 * @param pushOnStack v pripade true hodnota bude pridana na zasobnik(zpracovani vyrazu)
 */
bool substringStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, char *param3, bool isVar3, bool pushOnStack);

/**
 * @brief volani pomocne funkce
 * 
 * @param name nazev funkce
 */
bool callFuncGen(char *name); //TODO doplnit parametry(parametry funkce, jejich pocet)

/**
 * @brief pripsani navratove hodnoty z pomocne funkce do promenne, pripadne na zasobnik
 * 
 * @param ID nazev promenne, kam se ma ulozit hodnota
 * @param pushOnStack v pripade true, prida se hodnota na vrchol zasobniku
 */
bool retValGen(char *ID, bool pushOnStack);

/**
 * @brief vygeneruje zacatek pomocne funkce 
 * 
 * @param 
 */
bool funcStartGen(char *name); //TODO parametrs

/**
 * @brief vygeneruje konec pomocne funkce 
 */
bool funcEndGen();

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
 * @brief zmeni string na vhodny retezec pro IFJcode24
 * 
 * @param input vstupni string
 */
char *replace_special_characters(const char *input);

/**
 * @brief v pripade, ze jsme v cyclu a promenna se rovna aktualnimu nazvu isNullable promenne, zameni ji na nazev pomocne promenne $$while$isNullable$1 promenne
 * 
 * @param ID vstupni nazev promenne
 */
char *isNullableVar(char *ID);


