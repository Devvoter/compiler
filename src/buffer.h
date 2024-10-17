/**
 * @file buffer.h
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-09-30
 * 
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFFER_SIZE 8

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief buffer struktura pro []u8
 * 
 */
typedef struct{
    char*      data;    // < ukazatel na řetězec
    size_t     size;    // < aktualní size řetezce
    size_t   length;    // < max size řetezce
} tBuffer;


/**
 * @brief  V buffer se zapísuje symbol c.
 * 
 * @param[in] buffer Ukazatel na buffer
 * @param[in] c Symbol do buffru
 * 
 * @return Vrátí true pokud se povede uložít jinak false
 * 
 */
bool bufferAddChar(tBuffer* buffer , char c);

/**
 * @brief Inicializace buffru
 * 
 * @return Ukazatel na inicializovaný buffer. V připadě chyby alokace 
 *         vrací hodnotu NULL
 * 
 */
tBuffer* bufferInit();

/**
 * @brief Uvolnění paměti
 * 
 * @param[in] buffer Ukazatel na buffer
 */
void bufferFree(tBuffer *buffer);


#endif // _BUFFER_H_ 