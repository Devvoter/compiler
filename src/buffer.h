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

// ...

bool bufferAddChar(tBuffer* buffer ,char c);

tBuffer* dynamicBuffer_INIT();
#endif // _BUFFER_H_ 