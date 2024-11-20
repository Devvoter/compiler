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
    if (bufInit(&buffer) && addCodeToBuf(&buffer, "\nCREATEFRAME") &&
        addCodeToBuf(&buffer, "\nPUSHFRAME"))
    {
        return true;
    }
    return false;
}

bool endGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME\n"))
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
    if (addCodeToBuf(&buffer, "\nCREATEFRAME") && addCodeToBuf(&buffer, "\nPUSHFRAME"))
    {
        return true;
    }
    return false;
}

bool endMainGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME"))
    {
        return true;
    }
    return false;
}

bool defVarGen(char *ID, bool LF)
{
    if (addCodeToBuf(&buffer, "\nDEFVAR "))
    {
        if (LF == true)
        {
            if (addCodeToBuf(&buffer, "LF@"))
            {
                if (addCodeToBuf(&buffer, ID))
                {
                    return true;
                }
            }
        }
        else
        {
            if (addCodeToBuf(&buffer, "TF@"))
            {
                if (addCodeToBuf(&buffer, ID))
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
    if (addCodeToBuf(&buffer, "\nMOVE "))
    {
        if (toLF == true)
        {
            if (addCodeToBuf(&buffer, "LF@") && addCodeToBuf(&buffer, ID))
            {
                if (t == T_VAR)
                {
                    if (fromLF == true)
                    {
                        if (addCodeToBuf(&buffer, " LF@") && addCodeToBuf(&buffer, value))
                        {
                            return true;
                        }
                    }
                    else if (addCodeToBuf(&buffer, " TF@") && addCodeToBuf(&buffer, value))
                    {
                        return true;
                    }
                }
                else if (t == T_STRING)
                {
                    // TODO
                }
                else if (t == T_I32_ID)
                {
                    if (addCodeToBuf(&buffer, " int@") && addCodeToBuf(&buffer, value))
                    {
                        return true;
                    }
                }
                else if (t == T_F64_ID)
                {
                    if (addCodeToBuf(&buffer, " float@") && addCodeToBuf(&buffer, value))
                    {
                        return true;
                    }
                }
                else
                {
                    if (addCodeToBuf(&buffer, " nil@nil"))
                    {
                        return true;
                    }
                }
            }
        }
        else if (addCodeToBuf(&buffer, "TF@") && addCodeToBuf(&buffer, ID))
        {
            if (t == T_VAR)
            {
                if (fromLF == true)
                {
                    if (addCodeToBuf(&buffer, " LF@") && addCodeToBuf(&buffer, value))
                    {
                        return true;
                    }
                }
                else if (addCodeToBuf(&buffer, " TF@") && addCodeToBuf(&buffer, value))
                {
                    return true;
                }
            }
            else if (t == T_STRING)
            {
                // TODO
            }
            else if (t == T_I32_ID)
            {
                if (addCodeToBuf(&buffer, " int@") && addCodeToBuf(&buffer, value))
                {
                    return true;
                }
            }
            else if (t == T_F64_ID)
            {
                if (addCodeToBuf(&buffer, " float@") && addCodeToBuf(&buffer, value))
                {
                    return true;
                }
            }
            else
            {
                if (addCodeToBuf(&buffer, " nil@nil"))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool callStandFuncGen(standFunc_t t, bool assign, bool pushOnStack, char *ID, char *param1, char *param2, char *param3)
{
    if (t == T_WRITE)
    {
        //TODO
    }
    else if (t == T_READSTR)
    {
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
}

bool FuncEndGen()
{
    if (addCodeToBuf(&buffer, "\nPOPFRAME") && addCodeToBuf(&buffer, "\nRETURN"))
    {
        return true;
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