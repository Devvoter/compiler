/**
 * @file scanner.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-09-30
 * 
 */


#include "scanner.h"
#include "buffer.h"
#include "error.h"


#ifdef USE_FILE
    #define SOURCE source_file
#else
    #define SOURCE stdin
#endif

enum automat_state changeAutomatState (char c){

    if (isalpha(c) || (c == '_')){
        return S_LETTER;
    }
    else if (isdigit(c)){
        return S_INT_NUM;
    }
    else if(isspace(c)){
        return S_START;
    }

    switch (c)
    {
    case ':':
        return S_COLON;
        break;
    case '.':
        return S_DOT;
        break;
    case ',':
        return S_COMMA;
        break;
    case ';':
        return S_SEMICOLON;
        break;
    case '}':
        return S_CLOSE_BRACKET;
        break;
    case '{':
        return S_OPEN_BRACKET;
        break;
    case ')':
        return S_CLOSE_PARENTHESES;
        break;
    case '(':
        return S_OPEN_PARENTHESES;
        break;
    case '=':
        return S_ASSIGN;
        break;
    case '>':
        return S_GREATER_THAN;
        break;
    case '<':
        return S_LESS_THAN;
        break;
    case '*':
        return S_MUL;
        break;
    case '/':
        return S_SLASH;
        break;
    case '+':
        return S_ADD;
        break;
    case '-':
        return S_SUB;
        break;
    case '"':
        return S_QUOTE;
        break;
    case '?':
        return S_QUESTIONER;
        break;
    case '!':
        return S_EXCLA;
        break;
    case '@':
        return S_AT;
    case '|':
        return S_VERTICAL_BAR;
    case '[':
        return S_SQUQRE_BRACKET_OPEN;
    default:
        return S_ERROR;
        break;
    }
};

void isNullType (Token* newToken){
    if (((strcmp(newToken->data.u8->data, "?[]u8")) == 0)) newToken->type = T_U8_NULLABLE;
    else if (((strcmp(newToken->data.u8->data, "?i32")) == 0)) newToken->type = T_I32_NULLABLE;
    else if (((strcmp(newToken->data.u8->data, "[]u8")) == 0)) newToken->type = T_U8_ID;
    else if (((strcmp(newToken->data.u8->data, "?f64")) == 0)) newToken->type = T_F64_NULLABLE;
    else {
        newToken->type = T_ERROR;
        exitWithError(newToken, ERR_LEXICAL_ANALYSIS);
    }


}


// Pole klíčových slov s odpovídajícími typy tokenů
KeywordTokenPair keyword_tokens[] = {
    {"fn",     T_FN},
    {"if",     T_IF},
    {"pub",    T_PUB},
    {"var",    T_VAR},
    {"ifj",    T_IFJ},
    {"else",   T_ELSE},
    {"null",   T_NULL},
    {"void",   T_VOID},
    {"const",  T_CONST},
    {"while",  T_WHILE},
    {"return", T_RETURN},
    {"i32",    T_I32_ID},
    {"f64",    T_F64_ID},
    {"import", T_IMPORT}
};

TokenType isKeyWord(const char* word) {
    
    size_t keywords_count = sizeof(keyword_tokens) / sizeof(keyword_tokens[0]);
    for (size_t i = 0; i < keywords_count; ++i) {
        if (strcmp(word, keyword_tokens[i].keyword) == 0) {
            return keyword_tokens[i].token_type;
        }
    }
    // Pokud slovo není v seznamu klíčových slov nalezeno, vrátí identifikátor
    return T_ID;
}

FILE *source_file; // globální proměna pro soubor

void fileInit(FILE *source) {
    source_file = source;
}


void loadSymbol(Token* token, char c, unsigned long *init_count){
    
    //init buffer
    if (!(*init_count)){
        token->data.u8 = bufferInit();
        if (token->data.u8 == NULL){
            exitWithError(NULL, ERR_INTERNAL_COMPILER);
            return;
        }
        *init_count = 1;
    }

    bool err = bufferAddChar(token->data.u8, c);
    if (err == false){
        exitWithError(NULL, ERR_INTERNAL_COMPILER);
        return;
    }
}


