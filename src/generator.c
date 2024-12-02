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
    if (done)
        bufPrint(&buffer);
    CodeStack_destroy(ifStack);
    CodeStack_destroy(whileStack);
    CodeStack_destroy(whileIsNullableStack);
    bufDestroy(buffer);
}


bool startMainGen()
{
    return addCodeToBuf(&buffer, "\nCREATEFRAME\nPUSHFRAME\nDEFVAR LF@$tmp$\nDEFVAR LF@$str_strlen$\nDEFVAR LF@$concat_string1$\nDEFVAR LF@$concat_string2$\nDEFVAR LF@$stri2int_string$\nDEFVAR LF@$stri2int_int$\nDEFVAR LF@$tmp_num$", T_OTHERS);
}


bool endMainGen()
{
    return addCodeToBuf(&buffer, "\nEXIT int@0\nPOPFRAME", T_OTHERS);
}


bool defVarGen(char *ID)
{
    char *storedID = storeChar(ID);
    if (storedID == NULL)
        return false;
    return (addCodeToBuf(&buffer, "\nDEFVAR ", T_OTHERS) && addCodeToBuf(&buffer, "LF@", T_OTHERS) && addCodeToBuf(&buffer, storedID, T_STRING_FROM_PARSER));
}


bool writeStandFuncGen()
{
    return addCodeToBuf(&buffer, "\nPOPS LF@$tmp$\nWRITE LF@$tmp$", T_OTHERS);
}


