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

// inicializace buffru pro ulozeni kodu, pocitadel pro whily a ify
// a zasobniku pro ify, whily a nazvy promennych isNullable
codeBuf *buffer;
int ifCounter;
int whileCounter;
Stack *whileStack;
Stack *ifStack;
Stack *whileIsNullableStack;

bool startGen()
{
    ifCounter = 0;
    whileCounter = 0;
    CodeStack_Init(&ifStack);
    CodeStack_Init(&whileStack);
    CodeStack_Init(&whileIsNullableStack);
    return (bufInit(&buffer) && addCodeToBuf(&buffer, "\nCREATEFRAME\nPUSHFRAME", T_OTHERS));
}


void disposeGen(bool done)
{
    if (done == true)
        bufPrint(&buffer);
    CodeStack_destroy(ifStack);
    CodeStack_destroy(whileStack);
    CodeStack_destroy(whileIsNullableStack);
    bufDestroy(buffer);
}

bool startMainGen()
{
    return addCodeToBuf(&buffer, "\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@$tmp$", T_OTHERS);
}

bool endMainGen()
{
    return addCodeToBuf(&buffer, "\nEXIT int@0\nPOPFRAME", T_OTHERS);
}

bool defVarGen(char *ID, bool LF)
{
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nDEFVAR ", T_OTHERS))
    {
        if (LF)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS))
            {
                return (addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER));
            }
        }
        else
        {
            if (addCodeToBuf(&buffer, "TF@", T_OTHERS))
            {
                return (addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER));
            }
        }
    }
    return false;
}

bool assignVarGen(char *ID, TokenType t, char *value, bool fromLF, bool toLF)
{
    char *storedID = storeChar(ID);
    char *storedvalue = storeChar(value);
    if (storedID == NULL || storedvalue == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nMOVE ", T_OTHERS))
    {
        if (toLF)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
            {
                if (t == T_VAR)
                {
                    if (fromLF)
                    {
                        return (addCodeToBuf(&buffer, " LF@", T_OTHERS) &&
                                addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                    }
                    else
                    {
                        return (addCodeToBuf(&buffer, " TF@", T_OTHERS) &&
                                addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                    }
                }
                else if (t == T_STRING_TYPE)
                {
                    storedvalue = replace_special_characters(storedvalue);
                    if (storedvalue == NULL)
                        return false;
                    return (addCodeToBuf(&buffer, " string@", T_OTHERS) &&
                            addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                }
                else if (t == T_I32_ID)
                {
                    return (addCodeToBuf(&buffer, " int@", T_OTHERS) &&
                            addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                }
                else if (t == T_F64_ID)
                {
                    return (addCodeToBuf(&buffer, " float@", T_OTHERS) &&
                            addCodeToBuf(&buffer, storedvalue, T_FLOAT));
                }
                else if (t == T_NULL)
                {
                    return addCodeToBuf(&buffer, " nil@nil", T_OTHERS);
                }
            }
        }
        else if (addCodeToBuf(&buffer, "TF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
        {
            if (t == T_VAR)
            {
                if (fromLF)
                {
                    return (addCodeToBuf(&buffer, " LF@", T_OTHERS) &&
                            addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                }
                else
                {
                    return (addCodeToBuf(&buffer, " TF@", T_OTHERS) &&
                            addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
                }
            }
            else if (t == T_STRING_TYPE)
            {
                storedvalue = replace_special_characters(storedvalue);
                if (storedvalue == NULL)
                    return false;
                return (addCodeToBuf(&buffer, " string@", T_OTHERS) &&
                        addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
            }
            else if (t == T_I32_ID)
            {
                return (addCodeToBuf(&buffer, " int@", T_OTHERS) &&
                        addCodeToBuf(&buffer, storedvalue, T_STRING_FROM_PARSER));
            }
            else if (t == T_F64_ID)
            {
                return (addCodeToBuf(&buffer, " float@", T_OTHERS) &&
                        addCodeToBuf(&buffer, storedvalue, T_FLOAT));
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
            char *storedparam = storeChar(param);
            if (storedparam == NULL)
                return false;
            return (addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_I32_ID)
        {
            char *storedparam = storeChar(param);
            if (storedparam == NULL)
                return false;
            return (addCodeToBuf(&buffer, "int@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_F64_ID)
        {
            char *storedparam = storeChar(param);
            if (storedparam == NULL)
                return false;
            return (addCodeToBuf(&buffer, "float@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_FLOAT));
        }
        else if (t == T_STRING_TYPE)
        {
            char *storedparam = replace_special_characters(param);
            if (storedparam == NULL)
                return false;
            return (addCodeToBuf(&buffer, "string@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
    }
    return false;
}

bool readStandFuncGen(readFunc_t t, char *ID, bool pushOnStack)
{
    if (pushOnStack == true)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nREAD ", T_OTHERS))
    {
        if (t == T_READSTR)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER) &&
                addCodeToBuf(&buffer, " string", T_OTHERS))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(ID, T_VAR));
                }
                else
                {
                    return true;
                }
            }
        }
        else if (t == T_READI32)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER) &&
                addCodeToBuf(&buffer, " int", T_OTHERS))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(ID, T_VAR));
                }
                else
                {
                    return true;
                }
            }
        }
        else if (t == T_READF64)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER) &&
                addCodeToBuf(&buffer, " float", T_OTHERS))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(ID, T_VAR));
                }
                else
                    return true;
            }
        }
    }
    return false;
}

bool stringStandFuncGen(char *ID, char *param, bool pushOnStack)
{
    if (pushOnStack) ID = "$tmp$";
    char *storedID = storeChar(ID);
    char *storedparam = replace_special_characters(param);
    if (storedID == NULL || storedparam == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nMOVE LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER) &&
        addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING))
    {
        if (pushOnStack)
        {
            return (pushOnStackGen(storedID, T_VAR));
        }
        else
            return true;
    }
}

