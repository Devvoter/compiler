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

/**
 * @brief buffer struktura pro []u8
 * 
 */
typedef struct{
    char*      data;    // < ukazatel na řetězec
    size_t   length;    // < delka 
    size_t capacity;    // < kapacita bafru
} tBuffer;

// ...

#endif // _BUFFER_H_ 