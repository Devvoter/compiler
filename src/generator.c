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
//a zasobniku pro ify, whily a nazvy promennych isNullable
codeBuf *buffer;
int ifCounter;
int whileCounter;
Stack *whileStack, *ifStack, *whileIsNullableStack;


bool startGen()
{
    ifCounter = 0;
    whileCounter = 0;
    CodeStack_Init(&ifStack);
    CodeStack_Init(&whileStack);
    CodeStack_Init(&whileIsNullableStack);
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
    CodeStack_destroy(whileStack);
    CodeStack_destroy(whileIsNullableStack);
    bufDestroy(buffer);
}


bool startMainGen()
{
    return (addCodeToBuf(&buffer, "\nCREATEFRAME", T_OTHERS) && 
            addCodeToBuf(&buffer, "\nPUSHFRAME", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nDEFVAR LF@$tmp$", T_OTHERS));
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


bool readStandFuncGen(readFunc_t t, char *ID, bool pushOnStack)
{
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nREAD ", T_OTHERS))
    {
        if (t == T_READSTR)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS) && 
                addCodeToBuf(&buffer, " string", T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else {
                        return true;
                    }
            }
        }
        else if (t == T_READI32)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS) && 
                addCodeToBuf(&buffer, " int", T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else {
                        return true;
                    }
            }
        }
        else if (t == T_READF64)
        {
            if (addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS) && 
                addCodeToBuf(&buffer, " float", T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else return true;
            }
        }
    }
    return false;
}


bool stringStandFuncGen(char *ID, char *param, bool pushOnStack)
{
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    param = replace_special_characters(param);
    if (param == NULL)
    {
        return false;
    }
    if (addCodeToBuf(&buffer, "\nMOVE ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS) && addCodeToBuf(&buffer, " string@", T_OTHERS) && 
        addCodeToBuf(&buffer, param, T_STRING)) {
                if (pushOnStack == true) {
                    return (pushOnStackGen(ID, T_VAR));
                } else return true;
            }
}


bool lengthStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack)
{
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nSTRLEN ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
    {
        if (isVar == false)
        {
            param = replace_special_characters(param);
            if (param = NULL)
            {
                return false;
            }
            if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param, T_STRING)) {
                if (pushOnStack == true) {
                    return (pushOnStackGen(ID, T_VAR));
                } else return true;
            }
        }
        else if (isVar == true)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS)) {
                if (pushOnStack == true) {
                    return (pushOnStackGen(ID, T_VAR));
                } else return true;
            }
        }
    }
    return false;
}


bool concatStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack)
{
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nCONCAT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, ID, T_OTHERS))
    {
        if (isVar1 == true)
        {
            if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_OTHERS))
            {
                if (isVar2 == true)
                {
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_STRING)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
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
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
                }
                else
                {
                    param2 = replace_special_characters(param2);
                    if (param2 == NULL)
                    {
                        return false;
                    }
                    if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_STRING)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
                }
            }
        }
    }
    return false;
}


bool ordStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, bool pushOnStack) {
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nSTRI2INT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
            if (isVar1 == true) {
                if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_OTHERS)) {
                    if (isVar2 == true) {
                        if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                            if (pushOnStack == true) {
                                return (pushOnStackGen(ID, T_VAR));
                            } else return true;
                        }
                    } else {
                        if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                            if (pushOnStack == true) {
                                return (pushOnStackGen(ID, T_VAR));
                            } else return true;
                        }
                    }
                } 
            } else {
                param1 = replace_special_characters(param1);
                if (param2 == NULL) {
                    return false;
                }
                if (addCodeToBuf(&buffer, " string@", T_OTHERS) && addCodeToBuf(&buffer, param1, T_STRING)) {
                   if (isVar2 == true) {
                        if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                            if (pushOnStack == true) {
                                return (pushOnStackGen(ID, T_VAR));
                            } else return true;
                        }
                    } else {
                        if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, param2, T_OTHERS)) {
                            if (pushOnStack == true) {
                                return (pushOnStackGen(ID, T_VAR));
                            } else return true;
                        }
                    } 
                }
            }
    }
    return false;
}


