/**
 * @file generatorBuf.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice struktur a funkci pro buffer generatoru
 * 
 * @date 18.11.2024
 */

#include <stdbool.h>

typedef enum {
    T_FLOAT,
    T_INT,
    T_STRING,
    T_STRING_FROM_PARSER,
    T_OTHERS
} PRINT_TYPE;

/**
 * @brief struktura elementu pro ukladani mezikodu
 */
typedef struct codeBufElem {
    void *code;
    PRINT_TYPE print_t;
    bool while_t;
    struct codeBufElem *next;
    struct codeBufElem *previous;
} *codeBufElemPtr;

/**
 * @brief struktura jednosmerneho seznamu pro ukladani mezikodu
 */
typedef struct {
    codeBufElemPtr first;
    codeBufElemPtr active;
} codeBuf;

/**
 * @brief inicializace buffer pro ukladani mezikodu
 * 
 * @param buffer list obsahujicic mezikod na vypis
 */
bool bufInit(codeBuf** buffer);


/**
 * @brief prida string do buffru s kodem
 * 
 * @param buffer list obsahujici mezikod na vypis
 * @param str ukazatel na pridavany kod
 * @param print_t typ pridavaneho kodu
 * @param while_t pokud true, kod obsahuje zacatek while cyklu
 */
bool addCodeToBuf(codeBuf **buffer, void *str, PRINT_TYPE print_t, bool while_t);

/**
 * @brief vytvori definici promenne pred cyklem 
 * 
 * @param buffer list obsahujici mezikod na vypis
 * @param str ukazatel na pridavany kod
 * @param print_t typ pridavaneho kodu
 */
bool addVarBeforeWhile(codeBuf **buffer, void *str, PRINT_TYPE print_t);

/**
 * @brief vytiskne vytvoreny kod na stdout
 * 
 * @param buffer list obsahujici mezikod na vypis
 */
void bufPrint(codeBuf** buffer);

/**
 * @brief uvolni veskerou pamet alokovanou pro codeBuffer
 * 
 * @param buffer list obsahujici mezikod na vypis
 */
void bufDestroy(codeBuf* buffer);
