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
#include <stdbool.h>

#define ERROR_TODO -1

#ifdef USE_STDIN
    #define SOURCE stdin
#else
    #define SOURCE source_file
#endif

enum automat_state change_automat_state (char c){

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
        return S_OPEN_PARANTHESES;
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
    default:
        return S_ERROR;
        break;
    }
};

// Struktura pro mapování klíčových slov na jejich typy
typedef struct {
    const char* keyword;
    TokenType token_type;
} KeywordTokenPair;

// Pole klíčových slov s odpovídajícími typy tokenů
KeywordTokenPair keyword_tokens[] = {
    {"const", T_CONST},
    {"else", T_ELSE},
    {"fn", T_FN},
    {"if", T_IF},
    {"i32", T_I32_ID},
    {"f64", T_F64_ID},
    {"null", T_NULL},
    {"pub", T_PUB},
    {"return", T_RETURN},
    {"u8", T_U8_ID},
    {"var", T_VAR},
    {"void", T_VOID},
    {"while", T_WHILE},

    {"?i32", T_I32_NULLABLE},
    {"?f64", T_F64_NULLABLE},
    {"?[]u", T_U8_NULLABLE}
};

TokenType isKeyWord(const char* word) {
    
    size_t keywords_count = sizeof(keyword_tokens) / sizeof(keyword_tokens[0]);
    for (size_t i = 0; i < keywords_count; ++i) {
        if (strcmp(word, keyword_tokens[i].keyword) == 0) {
            return keyword_tokens[i].token_type;
        }
    }
    // Pokud slovo není v seznamu klíčových slov nalezeno, vrátí identifikátor funkce ????????
    return T_ID;  // или другой тип для обычных слов
}



FILE *source_file; // глобальная переменная для хранения файла

void scanner_init(FILE *source) {
    source_file = source;
}


Token scanner_get_next_token(){
    // whate state in the state_machine?
    // в зависимости от состояния -> 

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
    

    enum automat_state STATE = change_automat_state(c);

    /*          PROCESS PROGRAM              */ // 
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
            STATE = change_automat_state(c);
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
        /* ) */
        case S_OPEN_PARANTHESES:
            newToken.type = T_OPEN_PARENTHESES;
            return newToken;
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
        /* * */
        case S_MUL:
            newToken.type = T_MUL;
            return newToken;
        /* / */
        case S_SLASH:
            newToken.type = T_DIV;
            STATE = S_LINE_COMMENT;
            break;
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
                        load_string(&newToken, '\n', &init_count);  // Znak nového řádku
                        break;
                    case 'r':
                        load_string(&newToken, '\r', &init_count);  // Návrat na začátek řádku
                        break;
                    case 't':
                        load_string(&newToken, '\t', &init_count);  // Tabulace
                        break;
                    case '\\':
                        load_string(&newToken, '\\', &init_count);  // Escape sekvence pro znak '\'
                        break;
                    case '"':
                        load_string(&newToken, '"', &init_count);   // Escape sekvence pro uvozovky
                        break;
                    case 'x': {
                        // Zpracování escape sekvence \xdd (šestnáctkový kód)
                        char hex[3] = {0};
                        hex[0] = getchar();  // Čteme první znak
                        hex[1] = getchar();  // Čteme druhý znak

                        if (isxdigit(hex[0]) && isxdigit(hex[1])) {
                            // Převedeme šestnáctkovou hodnotu na znak
                            int value = strtol(hex, NULL, 16);
                            load_string(&newToken, (char)value, &init_count);
                        } else {
                            // Pokud znaky po \x nejsou platné šestnáctkové znaky
                            //error_handle(1);  // Chyba 1
                            exit(ERROR_TODO);
                        }
                        break;
                    }
                    default:
                        //error_handle(1);  // Pokud znak po '\' není platná escape sekvence
                        exit(ERROR_TODO);
                }
                escapeSequence = false;  // Resetování flagu escape sekvence
            } 
            else if (c == '\\') {  // Narazili jsme na escape sekvenci nebo pokračování víceřádkového řetězce
                char nextChar = getchar();  // Čteme další znak
                if (nextChar == '\n') {  // Víceřádkové pokračování řetězce
                    // Přeskakujeme bílé znaky na dalším řádku
                    char skipSpace;
                    do {
                        skipSpace = getchar();
                    } while (isspace(skipSpace));  // Přeskakujeme mezery

                    if (skipSpace == '\\') {
                        // Pokud řetězec skutečně pokračuje, přidáme '\n' do řetězce
                        load_string(&newToken, '\n', &init_count);
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
                //error_handle(1);  // Chyba: není dovoleno přenášet řetězec na nový řádek
                exit(ERROR_TODO);
            }
            else {
                load_string(&newToken, c, &init_count);  // Standardní načítání znaku do řetězce
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
            load_letter(&newToken, c, &init_count);
            break;

        
            
        default:
            break;
        }
        

        
        c = fgetc(SOURCE);
        //printf("Read character: %c\n", c);  
    }
}

void load_string(Token* token, char c, unsigned long *init_count){
    /* INIT BUFFER*/
    if (!(*init_count)){
        token->data.u8 = dynamicBuffer_INIT();
        if (token->data.u8 == NULL){
            //error_handle();
            exit(ERROR_TODO);
            return;
        }
        *init_count = 1;
    }
    /* PROCESS skip first '"' */
    if (c != '"' || token->data.u8->size > 0){
        bool err = bufferAddChar(token->data.u8, c);
        if (err == false){
            //error_handle();
            exit(ERROR_TODO);
            return;
        }
    }
    token->type = T_ERROR;
    

}



void load_letter(Token* token, char c, unsigned long *init_count){
    /* INIT BUFFER*/
    if (!(*init_count)){
        token->data.u8 = dynamicBuffer_INIT();
        if (token->data.u8 == NULL){
            //error_handle();
            exit(ERROR_TODO);
            return;
        }
        *init_count = 1;
    }
    /* PROCESS */
    bool err = bufferAddChar(token->data.u8, c);
    if (err == false){
        //error_handle();
        exit(ERROR_TODO);
        return;
    }
}
