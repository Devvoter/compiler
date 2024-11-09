/**
 * @file generator.c
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief implementace generatoru
 *
 * @date 1.11.2024
 */

#include "generator.h"
#include "error.h"

#include <stdio.h>
#include <string.h>

void BufferInit(codeBuffer *buffer)
{
    buffer = malloc(sizeof(codeBuffer));
    if (buffer == NULL)
    {
        exitWithError(NULL, ERR_INTERNAL_COMPILER);
    }
    buffer->first = NULL;
    buffer->active = NULL;
}

void addCodeBufferElement(char *instr, char *firstArg, char *secondArg, char *thirdArg)
{
    //instrukce pro praci s ramci, volani funkce
    if (strcmp(instr, "MOVE") == 0)
    {
        // proved neco
    }
    else if (strcmp(instr, "CREATEFRAME") == 0)
    {
        // proved neco
    }
    else if (strcmp(instr, "PUSHFRAME") == 0)
    {
        //proved neco
    }
    else if (strcmp(instr, "POPFRAME") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "DEFVAR") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "CALL") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "RETURN") == 0)
    {
        //proved neco 
    }
    //instrukce pro prace s datovym zasobnikem
    else if (strcmp(instr, "PUSHS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "POPS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "CLEARS") == 0)
    {
        //proved neco 
    }
    //aritmeticke, relacni, booleovske a konverzni funkce
    else if (strcmp(instr, "ADD") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "SUB") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "MUL") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "DIV") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "IDIV") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "ADDS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "SUBS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "MULS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "DIVS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "IDIVS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "LT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "GT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "EQ") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "AND") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "OR") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "NOT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "ANDS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "ORS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "NOTS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "INT2FLOAT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "FLOAT2INT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "INT2CHAR") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "STRI2INT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "INT2FLOATS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "FLOAT2INTS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "INT2CHARS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "STRI2INTS") == 0)
    {
        //proved neco 
    }
    //vstupne-vystupni instrukce
    else if (strcmp(instr, "READ") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "WRITE") == 0)
    {
        //proved neco 
    }
    // prace s retezci
    else if (strcmp(instr, "CONCAT") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "STRLEN") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "GETCHAR") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "SETCHAR") == 0)
    {
        //proved neco 
    }
    //prace s typy
    else if (strcmp(instr, "TYPE") == 0)
    {
        //proved neco 
    }
    //rizeni toku programu
    else if (strcmp(instr, "LABEL") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "JUMP") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "JUMPIFEQ") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "JUMPIFNEQ") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "JUMPIFEQS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "JUMPIFNEQS") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "EXIT") == 0)
    {
        //proved neco 
    }
    //ladici instrukce
    else if (strcmp(instr, "BREAK") == 0)
    {
        //proved neco 
    }
    else if (strcmp(instr, "DPRINT") == 0)
    {
        //proved neco 
    }
}

void generateCode()
{
    return 0;
}