/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice struktur pro generator
 * 
 * @date 18.11.2024
 */

/**
 * @brief struktura elementu pro ukladani mezikodu
 */
typedef struct codeBufElem {
    char *code;
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
void bufInit(codeBuf** buffer);

/**
 * @brief pridani polozky do bufferu
 */
void addCodeBufferElement(codeBuf **buffer, char instr[10], char* firstArg, char* secondArg, char* thirdArg);

/**
 * @brief uvolni veskerou pamet alokovanou pro codeBuffer
 */
void bufDestroy(codeBuf* buffer);

/**
 * @brief element struktury zásobníku rámců
 */
typedef struct framesStackElement{
    //nejaka data
    struct framesStackElement *next;
} *framesStackElementPtr;

/**
 *  struktura zasobniku rámců
 */
typedef struct {
    framesStackElementPtr *top;
} framesStack;