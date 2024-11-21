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
    if (bufInit(&buffer) && addCodeToBuf(&buffer, "\nCREATEFRAME", false) &&
        addCodeToBuf(&buffer, "\nPUSHFRAME", false))
    {
        return true;
    }
    return false;
}

bool endGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME\n", false))
    {
        return true;
    }
    return false;
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
    if (addCodeToBuf(&buffer, "\nCREATEFRAME", false) && addCodeToBuf(&buffer, "\nPUSHFRAME", false))
    {
        return true;
    }
    return false;
}

bool endMainGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME", false))
    {
        return true;
    }
    return false;
}

bool defVarGen(char *ID, bool LF)
{
    if (addCodeToBuf(&buffer, "\nDEFVAR ", false))
    {
        if (LF == true)
        {
            if (addCodeToBuf(&buffer, "LF@", false))
            {
                if (addCodeToBuf(&buffer, ID, false))
                {
                    return true;
                }
            }
        }
        else
        {
            if (addCodeToBuf(&buffer, "TF@", false))
            {
                if (addCodeToBuf(&buffer, ID, false))
                {
                    return true;
                }
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
                        if (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, value, false))
                        {
                            return true;
                        }
                    }
                    else if (addCodeToBuf(&buffer, " TF@", false) && addCodeToBuf(&buffer, value, false))
                    {
                        return true;
                    }
                }
                else if (t == T_STRING)
                {
                    value = replace_special_characters(value);
                    if (value == NULL)
                    {
                        return false;
                    }
                    if (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, value, false))
                    {
                        return true;
                    }
                }
                else if (t == T_I32_ID)
                {
                    if (addCodeToBuf(&buffer, " int@", false) && addCodeToBuf(&buffer, value, false))
                    {
                        return true;
                    }
                }
                else if (t == T_F64_ID)
                {
                    if (addCodeToBuf(&buffer, " float@", false) && addCodeToBuf(&buffer, value, true))
                    {
                        return true;
                    }
                }
                else if (t == T_NULL)
                {
                    if (addCodeToBuf(&buffer, " nil@nil", false))
                    {
                        return true;
                    }
                }
            }
        }
        else if (addCodeToBuf(&buffer, "TF@", false) && addCodeToBuf(&buffer, ID, false))
        {
            if (t == T_VAR)
            {
                if (fromLF == true)
                {
                    if (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, value, false))
                    {
                        return true;
                    }
                }
                else if (addCodeToBuf(&buffer, " TF@", false) && addCodeToBuf(&buffer, value, false))
                {
                    return true;
                }
            }
            else if (t == T_STRING)
            {
                value = replace_special_characters(value);
                if (value == NULL)
                {
                    return false;
                }
                if (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, value, false))
                {
                    return true;
                }
            }
            else if (t == T_I32_ID)
            {
                if (addCodeToBuf(&buffer, " int@", false) && addCodeToBuf(&buffer, value, false))
                {
                    return true;
                }
            }
            else if (t == T_F64_ID)
            {
                if (addCodeToBuf(&buffer, " float@", false) && addCodeToBuf(&buffer, value, true))
                {
                    return true;
                }
            }
            else if (t == T_NULL)
            {
                if (addCodeToBuf(&buffer, " nil@nil", false))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool callStandFuncGen(standFunc_t t, TokenType t2, bool assign, bool pushOnStack, char *ID, char *param1, char *param2, char *param3)
{
    if (t == T_WRITE)
    {
        if (addCodeToBuf(&buffer, "\nWRITE ", false))
        {
            if (t2 == T_VAR)
            {
                if (addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, param1, false))
                {
                    return true;
                }
            }
            else if (t2 == T_I32_ID)
            {
                if (addCodeToBuf(&buffer, "int@", false) && addCodeToBuf(&buffer, param1, false))
                {
                    return true;
                }
            }
            else if (t2 == T_F64_ID)
            {
                if (addCodeToBuf(&buffer, "float@", false) && addCodeToBuf(&buffer, param1, true))
                {
                    return true;
                }
            }
            else if (t2 == T_STRING)
            {
                param1 = replace_special_characters(param1);
                if (param1 == NULL)
                {
                    return false;
                }
                if (addCodeToBuf(&buffer, "string@", false) && addCodeToBuf(&buffer, param1, false))
                {
                    return true;
                }
            }
        }
    }
    else if (t == T_READSTR)
    {
        //musi se nacist do promenne? READ <var> <type>
    }
    else if (t == T_READI32)
    {
    }
    else if (t == T_READF64)
    {
    }
    else if (t == T_STRING)
    {
    }
    else if (t == T_LENGTH)
    {
        //take se musi nacist do promenne?
        if (addCodeToBuf(&buffer, "\nSTRLEN ", false) && addCodeToBuf(&buffer, "LF@", false) && addCodeToBuf(&buffer, ID, false)) {
            if (t == T_STRING) {
                param1 = replace_special_characters(param1);
                if (param1 = NULL) {
                    return false;
                }
                if (addCodeToBuf(&buffer, " string@", false) && addCodeToBuf(&buffer, param1, false)) {
                    return true;
                }
            } else if (t == T_VAR) {
                if (addCodeToBuf(&buffer, " LF@", false) && addCodeToBuf(&buffer, param1, false)) {
                    return true;
                }
            }
        }
    }
    else if (t == T_CONCAT)
    {
    }
    else if (t == T_SUBSTRING)
    {
    }
    else if (t == T_STRCMP)
    {
    }
    else if (t == T_ORD)
    {
    }
    else if (t == T_CHR)
    {
    }
    else if (t == T_I2F)
    {
    }
    else if (t == T_F2I)
    {
    }
    return false;
}

bool FuncEndGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME", false) && addCodeToBuf(&buffer, "\nRETURN", false))
    {
        return true;
    }
    return false;
}

char *replace_special_characters(const char *input) {
    if (!input) return NULL;

    size_t buffer_size = 64;
    char *result = (char *)malloc(buffer_size);
    if (!result) {
        return NULL;
    }

    size_t result_len = 0;

    for (size_t i = 0; input[i] != '\0'; i++) {
        const char *replacement = NULL;
        size_t replacement_len = 0;

        switch (input[i]) {
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

        while (result_len + replacement_len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_result = (char *)realloc(result, buffer_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        if (replacement_len == 1) {
            result[result_len++] = *replacement;
        } else {
            for (size_t j = 0; j < replacement_len; j++) {
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
    assignVarGen("prom", T_STRING, "rete zec s lomitkem \\ a novym#radkem", true, true);
    endMainGen();
    endGen();
    disposeGen(true);
    return;
}