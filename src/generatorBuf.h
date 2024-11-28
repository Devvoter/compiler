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
    PRINT_TYPE t;
    struct codeBufElem *next;
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
 */
bool bufInit(codeBuf** buffer);


/**
 * @brief prida string do buffru s kodem
 */
bool addCodeToBuf(codeBuf **buffer, void *str, PRINT_TYPE t);



/**
 * @brief vytiskne vytvoreny kod na stdout
 */
void bufPrint(codeBuf** buffer);

/**
 * @brief uvolni veskerou pamet alokovanou pro codeBuffer
 */
void bufDestroy(codeBuf* buffer);