void stringToNum(Token* token) {
    char* err;

    long tmp_int;
    double tmp_double;

    // Pokus o převod řetězce na celé číslo
    tmp_int = strtol(token->data.u8->data, &err, 10);

    // Pokud se řetězec úspěšně převedl na celé číslo
    if (*err == '\0') {
        token->type = T_I32_VAR;
        bufferFree(token->data.u8);  // Uvolnění řetězce
        token->data.i32 = tmp_int;
        return;
    }

    // Pokud převod na int selhal, zkusíme převést na double
    tmp_double = strtod(token->data.u8->data, &err);

    // Zkontrolujeme úspěšnost převodu na double
    if (*err == '\0') {
        token->type = T_F64_VAR;
        bufferFree(token->data.u8);  // Uvolnění řetězce
        token->data.f64 = tmp_double;
        return;
    }

    // Pokud se nepodařilo rozpoznat jako celé nebo desetinné číslo
    bufferFree(token->data.u8);
    token->type = T_ERROR;
}


Token getNextToken(){

    Token newToken;
    newToken.type = T_UNKNOW;

    static unsigned long init_count;
    static unsigned long line_count;
    newToken.line = line_count;

    int c = fgetc(SOURCE);
    //printf("Read character: %c\n", c);  
    bool escapeSequence = false;

    if (c == EOF){
        newToken.type = T_EOF;
        return newToken;
    }
    
    enum automat_state STATE = changeAutomatState(c);

    while (c != EOF)
    {
        switch (STATE)
        {
        /* white space */
        case S_START:
            if (c == '\n')
            {
                line_count++;
                newToken.line = line_count;
            }
            STATE = changeAutomatState(c);
            if (STATE != S_START)
            {
                ungetc(c, SOURCE);
            }
            break;
        /* : */
        case S_COLON:
            newToken.type = T_COLON;
            return newToken;
        /* . */
        case S_DOT:
            newToken.type = T_DOT;
            return newToken;
        /* , */
        case S_COMMA:
            newToken.type = T_COMMA;
            return newToken;
        /* ; */
        case S_SEMICOLON:
            newToken.type = T_SEMICOLON;
            return newToken;
        /* } */
        case S_CLOSE_BRACKET:
            newToken.type = T_CLOSE_BRACKET;
            return newToken;
        /* { */
        case S_OPEN_BRACKET:
            newToken.type = T_OPEN_BRACKET;
            return newToken;
        /* ) */
        case S_CLOSE_PARENTHESES:
            newToken.type = T_CLOSE_PARENTHESES;
            return newToken;
        /* ( */
        case S_OPEN_PARENTHESES:
            newToken.type = T_OPEN_PARENTHESES;
            return newToken;
        /* [ */
        case S_SQUQRE_BRACKET_OPEN:
            ungetc(c, SOURCE);
            STATE = S_TYPE_ID;
            break;
        /* = */
        case S_ASSIGN:
            newToken.type = T_ASSIGN;
            STATE = S_EQUALS;
            break;
        /* == */
        case S_EQUALS:
            if (c == '=') newToken.type = T_EQUALS;
            else ungetc(c, SOURCE);
            return newToken;
        /* > */
        case S_GREATER_THAN:
            newToken.type = T_GREATER_THAN;
            STATE = S_GREATER_OR_EQUAL;
            break;
        /* >= */
        case S_GREATER_OR_EQUAL:
            if (c == '=') newToken.type = T_GREATER_OR_EQUAL;
            else ungetc(c, SOURCE);
            return newToken;
        /* < */
        case S_LESS_THAN:
            newToken.type = T_LESS_THAN;
            STATE = S_LESS_OR_EQUAL;
            break;
        /* <= */
        case S_LESS_OR_EQUAL:
            if (c == '=') newToken.type = T_LESS_OR_EQUAL;
            else ungetc(c, SOURCE);
            return newToken;
        /* ! */
        case S_EXCLA: 
            newToken.type = T_ERROR;
            STATE = S_NOT_EQUALS;
            break;
        /* != */
        case S_NOT_EQUALS:
            if (c == '=') newToken.type = T_NOT_EQUALS;
            else ungetc(c,SOURCE);
            return newToken;
        /* @ */
        case S_AT:
            newToken.type = T_AT;
            return newToken;
        case S_VERTICAL_BAR:
            newToken.type = T_VERTICAL_BAR;
            return newToken;
        /* * */
        case S_MUL:
            newToken.type = T_MUL;
            return newToken;
        /* / */
        case S_SLASH:
            newToken.type = T_DIV;
            if ((c = fgetc(SOURCE)) == '/')
            {
                STATE = S_LINE_COMMENT;
                break;
            }
            ungetc(c,SOURCE);
            return newToken;
        /* // */
        case S_LINE_COMMENT:

            if (c == '\n'){
                newToken.type = T_UNKNOW;
                line_count++;
                newToken.line = line_count;
                STATE = S_START;
            }
            break;
        /* + */
        case S_ADD:
            newToken.type = T_ADD;
            return newToken;
        /* - */
        case S_SUB:
            newToken.type = T_SUB;
            return newToken;
        /* "..." */
        case S_QUOTE:
            if (escapeSequence) {
                // Zpracování standardních escape sekvencí
                switch (c) {
                    case 'n':
                        loadSymbol(&newToken, '\n', &init_count);  // Znak nového řádku
                        break;
                    case 'r':
                        loadSymbol(&newToken, '\r', &init_count);  // Návrat na začátek řádku
                        break;
                    case 't':
                        loadSymbol(&newToken, '\t', &init_count);  // Tabulace
                        break;
                    case '\\':
                        loadSymbol(&newToken, '\\', &init_count);  // Escape sekvence pro znak '\'
                        break;
                    case '"':
                        loadSymbol(&newToken, '"', &init_count);   // Escape sekvence pro uvozovky
                        break;
                    case 'x':
                        char nextChar = fgetc(SOURCE);
                        if (nextChar == '{') {
                            // Začínáme číst šestnáctkové číslice
                            char hex[3] = {0};
                            int i = 0;
                            while (i < 2) {  // Omezujeme délku na dva znaky
                                char hexChar = fgetc(SOURCE);
                                if (hexChar == '}') {
                                    break;  // Ukončujeme sekvenci při setkání s '}'
                                } else if (isxdigit(hexChar)) {
                                    hex[i++] = hexChar;  // Čteme šestnáctkové číslice
                                } else {
                                    // Neplatný znak v šestnáctkové sekvenci
                                    exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
                                }
                            }

                            if (i == 2) {
                                // Převádíme šestnáctkové číslice na hodnotu znaku
                                int value = strtol(hex, NULL, 16);
                                loadSymbol(&newToken, (char)value, &init_count);
                                fgetc(SOURCE);  // Přeskakujeme znak uzavírající závorky '}'
                            } else {
                                // Nedostatek šestnáctkových číslic nebo nesprávný formát
                                exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
                            }
                        } else {
                            // Pokud jsme nenarazili na '{', jde o chybu
                            exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
                        }
                        break;
                    default:
                        // Pokud znak po '\' není platná escape sekvence
                        exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
                }
                escapeSequence = false;  // Resetování flagu escape sekvence
            } 
            else if (c == '\\') {  // Narazili jsme na escape sekvenci nebo pokračování víceřádkového řetězce
                char nextChar = fgetc(SOURCE);  // Čteme další znak
                if (nextChar == '\n') {  // Víceřádkové pokračování řetězce
                    // Přeskakujeme bílé znaky na dalším řádku
                    char skipSpace;
                    do {
                        skipSpace = fgetc(SOURCE);
                    } while (isspace(skipSpace));  // Přeskakujeme mezery

                    if (skipSpace == '\\') {
                        // Pokud řetězec skutečně pokračuje, přidáme '\n' do řetězce
                        loadSymbol(&newToken, '\n', &init_count);
                        continue;  // Pokračujeme na další znak
                    } else {
                        // Pokud řetězec nepokračuje, vrátíme znak zpět do vstupu
                        ungetc(skipSpace, SOURCE);
                    }
                } else {
                    // Pokud to není víceřádkový řetězec, ale symbol '\', aktivujeme escape sekvenci
                    ungetc(nextChar, SOURCE);  // Vrátíme znak zpět do vstupu
                    escapeSequence = true;    // Aktivujeme escape sekvenci
                }
            }
            else if (c == '"' && (init_count)) {  // Narazili jsme na uzavírací uvozovku
                newToken.data.u8->data[newToken.data.u8->size] = '\0';  // Uzavíráme řetězec
                init_count = 0;
                newToken.type = T_STRING_TYPE;
                return newToken;
            }
            else if (c == '\n') {
                // Chyba: není dovoleno přenášet řetězec na nový řádek
                exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
            }
            else {
                if (c == '"') break; // skipujeme první "
                loadSymbol(&newToken, c, &init_count);  // Standardní načítání znaku do řetězce
            }
            break;
        /* _a-zA-Z */
        case S_LETTER:

            if ((!isalpha(c)) && (!isdigit(c)) && (c != '_')){
                /* check for reserved words */
                newToken.type = isKeyWord(newToken.data.u8->data);
                ungetc(c, SOURCE);
                init_count = 0;
                return newToken;
            }
            //newToken.type = T_ID;
            loadSymbol(&newToken, c, &init_count);
            break;
        /* ? */
        case S_QUESTIONER:
            STATE = S_TYPE_ID;
            ungetc(c, SOURCE);
            newToken.type = T_ERROR;
            break;
        /* ?i32 | ?[]u8 | ?f64 */
        case S_TYPE_ID:
            if ((isalpha(c)) || (isdigit(c)) || (c == '?') || (c == '[') || (c == ']')) loadSymbol(&newToken, c, &init_count);
            else{
                ungetc(c, SOURCE);
                init_count = 0;
                isNullType(&newToken);
                return newToken;
            }
            break;
        /* 0-9 */
        case S_INT_NUM:
            if (isdigit(c)){
                loadSymbol(&newToken, c, &init_count);
            }
            else if (c == '.'){
                loadSymbol(&newToken, c, &init_count);
                STATE = S_FLOAT_NUM;
            }
            else if ((c == 'e') || (c == 'E')){
                loadSymbol(&newToken, c, &init_count);
                STATE = S_EXP_NUM;
            }
            else {
                stringToNum(&newToken);
                ungetc(c, SOURCE);
                init_count = 0;
                return newToken;
            }
            break;
        case S_FLOAT_NUM:
            if (isdigit(c)){
                loadSymbol(&newToken, c, &init_count);
            }
            else if ((c == 'e') || (c == 'E')){
                loadSymbol(&newToken, c, &init_count);
                STATE = S_EXP_NUM;
            }
            else {
                stringToNum(&newToken);
                ungetc(c, SOURCE);
                init_count = 0;
                return newToken;
            }
            break;
        case S_EXP_NUM:
            if (isdigit(c)){
                loadSymbol(&newToken, c, &init_count);
                STATE = S_FLOAT_NUM;
            }
            else if ((c == '+') || (c == '-')){
                loadSymbol(&newToken, c, &init_count);
                STATE = S_FLOAT_NUM;
            }
            else {
                init_count = 0;
                newToken.type = T_ERROR;
                return newToken;
            }
            break;
        
            
        default:
            //printf("bread pit\n");
            newToken.type = T_UNKNOW;
            return newToken;

        }
        
        c = fgetc(SOURCE);
        //printf("Read character: %c\n", c);  
    }


    if(STATE == S_INT_NUM || STATE == S_EXP_NUM || STATE == S_FLOAT_NUM) stringToNum(&newToken);
    if (STATE == S_QUOTE) newToken.type = T_ERROR;
    else if(STATE == S_LETTER) newToken.type = isKeyWord(newToken.data.u8->data);
    else if (STATE == S_TYPE_ID) isNullType(&newToken);
    else if (c == EOF) newToken.type = T_EOF;

    return newToken;
}