bool chrStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack) {
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nINT2CHAR ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
                if (isVar == true) {
                    if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
                } else {
                    if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, param, T_INT)) {
                        if (pushOnStack == true) {
                            return (pushOnStackGen(ID, T_VAR));
                        } else return true;
                    }
                }
        }
    return false;
}


bool i2fStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack) {
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nINT2FLOAT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
            if (isVar == true) {
                if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else return true;
                }
            } else {
                if (addCodeToBuf(&buffer, " int@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else return true;
                }
            }
    }
    return false;
}


bool f2iStandFuncGen(char *ID, char *param, bool isVar, bool pushOnStack) {
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
    if (addCodeToBuf(&buffer, "\nFLOAT2INT ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
        addCodeToBuf(&buffer, ID, T_OTHERS)) {
            if (isVar == true) {
                if (addCodeToBuf(&buffer, " LF@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else return true;
                }
            } else {
                if (addCodeToBuf(&buffer, " float@", T_OTHERS) && addCodeToBuf(&buffer, param, T_FLOAT)) {
                    if (pushOnStack == true) {
                        return (pushOnStackGen(ID, T_VAR));
                    } else return true;
                }
            }
    }
    return false;
}


bool substringStandFuncGen(char *ID, char *param1, bool isVar1, char *param2, bool isVar2, char *param3, bool isVar3, bool pushOnStack) {
    if (pushOnStack == true) {
        ID = "$tmp$";
    }
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
            addCodeToBuf(&buffer, ID, T_OTHERS) &&
            addCodeToBuf(&buffer, "\nCLEARS", T_OTHERS));
}


bool funcEndGen()
{
    return (addCodeToBuf(&buffer, "\nPOPFRAME\nRETURN", T_OTHERS));
}


bool startIfGen(bool withNull, char *ID) {
    ifCounter++;
    if (CodeStack_Push(ifStack, &ifCounter, T_COUNTER)) {
        if (withNull) {
            return (addCodeToBuf(&buffer, "\nPUSHS nil@nil", T_OTHERS) &&           //push na zasobnik null hodnoty 
                    addCodeToBuf(&buffer, "\nJUMPIFEQS $$if$", T_OTHERS) &&         //porovname null s vysledkem vyrazu a udelame skok
                    addCodeToBuf(&buffer, (void *) &ifCounter, T_INT) &&                   
                    addCodeToBuf(&buffer, "$else", T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nDEFVAR LF@", T_OTHERS) &&              //v pripade ne NULL definujeme promennou
                    addCodeToBuf(&buffer, ID, T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&               //popneme null ze zasobniku
                    addCodeToBuf(&buffer, ID, T_OTHERS) &&
                    addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&               //a popneme hodnotu vyrazu do promenne
                    addCodeToBuf(&buffer, ID, T_OTHERS));
        } else {
            return (addCodeToBuf(&buffer, "\nPUSHS bool@true", T_OTHERS) &&
                    addCodeToBuf(&buffer, "JUMPIFNEQS $$if$", T_OTHERS) &&
                    addCodeToBuf(&buffer, (void *) &ifCounter, T_INT) && 
                    addCodeToBuf(&buffer, "$else", T_OTHERS));
        }   
    }           
}


bool startElseGen() {
    return (addCodeToBuf(&buffer, "\nJUMP $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *)CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$else", T_OTHERS));
}


bool endIfElseGen(bool withElse) {
    if (withElse) {
        if (addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) && 
            addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS)) {
                CodeStack_Pop(ifStack);
                return true;
            } else return false;
    } else {
        if (addCodeToBuf(&buffer, "\nLABEL $$if$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *) CodeStack_Top(ifStack)->data, T_INT) &&
            addCodeToBuf(&buffer, "$else", T_OTHERS)) {
                CodeStack_Pop(ifStack);
                return true;
            } else return false;
    }
    
}


bool startWhileGen() {
    whileCounter++;
    if (CodeStack_Push(whileStack, (void *) &whileCounter, T_COUNTER)) {
        return (addCodeToBuf(&buffer, "\nDEFVAR LF@", T_OTHERS) &&
                addCodeToBuf(&buffer, "$$while$isNullable$", T_OTHERS) &&   //definovani pomocne promenne
                addCodeToBuf(&buffer, (void *) &whileCounter, T_INT) &&    //pro pocitani s IsNullable
                addCodeToBuf(&buffer, "\nLABEL $$while$", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *) &whileCounter, T_INT));
    }
}


bool endCondWhileGen(bool isNullable, char *ID) {
    if (isNullable) {
        if (addCodeToBuf(&buffer, "\nPUSHS nil@nil", T_OTHERS) &&
            addCodeToBuf(&buffer, "\nJUMPIFEQS $$while$", T_OTHERS) &&
            addCodeToBuf(&buffer, (void *) &whileCounter, T_INT) &&
            addCodeToBuf(&buffer, "$end", T_OTHERS) &&
            
            addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&                     //ID = hodnota vyrazu
            addCodeToBuf(&buffer, ID, T_OTHERS) &&
            addCodeToBuf(&buffer, "\nPOPS LF@", T_OTHERS) &&
            addCodeToBuf(&buffer, ID, T_OTHERS) &&

            addCodeToBuf(&buffer, "\nMOVE LF@$$while$isNullable$", T_OTHERS) &&  //$$while$isNullable$1 = ID
            addCodeToBuf(&buffer, (void *) &whileCounter, T_INT) &&
            addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
            addCodeToBuf(&buffer, ID, T_OTHERS)) {
                return CodeStack_Push(whileIsNullableStack, (void *)ID, T_WHILE_IS_NULLABLE);
            }
    } else {
        return (addCodeToBuf(&buffer, "\nPUSHS bool@true", T_OTHERS) &&
                addCodeToBuf(&buffer, "\nJUMPIFNEQS $$while$", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *) &whileCounter, T_INT) &&
                addCodeToBuf(&buffer, "$end", T_OTHERS)); 
    }
}


