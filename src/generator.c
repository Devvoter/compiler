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
    return (bufInit(&buffer) && addCodeToBuf(&buffer, "\nCREATEFRAME", false) &&
            addCodeToBuf(&buffer, "\nPUSHFRAME", false));
}

bool endGen()
{
    return addCodeToBuf(&buffer, "\nPOPFRAME\n", false);
}

void disposeGen(bool done)
{
    if (done == true)
    {
        bufPrint(&buffer);
    }
    bufDestroy(buffer);
}

bool startMainGen()
{
    return (addCodeToBuf(&buffer, "\nCREATEFRAME", false) && addCodeToBuf(&buffer, "\nPUSHFRAME", false));
}

bool endMainGen()
{
    return addCodeToBuf(&buffer, "\nPOPFRAME", false);
}

bool defVarGen(char *ID, bool LF)
{
    if (addCodeToBuf(&buffer, "\nDEFVAR ", false))
    {
        if (LF == true)
        {
            if (addCodeToBuf(&buffer, "LF@", false))
            {
                return (addCodeToBuf(&buffer, ID, false));
            }
        }
        else
        {
            if (addCodeToBuf(&buffer, "TF@", false))
            {
                return (addCodeToBuf(&buffer, ID, false));
            }
        }
    }
    return false;
}

bool assignVarGen(char *ID, TokenType t, char *value, bool fromLF, bool toLF)
{
    if (addCodeToBuf(&buffer, "\nMOVE ", false))
    {
        if (toLF == true)
        {
            if (addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, ID, false))
            {
                if (t == T_VAR)
                {
                    if (fromLF == true)
                    {
                        return (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, value, false));
                    }
                    else
                    {
                        return (addCodeToBuf(&buffer, " TF@", false) && addCodeToBuf(&buffer, value, false));
                    }
                }
                else if (t == T_STRING_TYPE)
                {
                    value = replace_special_characters(value);
                    if (value == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, value, false));
                }
                else if (t == T_I32_ID)
                {
                    return (addCodeToBuf(&buffer, " int@", false) && addCodeToBuf(&buffer, value, false));
                }
                else if (t == T_F64_ID)
                {
                    return (addCodeToBuf(&buffer, " float@", false) && addCodeToBuf(&buffer, value, true));
                }
                else if (t == T_NULL)
                {
                    return addCodeToBuf(&buffer, " nil@nil", false);
                }
            }
        }
        else if (addCodeToBuf(&buffer, "TF@", false) && addCodeToBuf(&buffer, ID, false))
        {
            if (t == T_VAR)
            {
                if (fromLF == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, value, false));
                }
                else
                {
                    return (addCodeToBuf(&buffer, " TF@", false) && addCodeToBuf(&buffer, value, false));
                }
            }
            else if (t == T_STRING_TYPE)
            {
                value = replace_special_characters(value);
                if (value == NULL)
                {
                    return false;
                }
                return (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, value, false));
            }
            else if (t == T_I32_ID)
            {
                return (addCodeToBuf(&buffer, " int@", false) && addCodeToBuf(&buffer, value, false));
            }
            else if (t == T_F64_ID)
            {
                return (addCodeToBuf(&buffer, " float@", false) && addCodeToBuf(&buffer, value, true));
            }
            else if (t == T_NULL)
            {
                return (addCodeToBuf(&buffer, " nil@nil", false));
            }
        }
    }
    return false;
}

bool WriteStandFuncGen(TokenType t, char *param)
{
    if (addCodeToBuf(&buffer, "\nWRITE ", false))
    {
        if (t == T_VAR)
        {
            return (addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, param, false));
        }
        else if (t == T_I32_ID)
        {
            return (addCodeToBuf(&buffer, "int@", false) && addCodeToBuf(&buffer, param, false));
        }
        else if (t == T_F64_ID)
        {
            return (addCodeToBuf(&buffer, "float@", false) && addCodeToBuf(&buffer, param, true));
        }
        else if (t == T_STRING_TYPE)
        {
            param = replace_special_characters(param);
            if (param == NULL)
            {
                return false;
            }
            return (addCodeToBuf(&buffer, "string@", false) && addCodeToBuf(&buffer, param, false));
        }
    }
    return false;
}