bool lengthStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nSTRLEN LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (!isVar)
        {
            char *storedparam = replace_special_characters(param);
            if (storedparam = NULL)
                return false;
            if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR));
                }
                else
                    return true;
            }
        }
        else // param je promenna
        {
            char *storedparam = storeChar(param);
            if (storedparam == NULL)
                return false;
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR));
                }
                else
                    return true;
            }
        }
    }
    return false;
}

bool concatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nCONCAT LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (isVar1)
        {
            char *storedparam1 = storeChar(param1);
            if (storedparam1 == NULL)
                return false;
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam1, T_STRING_FROM_PARSER))
            {
                if (isVar2)
                {
                    char *storedparam2 = storeChar(param2);
                    if (storedparam2 == NULL)
                        return false;
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
                else // param2 je string
                {
                    char *storedparam2 = replace_special_characters(param2);
                    if (storedparam2 == NULL)
                        return false;
                    if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
            }
        }
        else
        { // param1 je string
            char *storedparam1 = replace_special_characters(param1);
            if (storedparam1 == NULL)
                return false;
            if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam1, T_STRING))
            {
                if (isVar2)
                {
                    char *storedparam2 = storeChar(param2);
                    if (storedparam2 == NULL)
                        return false;
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
                else // param2 je string
                {
                    char *storedparam2 = replace_special_characters(param2);
                    if (param2 == NULL)
                        return false;
                    if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ordStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    char *storedparam2 = storeChar(param2);
    if (storedID == NULL || storedparam2 == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nSTRI2INT LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (isVar1)
        {
            char *storedparam1 = storeChar(param1);
            if (storedparam1 == NULL)
                return false;
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam1, T_STRING_FROM_PARSER))
            {
                if (isVar2)
                {
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
                else
                { // param2 je int
                    if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
            }
        }
        else
        { // param1 je string
            char *storedparam1 = replace_special_characters(param1);
            if (storedparam1 == NULL)
                return false;
            if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, storedparam1, T_STRING))
            {
                if (isVar2)
                {
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
                else
                { // param2 je int
                    if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, storedparam2, T_STRING_FROM_PARSER))
                    {
                        if (pushOnStack)
                        {
                            return (pushOnStackGen(storedID, T_VAR));
                        }
                        else
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool chrStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    char *storedparam = storeChar(param);
    if (storedID == NULL || storedparam == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nINT2CHAR LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (isVar)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR));
                }
                else
                    return true;
            }
        }
        else
        { // param je int
            if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, param, T_STRING_FROM_PARSER))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR));
                }
                else
                    return true;
            }
        }
    }
    return false;
}