bool readstrStandFuncGen() {
    return (addCodeToBuf(&buffer, "\nREAD LF@$tmp$ string", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool readi32StandFuncGen() {
    return (addCodeToBuf(&buffer, "\nREAD LF@$tmp$ int", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool readf64StandFuncGen() {
    return (addCodeToBuf(&buffer, "\nREAD LF@$tmp$ float", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


// bool stringStandFuncGen()
// {
//     //TODO ?
// }


bool lengthStandFuncGen()
{
    return (addCodeToBuf(&buffer, "\nPOPS LF@$str_strlen$\nSTRLEN LF@$tmp$ LF@$str_strlen$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool concatStandFuncGen()
{
    return (addCodeToBuf(&buffer, "\nPOPS LF@$concat_string2$\nPOPS LF@$concat_string1$\nCONCAT LF@$tmp$ LF@$concat_string1$ LF@$concat_string2$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool ordStandFuncGen()
{
  return (addCodeToBuf(&buffer, "\nPOPS LF@$stri2int_int$\nPOPS LF@$stri2int_string$\nSTRI2INT LF@$tmp$ LF@$stri2int_string$ LF@$stri2int_int$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool chrStandFuncGen()
{
    return (addCodeToBuf(&buffer, "\nPOPS LF@$tmp_num$\nINT2CHAR LF@$tmp$ LF@$tmp_num$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool i2fStandFuncGen()
{
    return (addCodeToBuf(&buffer, "\nPOPS LF@$tmp_num$\nINT2FLOAT LF@$tmp$ LF@$tmp_num$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool f2iStandFuncGen()
{
    return (addCodeToBuf(&buffer, "\nPOPS LF@$tmp_num$\nFLOAT2INT LF@$tmp$ LF@$tmp_num$", T_OTHERS) && pushOnStackGen("$tmp$", T_ID));
}


bool substringStandFuncGen()
{
    return addCodeToBuf(&buffer, "\nCREATEFRAME\nDEFVAR TF@string\nDEFVAR TF@startChar\nDEFVAR TF@endChar\nPOPS TF@endChar\nPOPS TF@startChar\nPOPS TF@string\nCALL $$ifj_substring$$\nPUSHS TF@$$$retval", T_OTHERS);
}


bool substringGen()
{
    return (addCodeToBuf(&buffer, "\nLABEL $$ifj_substring$$\nPUSHFRAME\nDEFVAR LF@$$$retval\nMOVE LF@$$$retval string@\nPUSHS bool@true\nPUSHS int@0\nPUSHS LF@startChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS int@0\nPUSHS LF@endChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@startChar\nPUSHS LF@endChar\nGTS\nJUMPIFEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@startChar\nDEFVAR LF@length\nSTRLEN LF@length LF@string\nPUSHS LF@length\nLTS\nJUMPIFNEQS $$notOk$$\nPUSHS bool@true\nPUSHS LF@endChar\nPUSHS LF@length\nGTS\nJUMPIFEQS $$notOk$$\nDEFVAR LF@tmp\nJUMP $$startCopy$$\nLABEL $$startWhile$$\nADD LF@startChar LF@startChar int@1\nJUMPIFEQ $$finishWhile$$ LF@startChar LF@endChar\nLABEL $$startCopy$$\nGETCHAR LF@tmp LF@string LF@startChar\nCONCAT LF@$$$retval LF@$$$retval LF@tmp\nJUMP $$startWhile$$\nJUMP $$end$$\nLABEL $$notOk$$\nMOVE LF@$$$retval nil@nil\nLABEL $$finishWhile$$\nPOPFRAME\nRETURN", T_OTHERS));
}


bool strcmpFuncGen()
{
    return addCodeToBuf(&buffer, "\nCREATEFRAME\nDEFVAR TF@string1\nDEFVAR TF@string2\nPOPS TF@string2\nPOPS TF@string1\nCALL $$ifj_strcmp$$", T_OTHERS);
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
        if (t == T_STRING_TYPE || t == T_STRING_TYPE_EMPTY) {
            char *str = replace_special_characters(param);
            return (addCodeToBuf(&buffer, "string@", T_OTHERS) &&
                    addCodeToBuf(&buffer, str, T_STRING_FROM_PARSER));
        }
        char *storedparam = storeChar(param);
        if (storedparam == NULL)
            return false;
        if (t == T_ID)
        {
            return (addCodeToBuf(&buffer, "LF@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_I32_ID || t == T_I32_VAR)
        {
            return (addCodeToBuf(&buffer, "int@", T_OTHERS) &&
                    addCodeToBuf(&buffer, storedparam, T_STRING_FROM_PARSER));
        }
        else if (t == T_F64_ID || t == T_F64_VAR)
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
    if (strcmp(ID, "_") == 0)   
    {
        return (addCodeToBuf(&buffer, "\nPOPS LF@$tmp$", T_OTHERS));
    }
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
                    addCodeToBuf(&buffer, "\nJUMPIFNEQS $$if$", T_OTHERS) &&
                    addCodeToBuf(&buffer, (void *)&ifCounter, T_INT) &&
                    addCodeToBuf(&buffer, "$else", T_OTHERS));
        }
    }
    return false;
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
    return false;
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
    return false;
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
    if (addCodeToBuf(&buffer, "\nCREATEFRAME\nDEFVAR TF@$tmp$\nDEFVAR TF@$str_strlen$\nDEFVAR TF@$concat_string1$\nDEFVAR TF@$concat_string2$\nDEFVAR TF@$stri2int_string$\nDEFVAR TF@$stri2int_int$\nDEFVAR TF@$tmp_num$", T_OTHERS))
    {
        for (int i = 1; i <= paramsCount; i++)
        {
            int i = paramsCount;
            if (addCodeToBuf(&buffer, "\nDEFVAR TF@$$$param", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *)&i, T_INT) &&
                addCodeToBuf(&buffer, "\nPOPS TF@$$$param", T_OTHERS) &&
                addCodeToBuf(&buffer, (void *)&i, T_INT))
            {
                continue;
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
    return false;
}


bool funcStartGen(char *name, ParamList *l)
{
    char *storedname = storeChar(name);
    if (storedname == NULL)
        return false;
    if (addCodeToBuf(&buffer, "\nLABEL $$$", T_OTHERS) &&
        addCodeToBuf(&buffer, storedname, T_STRING_FROM_PARSER) &&
        addCodeToBuf(&buffer, "\nPUSHFRAME", T_OTHERS))
    {
        l->activeElement = l->firstElement;
        int paramCount = 0;
        while (l->activeElement != NULL) {
            char *param = storeChar(l->activeElement->name);
            paramCount++;
            if (addCodeToBuf(&buffer, "\nDEFVAR LF@", T_OTHERS) && addCodeToBuf(&buffer, param, T_OTHERS) && addCodeToBuf(&buffer, "\nMOVE LF@", T_OTHERS) &&
                addCodeToBuf(&buffer, param, T_STRING_FROM_PARSER) && addCodeToBuf(&buffer, " LF@$$$param", T_OTHERS) && addCodeToBuf(&buffer, (void *)&paramCount, T_INT)) {
                    List_Next(l);
                    continue;
            } else {
                    return false;
            }
        }
        return true;
    }
    return true;
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

        switch (input[i+1])
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

bool returnMainGen() {
    return addCodeToBuf(&buffer, "\nEXIT int@0", T_OTHERS);
}

main () {
    
}
