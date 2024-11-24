/**
 * @file generator.c
 * @author Polina Ustiuzhantseva(xustiup00)
 * @brief implementace funkci pro generovani
 *
 * @date 1.11.2024
 */

#include "generator.h"
#include "generatorBuf.c"
#include "codeStack.c"

#include <stdio.h>
#include <string.h>


// inicializace buffru pro ulozeni kodu, pocitadla a zasobniku pro if statmenty
codeBuf *buffer;
int ifCounter;
Stack *ifStack;

bool startGen()
{
    ifCounter = 0;
    CodeStack_Init(&ifStack);
    return (bufInit(&buffer) && addCodeToBuf(&buffer, "\nCREATEFRAME", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nPUSHFRAME", T_OTHERS));
}

bool endGen()
{
    return addCodeToBuf(&buffer, "\nPOPFRAME\n", T_OTHERS);
}

void disposeGen(bool done)
{
    if (done == true)
    {
        bufPrint(&buffer);
    }
    CodeStack_destroy(ifStack);
    bufDestroy(buffer);
}

bool startMainGen()
{
    return (addCodeToBuf(&buffer, "\nCREATEFRAME", T_OTHERS) && 
            addCodeToBuf(&buffer, "\nPUSHFRAME", T_OTHERS));
}

bool endMainGen()
{
    return addCodeToBuf(&buffer, "\nPOPFRAME", T_OTHERS);
}

bool defVarGen(char *ID, bool LF)
{
    if (addCodeToBuf(&buffer, "\nDEFVAR ", T_OTHERS))
    {
        if (LF == true)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS))
            {
                return (addCodeToBuf(&buffer, ID, T_OTHERS));
            }
        }
        else
        {
            if (addCodeToBuf(&buffer, "TF@", T_OTHERS))
            {
                return (addCodeToBuf(&buffer, ID, T_OTHERS));
            }
        }
    }
    return false;
}

bool assignVarGen(char *ID, TokenType t, char *value, bool fromLF, bool toLF)
{
    if (addCodeToBuf(&buffer, "\nMOVE ", T_OTHERS))
    {
        if (toLF == true)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
            {
                if (t == T_VAR)
                {
                    if (fromLF == true)
                    {
                        return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                                addCodeToBuf(&buffer, value, T_OTHERS));
                    }
                    else
                    {
                        return (addCodeToBuf(&buffer, " TF@", T_OTHERS) && 
                                addCodeToBuf(&buffer, value, T_OTHERS));
                    }
                }
                else if (t == T_STRING_TYPE)
                {
                    value = replace_special_characters(value);
                    if (value == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", T_OTHERS) && 
                            addCodeToBuf(&buffer, value, T_STRING));
                }
                else if (t == T_I32_ID)
                {
                    return (addCodeToBuf(&buffer, " int@", T_OTHERS) && 
                            addCodeToBuf(&buffer, value, T_INT));
                }
                else if (t == T_F64_ID)
                {
                    return (addCodeToBuf(&buffer, " float@", T_OTHERS) && 
                            addCodeToBuf(&buffer, value, T_FLOAT));
                }
                else if (t == T_NULL)
                {
                    return addCodeToBuf(&buffer, " nil@nil", T_OTHERS);
                }
            }
        }
        else if (addCodeToBuf(&buffer, "TF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
        {
            if (t == T_VAR)
            {
                if (fromLF == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                            addCodeToBuf(&buffer, value, T_OTHERS));
                }
                else
                {
                    return (addCodeToBuf(&buffer, " TF@", T_OTHERS) && 
                            addCodeToBuf(&buffer, value, T_OTHERS));
                }
            }
            else if (t == T_STRING_TYPE)
            {
                value = replace_special_characters(value);
                if (value == NULL)
                {
                    return false;
                }
                return (addCodeToBuf(&buffer, " string@", T_OTHERS) && 
                        addCodeToBuf(&buffer, value, T_STRING));
            }
            else if (t == T_I32_ID)
            {
                return (addCodeToBuf(&buffer, " int@", T_OTHERS) && 
                        addCodeToBuf(&buffer, value, T_INT));
            }
            else if (t == T_F64_ID)
            {
                return (addCodeToBuf(&buffer, " float@", T_OTHERS) && 
                        addCodeToBuf(&buffer, value, T_FLOAT));
            }
            else if (t == T_NULL)
            {
                return (addCodeToBuf(&buffer, " nil@nil", T_OTHERS));
            }
        }
    }
    return false;
}