bool ReadStandFuncGen(readFunc_t t, char *ID)
{
    if (addCodeToBuf(&buffer, "\nREAD ", false))
    {
        if (t == T_READSTR)
        {
            return (addCodeToBuf(&buffer, "@LF", false) && addCodeToBuf(&buffer, ID, false) && addCodeToBuf(&buffer, " string", false));
        }
        else if (t == T_READI32)
        {
            return (addCodeToBuf(&buffer, "@LF", false) && addCodeToBuf(&buffer, ID, false) && addCodeToBuf(&buffer, " int", false));
        }
        else if (t == T_READF64)
        {
            return (addCodeToBuf(&buffer, "@LF", false) && addCodeToBuf(&buffer, ID, false) && addCodeToBuf(&buffer, " float", false));
        }
    }
    return false;
}

bool StringStandFuncGen(char *ID, char *param)
{
    param = replace_special_characters(param);
    if (param == NULL)
    {
        return false;
    }
    return (addCodeToBuf(&buffer, "\nMOVE ", false) && 
            addCodeToBuf(&buffer, "LF@", false) &&
            addCodeToBuf(&buffer, ID, false) && 
            addCodeToBuf(&buffer, " string@", false) && 
            addCodeToBuf(&buffer, param, false));
}

bool LengthStandFuncGen(char *ID, char *param, bool isVar)
{
    // dobavit rozsireni mozna, aby mohlo byt soucasti vyrazu
    if (addCodeToBuf(&buffer, "\nSTRLEN ", false) && addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, ID, false))
    {
        if (isVar == false)
        {
            param = replace_special_characters(param);
            if (param = NULL)
            {
                return false;
            }
            return (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, param, false));
        }
        else if (isVar == true)
        {
            return (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, param, false));
        }
    }
    return false;
}

bool ConcatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2)
{
    if (addCodeToBuf(&buffer, "\nCONCAT ", false) && addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, ID, false))
    {
        if (isVar1 == true)
        {
            if (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, param1, false))
            {
                if (isVar2 == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, param2, false));
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, param2, false));
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
            if (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, param1, false))
            {
                if (isVar2 == true)
                {
                    return (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, param2, false));
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    return (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, param2, false));
                }
            }
        }
    }
    return false;
}

bool pushOnStackGen(char *param, TokenType t)
{
    if (addCodeToBuf(&buffer, "\nPUSHS ", false))
    {
        if (t == T_VAR)
        {
            return (addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, param, false));
        }
        else if (t == T_I32_ID)
        {
            return (addCodeToBuf(&buffer, "int@", false) && addCodeToBuf(&buffer, param, false));
        }
        else if (t == T_F64_ID)
        {
            return (addCodeToBuf(&buffer, "float@", false) && addCodeToBuf(&buffer, param, true));
        }
    }
    return false;
}

bool makeOperationStackGen(TokenType t, bool idiv)
{
    if (t == T_ADD)
    {
        return addCodeToBuf(&buffer, "\nADDS", false);
    }
    else if (t == T_SUB)
    {
        return addCodeToBuf(&buffer, "\nSUBS", false);
    }
    else if (t == T_MUL)
    {
        return addCodeToBuf(&buffer, "\nMULS", false);
    }
    else if (t == T_DIV)
    {
        if (idiv == false)
        {
            return addCodeToBuf(&buffer, "\nDIVS", false);
        }
        else
        {
            return addCodeToBuf(&buffer, "\nIDIVS", false);
        }
    }
    else if (t == T_LESS_THAN)
    {
        return addCodeToBuf(&buffer, "\nLTS", false);
    }
    else if (t == T_GREATER_THAN)
    {
        return addCodeToBuf(&buffer, "\nGTS", false);
    }
    return false;
}

bool endExpAssignGen(char *ID)
{
    return (addCodeToBuf(&buffer, "\nPOPS ", false) && addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, ID, false));
}

bool FuncEndGen()
{
    return (addCodeToBuf(&buffer, "\nPOPFRAME", false) && addCodeToBuf(&buffer, "\nRETURN", false));
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

void main()
{

    // char *cha = replace_special_characters("rete zec s lomitkem \\ a novym#radkem");
    startGen();
    startMainGen();
    defVarGen("prom", true);
    assignVarGen("prom", T_STRING_TYPE, "rete zec s lomitkem \\ a novym#radkem", true, true);
    endMainGen();
    endGen();
    disposeGen(true);
    return;
}