bool endWhileGen() {
    if (addCodeToBuf(&buffer, "\nJUMP $$while$", T_OTHERS) &&
        addCodeToBuf(&buffer, (void *) CodeStack_Top(whileStack)->data, T_INT) &&
        addCodeToBuf(&buffer, "\nLABEL $$while$", T_OTHERS) &&
        addCodeToBuf(&buffer, (void *) CodeStack_Top(whileStack)->data, T_INT) &&
        addCodeToBuf(&buffer, "$end", T_OTHERS)) {
           CodeStack_Pop(whileStack); 
           CodeStack_Pop(whileIsNullableStack);
    }
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

char *isNullableVar(char *ID) {
    if (strcmp(ID, (char *)CodeStack_Top(whileIsNullableStack)->data) == 0) {
        return ID;
    } else {   
        int num = *((int *) CodeStack_Top(whileStack)->data);
        int digits = 0;
        while (num) {
            digits++;
            num /= 10;
        }
        char *firstPart = "$$while$isNullable$";
        char *ch = malloc(sizeof(firstPart) + digits);
        num = *(int *) (CodeStack_Top(whileStack)->data);
        for (int i = 0; i < 19; i++) {
            ch[i] = firstPart[i];
        }
        for (int i = (19+digits-1); i >= 19; i--) {
            ch[i] = num % 10 + '0';
            num /= 10;
        }
        ch[19+digits] = '\0';
        return ch;
    }
}


void main()
{
    //addCodeToBuf(&buffer, replace_special_characters("rete zec s lomitkem \\ a novym#radkem"), T_OTHERS);
    startGen();
    //startMainGen();
    // defVarGen("prom", true);
    // assignVarGen("prom", T_STRING_TYPE, "rete zec s lomitkem \\ a novym#radkem", true, true);
    // f2iStandFuncGen("newVar", "330.22", false, false);
    // readStandFuncGen(T_READF64, NULL, true);
    //startWhileGen();
    //startWhileGen();
    //endMainGen();
    //endGen();
    //disposeGen(true);
    char *ID = "prom";
    CodeStack_Push(whileIsNullableStack, ID, T_WHILE_IS_NULLABLE);
    int cislo = 255;
    CodeStack_Push(whileStack, &cislo, T_COUNTER);
    char *ptr = isNullableVar(ID);
    printf("%s\n", ptr);
    endGen();
    return;
}