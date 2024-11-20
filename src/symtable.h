/**
 * Project : Prekladac imperativniho jazyka IFJ24
 * @file symtable.h
 * @brief Struktura a deklarace funkci pro tabulku symbolu (TS)
 * 
 * @author Michaela Capíková (xcapikm00)
 * @date 2024-10-19
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/** 
 * @brief Prvek TS - proměnná
 */
typedef struct var{
    TokenType dataType;
    bool isDef; 
    bool isConst;
    bool isNull;
    bool isUsed;       // nevyuziti promenne je chyba
} tVar;

/**
 * @brief Prvek TS - funkce
 */
typedef struct fun{
    int retType;
    bool hasReturned;
    int *paramTypes; // typy parametrú funkce - pole?
    int paramCnt;    // pocet parametru
} tFun;

/**
 * @brief Struktura pro reprezentaci TS - AVL strom
 */
typedef struct symTabNode{
    char *id;
    bool isFun;             // uzel muze reprezentovat funkci/promennou
    tVar *varData;
    tFun *funData;

    tSymTabNode *left;
    tSymTabNode *right; 
    int height;             // vyskova vyvazenost uzlu   
} tSymTabNode;

/**
 * @brief Struktura zásobníku pro reprezentaci úrovní TS
 */
typedef struct frame{
    bool isFun;               // zda blok kodu patri funkci nebo while,if,..
    tSymTabNode *funDecl;     // pro kontrolu návratové hodnoty funkce
    int level;
    tSymTabNode *symTable;    // lokalni TS
    tFrame *prev;             // nadrazeny blok kodu
} tFrame;

/**
 * @brief Zasobník pro reprezentaci úrovní TS
 */
typedef struct frameStack{
    tFrame *current;
    tFrame *global;
} tFrameStack;

/**
 * @brief Funkce pro vytvoření a inicializaci zásobníku úrovní TS
 * @return Ukazatel na zásobník úrovní TS
 */
tFrameStack* init_frame_stack (tFrameStack *fs);

/**
 * @brief Funkce pro vložení nové úrovně TS do zásobníku úrovní
 * @param fs Ukazatel na zásobník úrovní TS
 * @param isFun True pokud nová úroveň patří funkci
 * @return Ukazatel na přidanou úroveň TS. NULL ak došlo k chybě
 */
tFrame* push_frame (tFrameStack *fs, bool isFun);

/**
 * @brief Funkce pro odstránění nejvíce zanořené úrovně TS ze zásobníku úrovní
 * @param fs Ukazatel na zásobník úrovní TS
 * @return Ukazatel na odstraněnou úroveň TS. NULL ak byl zásobník prázdný
 *
 * Úroveň je zapotřebí odstránit VŽDY při opuštění bloku kódu!!!
 * Zásobník pak bude obsahovat jenom symboly platné v aktuální úrovni
 * Funkce neuvolňuje paměť!!! Odstráněnou úroveň je možné dál využít
 */
tFrame* pop_frame (tFrameStack *fs);

/**
 * @brief Funkce pro hledání symbolu ve vybrané úrovni TS
 * @param node Ukazatel na úroveň TS
 * @param key Klíč = identifikátor hledaného symbolu
 * @return Ukazatel na nalezený symbol. NULL ak nebyl nalezen
 */
tSymTabNode* search_avl (tSymTabNode *node, char *key);

/**
 * @brief Funkce pro hledání symbolu v TS
 * @param fs Ukazatel na zásobník úrovní TS
 * @param key Klíč = identifikátor hledaného symbolu
 * @return Ukazatel na nalezený symbol. NULL ak nebyl nalezen
 */
tSymTabNode* search_symbol (tFrameStack *fs, char *key);

/**
 * @brief Funkce pro LL rekonfiguraci uzlú AVL stromu po operaci insert
 * @param root Kritický uzel 
 * @return Aktuální kořen podstromu po vykonaní rotace
 */
tSymTabNode* rotate_ll (tSymTabNode *root);

/**
 * @brief Funkce pro RR rekonfiguraci uzlú AVL stromu po operaci insert
 * @param root Kritický uzel 
 * @return Aktuální kořen podstromu po vykonaní rotace
 */
tSymTabNode* rotate_rr (tSymTabNode *root);

/**
 * @brief Funkce pro DLR rekonfiguraci uzlú AVL stromu po operaci insert
 * @param root Kritický uzel 
 * @return Aktuální kořen podstromu po vykonaní rotace
 */
tSymTabNode* rotate_dlr (tSymTabNode *root);

/**
 * @brief Funkce pro DRL rekonfiguraci uzlú AVL stromu po operaci insert
 * @param root Kritický uzel 
 * @return Aktuální kořen podstromu po vykonaní rotace
 */
tSymTabNode* rotate_drl (tSymTabNode *root);

/**
 * @brief Funkce pro vložení uzlu do AVL stromu. Zachovává výškovou vyváženost
 * @param root Kořen stromu - tabulka symbolú
 * @param node Vkladaný uzel
 * @param heightChange Oznamuje uzlu, zda došlo k změne výškové vyváženosti u jeho syna
 * @return Aktuální kořen stromu
 */
tSymTabNode* insert_avl (tSymTabNode *root, tSymTabNode *node, bool *heightChange);

/**
 * @brief Funkce pro vytvoření uzlu reprezentující proměnnou
 * @param isConst Zda je proměnná konstantou (První informace získaná při deklaraci)
 * @return Ukazatel na vytvořený uzel. Null když selhala alokace paměti
 */
tSymTabNode* create_var_node (bool isConst);

/**
 * @brief Funkce pro vytvoření uzlu reprezentující funkci
 * @return Ukazatel na vytvořený uzel. Null když selhala alokace paměti
 */
tSymTabNode* create_fun_node ();

/**
 * @brief Funkce pro vložení uzlu do tabulky symbolú. Před vložením ověří duplicitu identifikátorú
 * @param fs Ukazatel na zásobník úrovní TS
 * @param node Ukazatel na vkládaný uzel
 * @return True při úspěšném vložení. False když byl nalezen duplicitní identifikátor
 */
bool insert_symbol (tFrameStack *fs, tSymTabNode *node);

/**
 * @brief Funkce pro zrušení tabulky symbolú
 * @param root Kořen tabulky (avl stromu)
 */
void dispose_avl (tSymTabNode *root);

/**
 * @brief Funkce pro zrušení úrovně tabulky symbolú
 * @param frame Úroveň tabulky
 */
void dispose_frame(tFrame *frame);

/**
 * @brief Funkce pro zrušení zásobníku úrovní tabulky symbolú
 * @param fs Zásobník úrovní tabulky symbolú
 */
void dispose_frame_stack(tFrameStack *fs);

/* Konec souboru symtable.h */