bool i2fStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack, bool fromStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    char *storedparam = storeChar(param);
    if (storedID == NULL || storedparam == NULL)
        return false;
    bool popped = true;
    if (fromStack)
    {
        popped = addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_OTHERS);
    }
    if (addCodeToBuf(&buffer, "\nINT2FLOAT LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (isVar)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR) && popped);
                }
                else
                    return popped;
            }
        }
        else
        { // param je int
            if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR) && popped);
                }
                else
                    return popped;
            }
        }
    }
    return false;
}

bool f2iStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack, bool fromStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    char *storedparam = storeChar(param);
    if (storedID == NULL || storedparam == NULL)
        return false;
    bool popped = true;
    if (fromStack)
    {
        popped = addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_OTHERS);
    }
    if (addCodeToBuf(&buffer, "\nFLOAT2INT LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER))
    {
        if (isVar)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_OTHERS))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR) && popped);
                }
                else
                    return popped;
            }
        }
        else
        { // param je float
            if (addCodeToBuf(&buffer, " float@", T_OTHERS) && addCodeToBuf(&buffer, storedparam, T_FLOAT))
            {
                if (pushOnStack)
                {
                    return (pushOnStackGen(storedID, T_VAR) && popped);
                }
                else
                    return popped;
            }
        }
    }
    return false;
}

bool substringStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, char *param3, bool isVar3, bool pushOnStack)
{
    //TODO
    return false;
}

bool substringGen()
{
    return (addCodeToBuf(&buffer, "\nLABEL $$ifj_substring$$\nPUSHFRAME\nDEFVAR LF@$$$retval\nMOVE LF@$$$retval string@\nPUSHS bool@true\nPUSHS int@0\nPUSHS LF@startChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS int@0\nPUSHS LF@endChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@startChar\nPUSHS LF@endChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@startChar\nDEFVAR LF@length\nSTRLEN LF@length LF@string\nPUSHS LF@length\nLTS\nJUMPIFNEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@endChar\nPUSHS LF@length\nGTS\nJUMPIFEQS $$notOk$$\nDEFVAR LF@tmp\nJUMP $$startCopy$$\nLABEL $$startWhile$$\nADD LF@startChar LF@startChar int@1\nJUMPIFEQ $$finishWhile$$ LF@startChar LF@endChar\nLABEL $$startCopy$$\nGETCHAR LF@tmp LF@string LF@startChar\nCONCAT LF@$$$retval LF@$$$retval LF@tmp\nJUMP $$startWhile$$\nJUMP $$end$$\nLABEL $$notOk$$\nMOVE LF@$$$retval nil@nil\nLABEL $$finishWhile$$\nPOPFRAME\nRETURN", T_OTHERS));
}

bool strcmpFuncGen()
{
    //TODO
    return false;
}

bool strcmpGen() {
    return (addCodeToBuf(&buffer, "\nLABEL $$ifj_strcmp$$\nPUSHFRAME\nDEFVAR LF@$$$retval\nPUSHS LF@string1\nPUSHS LF@string2\nEQS\nPUSHS bool@true\nJUMPIFEQS $$success$$\nPUSHS LF@string1\nPUSHS LF@string2\nLTS\nPUSHS bool@true\nJUMPIFEQS $$less$$\nPUSHS LF@string1\nPUSHS LF@string2\nGTS\nPUSHS bool@true\nJUMPIFEQS $$greater$$\nLABEL $$success$$\nMOVE LF@$$$retval int@0\nJUMP $$finishStrcmp$$\nLABEL $$less$$\nMOVE LF@$$$retval int@-1\nJUMP $$finishStrcmp$$\nLABEL $$greater$$\nMOVE LF@$$$retval int@1\nJUMP $$finishStrcmp$$\nLABEL $$finishStrcmp$$\nPOPFRAME\nRETURN", T_OTHERS));
}

