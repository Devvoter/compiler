/**
 * @file buffer.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-09-30
 * 
 */

#include "buffer.h"


bool bufferAddChar(tBuffer* buffer ,char c) {

    if (buffer == NULL)  return false;


    if (buffer->size + 1 < buffer->length){
        buffer->data[buffer->size] = c;
        buffer->size++;
        return true;
    }
    else {
        
        char* tmp;
        tmp = realloc(buffer->data, buffer->length * 2);
        if (tmp == NULL) return false;
            

        buffer->data = tmp;
        buffer->length *= 2;
        buffer->data[buffer->size] = c;
        buffer->size++;

        for (size_t i = buffer->size; i < buffer->length; i++) {
            buffer->data[i] = 0;
        }
        return true;
    }
}