bool writeStandFuncGen(TokenType t, char *param)
{
    if (addCodeToBuf(&buffer, "\nWRITE ", T_OTHERS))
    {
        if (t == T_VAR)
        {
            return (addCodeToBuf(&buffer, "LF@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_OTHERS));
        }
        else if (t == T_I32_ID)
        {
            return (addCodeToBuf(&buffer, "int@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_INT));
        }
        else if (t == T_F64_ID)
        {
            return (addCodeToBuf(&buffer, "float@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_FLOAT));
        }
        else if (t == T_STRING_TYPE)
        {
            param = replace_special_characters(param);
            if (param == NULL)
            {
                return false;
            }
            return (addCodeToBuf(&buffer, "string@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_STRING));
        }
    }
    return false;
}

bool readStandFuncGen(readFunc_t t, char *ID)
{
    if (addCodeToBuf(&buffer, "\nREAD ", T_OTHERS))
    {
        if (t == T_READSTR)
        {
            return (addCodeToBuf(&buffer, "@LF", T_OTHERS) && 
                    addCodeToBuf(&buffer, ID, T_OTHERS) && 
                    addCodeToBuf(&buffer, " string", T_OTHERS));
        }
        else if (t == T_READI32)
        {
            return (addCodeToBuf(&buffer, "@LF", T_OTHERS) && 
                    addCodeToBuf(&buffer, ID, T_OTHERS) && 
                    addCodeToBuf(&buffer, " int", T_OTHERS));
        }
        else if (t == T_READF64)
        {
            return (addCodeToBuf(&buffer, "@LF", T_OTHERS) && 
                    addCodeToBuf(&buffer, ID, T_OTHERS) && 
                    addCodeToBuf(&buffer, " float", T_OTHERS));
        }
    }
    return false;
}

bool stringStandFuncGen(char *ID, char *param)
{
    param = replace_special_characters(param);
    if (param == NULL)
    {
        return false;
    }
    return (addCodeToBuf(&buffer, "\nMOVE ", T_OTHERS) && 
            addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
            addCodeToBuf(&buffer, ID, T_OTHERS) && 
            addCodeToBuf(&buffer, " string@", T_OTHERS) && 
            addCodeToBuf(&buffer, param, T_STRING));
}

bool lengthStandFuncGen(char *ID, char *param, bool isVar)
{
    // dobavit rozsireni mozna, aby mohlo byt soucasti vyrazu
    if (addCodeToBuf(&buffer, "\nSTRLEN ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
    {
        if (isVar == false)
        {
            param = replace_special_characters(param);
            if (param = NULL)
            {
                return false;
            }
            return (addCodeToBuf(&buffer, " string@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_STRING));
        }
        else if (isVar == true)
        {
            return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_OTHERS));
        }
    }
    return false;
}

bool concatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2)
{
    if (addCodeToBuf(&buffer, "\nCONCAT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
    {
        if (isVar1 == true)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_OTHERS))
            {
                if (isVar2 == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                            addCodeToBuf(&buffer, param2, T_OTHERS));
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", T_OTHERS) && 
                            addCodeToBuf(&buffer, param2, T_STRING));
                }
            }
        }
        else
        {
            param1 = replace_special_characters(param1);
            if (param1 == NULL)
            {
                return false;
            }
            if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_STRING))
            {
                if (isVar2 == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                            addCodeToBuf(&buffer, param2, T_OTHERS));
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", T_OTHERS) && 
                            addCodeToBuf(&buffer, param2, T_STRING));
                }
            }
        }
    }
    return false;
}

bool ordStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2) {
    if (addCodeToBuf(&buffer, "\nSTRI2INT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
            if (isVar1 == true) {
                if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_OTHERS)) {
                    if (isVar2 == true) {
                        return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                                addCodeToBuf(&buffer, param2, T_OTHERS));
                    } else {
                        return (addCodeToBuf(&buffer, " int@", T_OTHERS) && 
                                addCodeToBuf(&buffer, param2, T_OTHERS));
                    }
                } 
            } else {
                param1 = replace_special_characters(param1);
                if (param2 == NULL) {
                    return false;
                }
                if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_STRING)) {
                   if (isVar2 == true) {
                        return (addCodeToBuf(&buffer, " LF@", T_OTHERS) && 
                                addCodeToBuf(&buffer, param2, T_OTHERS));
                    } else {
                        return (addCodeToBuf(&buffer, " int@", T_OTHERS) && 
                                addCodeToBuf(&buffer, param2, T_OTHERS));
                    } 
                }
            }
        }
    return false;
}

