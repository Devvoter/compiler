/**
 * Project : Prekladac imperativniho jazyka IFJ24
 * @file symtable.h
 * @brief Struktura a deklarace funkci pro tabulku symbolu (TS)
 * 
 * @author Michaela Capíková (xcapikm00)
 * @date 2024-10-19
 *
 * @todo Nahradit placeholdery v strukturach!!!, operace AVL, funkce semanticke analyzy
 *       V struct tVar pouzit existujici TokenValue misto VarValue zo scanner.c??
 *       Odstranenou uroven symtab odstranujeme nebo uvolnujeme?
 *       Pri vkladani symbolu kontrolujeme duplicitu?
 */

#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"

enum dataType {
    i32,
    f64,
    u8
};

// TokenValue?
typedef union {
    long       i32;
    double     f64;
    tBuffer*    u8;
} VarValue;

/** 
 * @brief Prvek TS - proměnná
 */
typedef struct {
    int dataType;
    VarValue value;    // TokenValue?
    bool isConst;
    bool isNullable;
    bool isUsed;       // nevyuziti promenne je chyba
} tVar;

/**
 * @brief Prvek TS - funkce
 */
typedef struct {
    int retType;
    // *paramTypes   // funkce muze mit vice ruznych parametru - jak implementovat?
    int paramCnt;    // pocet parametru
} tFun;

/**
 * @brief Struktura pro reprezentaci TS - AVL strom
 */
typedef struct {
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
typedef struct {
    bool isFun;               // zda blok kodu patri funkci nebo while,if,..
    tSymTabNode *symTable;    // lokalni TS
    tFrame *prev;             // nadrazeny blok kodu
} tFrame;

/**
 * @brief Zasobník pro reprezentaci úrovní TS
 */
typedef struct {
    tFrame *current;
    tFrame *global;
} tFrameStack;

/**
 * @brief Funkce pro inicializaci zásobníku úrovní TS
 * @param fs Ukazatel na zásobník úrovní TS
 */
void init_frame_stack (tFrameStack *fs);

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
 * @todo Uvolnit úroveň nebo se bude ukládat jinam?
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

//---------------------IN PROGRESS----------------------------------------
/**
 * OPERACE PRO AVL STROM
 * create / init node: var, fun
 * insert node - kontrola duplicity
 * delete node (bude nekdy potreba mazat?)
 *  LLrot
 *  LRrot
 *  RRrot
 *  RLrot
*/

/* Konec souboru symtable.h */