bool pushOnStackGen(char *param, TokenType t)
{
    if (addCodeToBuf(&buffer, "\nPUSHS ", T_OTHERS))
    {
        if (t == T_NULL)
        {
            return addCodeToBuf(&buffer, "nil@nil", T_OTHERS);
        }
        char *storedparam = storeChar(param);
        if (storedparam == NULL)
            return false;
        if (t == T_VAR)
        {
            return (addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_I32_ID)
        {
            return (addCodeToBuf(&buffer, "int@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_F64_ID)
        {
            return (addCodeToBuf(&buffer, "float@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_FLOAT));
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
        if (idiv)
        {
            return addCodeToBuf(&buffer, "\nIDIVS", T_OTHERS);
        }
        else
        {
            return addCodeToBuf(&buffer, "\nDIVS", T_OTHERS);
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
    else if (t == T_LESS_OR_EQUAL)
    {
        return addCodeToBuf(&buffer, "\nGTS\nNOTS", T_OTHERS);
    }
    else if (t == T_GREATER_OR_EQUAL)
    {
        return addCodeToBuf(&buffer, "\nLTS\nNOTS", T_OTHERS);
    }
    else if (t == T_EQUALS) 
    {
        return addCodeToBuf(&buffer, "\nEQS", T_OTHERS);
    }
    else if (t == T_NOT_EQUALS) 
    {
        return addCodeToBuf(&buffer, "\nEQS\nNOT", T_OTHERS);
    } 
    else return false;
}

bool endExpAssignGen(char *ID)
{
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    return (addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&
            addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER));
}

bool startIfGen(bool withNull, char *ID)
{
    ifCounter++;
    if (CodeStack_Push(ifStack, &ifCounter, T_COUNTER))
    {
        if (withNull)
        {
            char *storedID = storeChar(ID);
            if (storedID == NULL)
                return false;
            return (addCodeToBuf(&buffer, "\nPUSHS nil@nil", T_OTHERS) &&   // push na zasobnik null hodnoty
                    addCodeToBuf(&buffer, "\nJUMPIFEQS $$if$", T_OTHERS) && // porovname null s vysledkem vyrazu a udelame skok
                    addCodeToBuf(&buffer, (void *)&ifCounter, T_INT) &&
                    addCodeToBuf(&buffer, "$else", T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nDEFVAR LF@", T_OTHERS) && // v pripade ne NULL definujeme promennou
                    addCodeToBuf(&buffer, storedID, T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) && // popneme null ze zasobniku
                    addCodeToBuf(&buffer, storedID, T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) && // a popneme hodnotu vyrazu do promenne
                    addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER));
        }
        else
        {
            return (addCodeToBuf(&buffer, "\nPUSHS bool@true", T_OTHERS) &&
                    addCodeToBuf(&buffer, "JUMPIFNEQS $$if$", T_OTHERS) &&
                    addCodeToBuf(&buffer, (void *)&ifCounter, T_INT) &&
                    addCodeToBuf(&buffer, "$else", T_OTHERS));
        }
    }
}

bool startElseGen()
{
    return (addCodeToBuf(&buffer, "\nJUMP $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$else", T_OTHERS));
}

bool endIfElseGen(bool withElse)
{
    if (withElse)
    {
        if (addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS))
        {
            CodeStack_Pop(ifStack);
            return true;
        }
        else
            return false;
    }
    else
    {
        if (addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$else", T_OTHERS))
        {
            CodeStack_Pop(ifStack);
            return true;
        }
        else
            return false;
    }
}

bool startWhileGen()
{
    whileCounter++;
    if (CodeStack_Push(whileStack, (void *)&whileCounter, T_COUNTER))
    {
        return (addCodeToBuf(&buffer, "\nDEFVAR LF@$$while$isNullable$", T_OTHERS) && // definovani pomocne promenne
                addCodeToBuf(&buffer, (void *)&whileCounter, T_INT) &&                // pro pocitani s IsNullable
                addCodeToBuf(&buffer, "\nLABEL $$while$", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *)&whileCounter, T_INT));
    }
}

bool endCondWhileGen(bool isNullable, char *ID)
{
    if (isNullable)
    {
        if (addCodeToBuf(&buffer, "\nPUSHS nil@nil", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nJUMPIFEQS $$while$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)&whileCounter, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS) &&

            addCodeToBuf(&buffer, "\nPOPS LF@$$while$isNullable$", T_OTHERS) && // ID = hodnota vyrazu
            addCodeToBuf(&buffer, (void *)&whileCounter, T_INT) &&
            addCodeToBuf(&buffer, "\nPOPS LF@$$while$isNullable$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)&whileCounter, T_INT))
        {
            return CodeStack_Push(whileIsNullableStack, (void *)ID, T_WHILE_IS_NULLABLE);
        }
    }
    else
    {
        return (addCodeToBuf(&buffer, "\nPUSHS bool@true", T_OTHERS) &&
                addCodeToBuf(&buffer, "\nJUMPIFNEQS $$while$", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *)&whileCounter, T_INT) &&
                addCodeToBuf(&buffer, "$end", T_OTHERS));
    }
}

bool endWhileGen()
{
    if (addCodeToBuf(&buffer, "\nJUMP $$while$", T_OTHERS) &&
        addCodeToBuf(&buffer, (void *)CodeStack_Top(whileStack)->data, T_INT) &&
        addCodeToBuf(&buffer, "\nLABEL $$while$", T_OTHERS) &&
        addCodeToBuf(&buffer, (void *)CodeStack_Top(whileStack)->data, T_INT) &&
        addCodeToBuf(&buffer, "$end", T_OTHERS))
    {
        CodeStack_Pop(whileStack);
        CodeStack_Pop(whileIsNullableStack);
        return true;
    }
    else
        return false;
}

bool callFuncGen(char *name, int paramsCount)
{
    bool paramsWritten = true;
    if (addCodeToBuf(&buffer, "\nCREATEFRAME", T_OTHERS))
    {
        while (paramsCount != 0)
        {
            if (addCodeToBuf(&buffer, "\nDEFVAR TF@$$$param", T_OTHERS) &&
                addCodeToBuf(&buffer, (int *)&paramsCount, T_INT) &&
                addCodeToBuf(&buffer, "\nPOPS TF@$$$param", T_OTHERS) &&
                addCodeToBuf(&buffer, (int *)&paramsCount, T_INT))
            {
                paramsCount--;
            }
            else
            {
                paramsWritten = false;
                break;
            }
        }
        char *storedname = storeChar(name);
        if (storedname == NULL)
            return false;
        if (addCodeToBuf(&buffer, "\nCALL $$$", T_OTHERS) &&
            addCodeToBuf(&buffer, storedname, T_STRING_FROM_PARSER))
        {
            return paramsWritten;
        }
    }
}

bool retValGen(char *ID, bool pushOnStack)
{
    if (pushOnStack)
        ID = "$tmp$";
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    if (assignVarGen(storedID, T_VAR, "$$$retval", false, true))
    {
        if (pushOnStack)
            return pushOnStackGen(storedID, T_VAR);
        else
            return true;
    }
    else
        return false;
}

bool funcStartGen(char *name)
{
    char *storedname = storeChar(name);
    if (storedname == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nLABEL $$$", T_OTHERS) &&
        addCodeToBuf(&buffer, storedname, T_STRING_FROM_PARSER) &&
        addCodeToBuf(&buffer, "\nPUSHFRAME", T_OTHERS) &&
        addCodeToBuf(&buffer, "\nDEFVAR LF@$$$retval", T_OTHERS) &&
        addCodeToBuf(&buffer, "\nMOVE LF@$$$retval nil@nil", T_OTHERS))
    {
        // TODO prace s parametry
    }
    else
        return false;
}

bool funcEndGen()
{
    return (addCodeToBuf(&buffer, "\nPOPFRAME\nRETURN", T_OTHERS));
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

char *storeChar(char *ID)
{
    if (CodeStack_IsEmpty(whileIsNullableStack) || strcmp(ID, (char *)CodeStack_Top(whileIsNullableStack)->data) != 0)
    {
        int length = strlen(ID);
        char *ch = malloc(length + 1);
        strcpy(ch, ID);
        return ch;
    }
    else
    { // prepiseme promennu na $while$isNullable$1
        int num = *((int *)CodeStack_Top(whileStack)->data);
        int digits = 0;
        while (num)
        {
            digits++;
            num /= 10;
        }
        char *firstPart = "$$while$isNullable$";
        char *ch = malloc(strlen(firstPart) + digits + 1);
        if (ch == NULL)
        {
            return NULL;
        }
        num = *(int *)(CodeStack_Top(whileStack)->data);
        for (int i = 0; i < 19; i++)
        {
            ch[i] = firstPart[i];
        }
        for (int i = (19 + digits - 1); i >= 19; i--)
        {
            ch[i] = num % 10 + '0';
            num /= 10;
        }
        ch[19 + digits] = '\0';
        return ch;
    }
}

bool endGen()
{
    return (substringGen() && strcmpGen() && addCodeToBuf(&buffer, "\nCLEARS\nPOPFRAME\n", T_OTHERS));
}

void main()
{
    startGen();
    startMainGen();
    endMainGen();
    endGen();
    disposeGen(true);
}