bool chrStandFuncGen(char *ID, char *param, bool isVar) {
    if (addCodeToBuf(&buffer, "\nINT2CHAR ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
                if (isVar == true) {
                    return (addCodeToBuf(&buffer, " LF@", T_OTHERS) &&
                            addCodeToBuf(&buffer, param, T_OTHERS));
                } else {
                    return (addCodeToBuf(&buffer, " int@", T_OTHERS) &&
                            addCodeToBuf(&buffer, param, T_INT));
                }
            }
    return false;
}

bool pushOnStackGen(char *param, TokenType t)
{
    if (addCodeToBuf(&buffer, "\nPUSHS ", T_OTHERS))
    {
        if (t == T_VAR)
        {
            return (addCodeToBuf(&buffer, "LF@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_OTHERS));
        }
        else if (t == T_I32_ID)
        {
            return (addCodeToBuf(&buffer, "int@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_INT));
        }
        else if (t == T_F64_ID)
        {
            return (addCodeToBuf(&buffer, "float@", T_OTHERS) && 
                    addCodeToBuf(&buffer, param, T_FLOAT));
        }
    }
    return false;
}

bool makeOperationStackGen(TokenType t, bool idiv)
{
    if (t == T_ADD)
    {
        return addCodeToBuf(&buffer, "\nADDS", T_OTHERS);
    }
    else if (t == T_SUB)
    {
        return addCodeToBuf(&buffer, "\nSUBS", T_OTHERS);
    }
    else if (t == T_MUL)
    {
        return addCodeToBuf(&buffer, "\nMULS", T_OTHERS);
    }
    else if (t == T_DIV)
    {
        if (idiv == false)
        {
            return addCodeToBuf(&buffer, "\nDIVS", T_OTHERS);
        }
        else
        {
            return addCodeToBuf(&buffer, "\nIDIVS", T_OTHERS);
        }
    }
    else if (t == T_LESS_THAN)
    {
        return addCodeToBuf(&buffer, "\nLTS", T_OTHERS);
    }
    else if (t == T_GREATER_THAN)
    {
        return addCodeToBuf(&buffer, "\nGTS", T_OTHERS);
    }
    return false;
}

bool endExpAssignGen(char *ID)
{
    return (addCodeToBuf(&buffer, "\nPOPS ", T_OTHERS) && 
            addCodeToBuf(&buffer, "LF@", T_OTHERS) && 
            addCodeToBuf(&buffer, ID, T_OTHERS));
}

bool funcEndGen()
{
    return (addCodeToBuf(&buffer, "\nPOPFRAME\nRETURN", T_OTHERS));
}

bool startIfGen(bool withNull, char *ID) {
    ifCounter++;
    if (withNull == true) {
        return (addCodeToBuf(&buffer, "\nPUSHS nil@nil", T_OTHERS) &&           //push na zasobnik null hodnoty 
                addCodeToBuf(&buffer, "\nJUMPIFEQS $$if$", T_OTHERS) &&         //porovname null s vysledkem vyrazu a udelame skok
                addCodeToBuf(&buffer, (void *) &ifCounter, T_INT) &&                   
                addCodeToBuf(&buffer, "$else", T_OTHERS) &&
                addCodeToBuf(&buffer, "\nDEFVAR LF@", T_OTHERS) &&              //v pripade ne NULL definujeme promennou
                addCodeToBuf(&buffer, ID, T_OTHERS) &&
                addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&               //a prepiseme ji hodnotu vupocitaneho vyrazu
                addCodeToBuf(&buffer, ID, T_OTHERS));
    } else {
        return (addCodeToBuf(&buffer, "\nPUSHS bool@true", T_OTHERS) &&
                addCodeToBuf(&buffer, "JUMPIFNEQS $$if$", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *) &ifCounter, T_INT) && 
                addCodeToBuf(&buffer, "$else", T_OTHERS));
    }
    CodeStack_Push(ifStack, ifCounter);                
}

bool startElseGen() {
    return (addCodeToBuf(&buffer, "\nJUMP $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack), T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack), T_INT) &&
            addCodeToBuf(&buffer, "$else", T_OTHERS));
}

bool endIfElseGen() {
    if (addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) && 
        addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack), T_INT) &&
        addCodeToBuf(&buffer, "$end", T_OTHERS)) {
                CodeStack_Pop(ifStack);
                return true;
            } else return false;
}

char *replace_special_characters(const char *input)
{
    if (!input)
        return NULL;

    size_t buffer_size = 64;
    char *result = (char *)malloc(buffer_size);
    if (!result)
    {
        return NULL;
    }

    size_t result_len = 0;

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        const char *replacement = NULL;
        size_t replacement_len = 0;

        switch (input[i])
        {
        case ' ':
            replacement = "\\032";
            replacement_len = 4;
            break;
        case '\\':
            replacement = "\\092";
            replacement_len = 4;
            break;
        case '\n':
            replacement = "\\010";
            replacement_len = 4;
            break;
        case '#':
            replacement = "\\035";
            replacement_len = 4;
            break;
        default:
            replacement = &input[i];
            replacement_len = 1;
            break;
        }

        while (result_len + replacement_len + 1 > buffer_size)
        {
            buffer_size *= 2;
            char *new_result = (char *)realloc(result, buffer_size);
            if (!new_result)
            {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        if (replacement_len == 1)
        {
            result[result_len++] = *replacement;
        }
        else
        {
            for (size_t j = 0; j < replacement_len; j++)
            {
                result[result_len++] = replacement[j];
            }
        }
    }

    result[result_len] = '\0';
    return result;
}

// void main()
// {

//     //addCodeToBuf(&buffer, replace_special_characters("rete zec s lomitkem \\ a novym#radkem"), T_OTHERS);
//     startGen();
//     startMainGen();
//     defVarGen("prom", true);
//     assignVarGen("prom", T_STRING_TYPE, "rete zec s lomitkem \\ a novym#radkem", true, true);
//     endMainGen();
//     endGen();
//     disposeGen(true);
//     return;
// }