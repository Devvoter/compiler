/**
 * @file generator.c
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief implementace funkci pro generovani
 *
 * @date 1.11.2024
 */

#include "generator.h"
#include "generatorBuf.c"

#include <stdio.h>
#include <string.h>

// inicializace buffru pro ulozeni kodu
codeBuf *buffer;

bool startGen()
{
    if (bufInit(&buffer) && addCodeToBuf(&buffer, "CREATEFRAME\n") &&
        addCodeToBuf(&buffer, "PUSHFRAME\n"))
    {
        return true;
    } 
    return false;
}

bool endGen()
{
    if (addCodeToBuf(&buffer, "POPFRAME\n")) {
        return true;
    }
    return false;
}

void disposeGen(bool done) {
    if (done == true) {
        bufPrint(&buffer);
    }
    bufDestroy(buffer);
}

bool startMainGen() {
    if (addCodeToBuf(&buffer, "CREATEFRAME\n") && addCodeToBuf(&buffer, "PUSHFRAME\n")) {
        return true;
    }
    return false;
}

bool endMainGen() {
    if (addCodeToBuf(&buffer, "POPFRAME\n")) {
        return true;
    }
    return false;
}

bool defVarGen(char* ID, bool LF) {
    if (addCodeToBuf(&buffer, "DEFVAR ")) {
        if (LF == true) {
            if (addCodeToBuf(&buffer, "LF@")) {
                if (addCodeToBuf(&buffer, ID)) {
                    if (addCodeToBuf(&buffer, "\n")) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void main()
{
    startGen();
    startMainGen();
    defVarGen("prom", true);
    endMainGen();
    endGen();
    disposeGen(true);
    return;
}