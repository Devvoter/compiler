/**
 * @file generator.h
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief definice struktur pro generator
 * 
 * @date 1.11.2024
 */

/**
 * @brief struktura elementu pro ukladani mezikodu
 */
typedef struct codeBufferElement {
    char *code;
    struct codeBufferElement *next;
} *codeBufferElementPtr;

/**
 * @brief struktura jednosmerneho seznamu pro ukladani mezikodu
 */
typedef struct {
    codeBufferElementPtr* first;
    codeBufferElementPtr* active;
    int CurrentLength;
} codeBuffer;

/**
 * @brief inicializace buffer pro ukladani mezikodu
 */
void BufferInit(codeBuffer* buffer);

/**
 * @brief pridani polozky do bufferu
 */
void addCodeBufferElement(char instr[10], char* firstArg, char* secondArg, char* thirdArg);

/**
 * @brief uvolni veskerou pamet alokovanou pro codeBuffer
 */
void destroyBuffer(codeBuffer* buffer);



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
