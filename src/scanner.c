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

    FILE *source_file; // globální proměna pro soubor
    void fileInit(FILE *source) {
        source_file = source;
    }
#else
    #define SOURCE stdin
#endif

static unsigned long line_count = 0;

/*--------------funkce-list--------------*/

void init_list_of_tokens(ListOfTokens *list){
    list->firstToken = NULL;
    list->activeToken = NULL;
    list->currentLength = 0;
    list->tokenToGet = NULL;
}

void free_list_of_tokens(ListOfTokens *list) {
	
	while (list->firstToken != NULL)
	{
		list->activeToken = list->firstToken; 
        list->firstToken = list->firstToken->nextToken; // posuneme first na dalsi prvek

        if (list->activeToken->token.type == T_IMPORT ||
            list->activeToken->token.type == T_IFJ ||
            list->activeToken->token.type == T_CONST ||
            list->activeToken->token.type == T_ELSE  ||
            list->activeToken->token.type == T_FN ||
            list->activeToken->token.type == T_IF ||
            list->activeToken->token.type == T_PUB ||
            list->activeToken->token.type == T_RETURN ||
            list->activeToken->token.type == T_U8_ID ||
            list->activeToken->token.type == T_VAR ||
            list->activeToken->token.type == T_VOID ||
            list->activeToken->token.type == T_WHILE ||
            list->activeToken->token.type == T_ID ||
            list->activeToken->token.type == T_STRING_TYPE)
        {
            free(list->activeToken->token.data.u8);
        }
        
        
        free(list->activeToken); // uvolnujeme prvek
	}
	
	list->firstToken = NULL;
	list->activeToken = NULL;
    list->firstToken = NULL;
	list->currentLength = 0; 
}

void insert_in_list_of_tokens(ListOfTokens *list, Token token){

    ListTokenPtr newElemnt = (ListTokenPtr)malloc(sizeof(struct ListToken));
    if (newElemnt == NULL) exitWithError(NULL, ERR_INTERNAL_COMPILER);

    newElemnt->token = token;
    newElemnt->nextToken = NULL;

    if (list->firstToken == NULL)
    {
        list->firstToken = newElemnt;
        list->activeToken = newElemnt;
    }
    else{
        list->activeToken = list->firstToken;
        while (list->activeToken->nextToken != NULL) list->activeToken = list->activeToken->nextToken;

        list->activeToken->nextToken = newElemnt;
    }

    list->currentLength++;
}

void i_want_to_get_tokens(ListOfTokens *list){
    list->tokenToGet = list->firstToken;
}

Token get_token_from_list(ListOfTokens *list){
    list->activeToken = list->tokenToGet;
    list->tokenToGet = list->tokenToGet->nextToken;
    return list->activeToken->token;
}

/*--------------konec-funkce-list--------------*/


/*--------------pomocne-funkce-automat--------------*/

enum automat_state changeAutomatState (char c){

    if      (isalpha(c) || (c == '_')) return S_LETTER;
    else if (isdigit(c))               return S_INT_NUM;
    else if (isspace(c))               return S_START;


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
        break;
    case '[':
        return S_SQUQRE_BRACKET_OPEN;
        break;
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
        if (strcmp(word, keyword_tokens[i].keyword) == 0) return keyword_tokens[i].token_type;
    }

    // Pokud slovo není v seznamu klíčových slov nalezeno, vrátí identifikátor
    return T_ID;
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
    exitWithError(token, ERR_LEXICAL_ANALYSIS);
}


void escape_Sequence(Token newToken, unsigned long *init_count, char c){
    c = fgetc(SOURCE);
    switch (c) {
        case 'n':
            loadSymbol(&newToken, '\n', init_count);  // Znak nového řádku
            break;
        case 'r':
            loadSymbol(&newToken, '\r', init_count);  // Návrat na začátek řádku
            break;
        case 't':
            loadSymbol(&newToken, '\t', init_count);  // Tabulace
            break;
        case '\\':
            loadSymbol(&newToken, '\\', init_count);  // Escape sekvence pro znak '\'
            break;
        case '"':
            loadSymbol(&newToken, '"', init_count);   // Escape sekvence pro uvozovky
            break;
        case 'x':
            char nextChar = fgetc(SOURCE);
            if (isxdigit(nextChar)) {
                // Pripad bez slozenych zavorek
                char hex[3] = {0};
                hex[0] = nextChar;  // Ukladame prvni znak
                hex[1] = fgetc(SOURCE);  // Cteme druhy znak

                if (isxdigit(hex[1])) {
                    // Pokud jsou oba znaky platne, prevadime je
                    int value = strtol(hex, NULL, 16);
                    loadSymbol(&newToken, (char)value, init_count);
                } 
                else exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);// Pokud druhy znak neni sestnactkovy
            } 
            else exitWithError(&newToken, ERR_LEXICAL_ANALYSIS); // Pokud po 'x' neni '{' a ani sestnactkovy znak

            break;

        default:
            // Pokud znak po '\' není platná escape sekvence
            exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
    }
}
/*--------------konce-pomocne-funkce-automat--------------*/


/*--------------hlavni-funkce-souboru--------------*/

Token getNextToken(ListOfTokens *list){

    Token newToken;
    newToken.type = T_UNKNOW;

    static unsigned long init_count;

    newToken.line = line_count;
    bool start = true;
    
    int c = fgetc(SOURCE);
    //printf("Read character: %c\n", c);  
    

    if (c == EOF){
        newToken.type = T_EOF;
        insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* . */
        case S_DOT:
            newToken.type = T_DOT;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* , */
        case S_COMMA:
            newToken.type = T_COMMA;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* ; */
        case S_SEMICOLON:
            newToken.type = T_SEMICOLON;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* } */
        case S_CLOSE_BRACKET:
            newToken.type = T_CLOSE_BRACKET;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* { */
        case S_OPEN_BRACKET:
            newToken.type = T_OPEN_BRACKET;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* ) */
        case S_CLOSE_PARENTHESES:
            newToken.type = T_CLOSE_PARENTHESES;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* ( */
        case S_OPEN_PARENTHESES:
            newToken.type = T_OPEN_PARENTHESES;
            insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* ! */
        case S_EXCLA: 
            newToken.type = T_ERROR;
            STATE = S_NOT_EQUALS;
            break;
        /* != */
        case S_NOT_EQUALS:
            if (c == '=') newToken.type = T_NOT_EQUALS;
            else {
                ungetc(c,SOURCE);
                exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
            }
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* @ */
        case S_AT:
            newToken.type = T_AT;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        case S_VERTICAL_BAR:
            newToken.type = T_VERTICAL_BAR;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* * */
        case S_MUL:
            newToken.type = T_MUL;
            insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
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
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        /* - */
        case S_SUB:
            newToken.type = T_SUB;
            insert_in_list_of_tokens(list, newToken);
            return newToken;
        
        /* "..." */
        case S_QUOTE:

            if (c == '"' && start) // pokud mame prvni " 
            {
                c = fgetc(SOURCE); // nacteme dalsi znak
                if (c == '"') // mame prazdy retezec ""
                {
                   newToken.type = T_STRING_TYPE_EMPTY;
                   insert_in_list_of_tokens(list, newToken);
                   return newToken;
                }

                // kontrola na viceradkovy retezec:

                int counter_space = 0;  // pocitadlo na mezery pred spetnyma lomitkami na pripad ze neni spetny lomitka a retezec zacina mezerami
                while (isspace(c)){ // pokud mezera
                    c = fgetc(SOURCE); // bereme dalsi znak 
                    counter_space++; // pridame jednicku k poctu mezer
                }

                if (c == '\\') // narazili jsme se na lomitka tak prejdeme do jineho stavu
                {
                    c = fgetc(SOURCE);
                    if (c == '\\')
                    {
                        STATE = S_MULTILINE_STRING;
                        start = false;
                        break;
                    }
                    ungetc(c, SOURCE);
                    escape_Sequence(newToken, &init_count, c);
                }
                else{ // narazili jsme se na nejaky symbol, coz retezec nezacina spetnyma lomitkama
                    for (int i = counter_space; i > 0; i--) loadSymbol(&newToken, ' ', &init_count); // pridame mezery do retezce pokud byly pred symbolem 
                    start = false; // nastavim flag aby jsme uz tady nezasli
                    loadSymbol(&newToken, c, &init_count);
                    // pokracuju v dalsi iterace
                }
                start = false;
                break;
            }

            // Narazili jsme na escape sekvence
            else if (c == '\\') escape_Sequence(newToken, &init_count, c);

            else if (c == '"' && (init_count)) {  // Narazili jsme na uzavírací uvozovku
                newToken.data.u8->data[newToken.data.u8->size] = '\0';  // Uzavíráme řetězec
                init_count = 0;
                newToken.type = T_STRING_TYPE;
                insert_in_list_of_tokens(list, newToken);
                return newToken;
            }
            else if (c == '\n') exitWithError(&newToken, ERR_LEXICAL_ANALYSIS); // Chyba: není dovoleno přenášet řetězec na nový řádek
            else {
                if (c == '"') break;
                loadSymbol(&newToken, c, &init_count);  // Standardní načítání znaku do řetězce
            }
            break;
        /* "\\ text" 
            \\ next text" */
        case S_MULTILINE_STRING:

            if (c == '\n')
            {
                line_count++;
                int counter_space = 0;
                c = fgetc(SOURCE);
                while (isspace(c))
                {
                    c = fgetc(SOURCE);
                    counter_space++;
                }

                if (c == '\\')
                {
                    c = getc(SOURCE);
                    if (c == '\\')
                    {
                        loadSymbol(&newToken, '\n', &init_count);
                        break;
                    }

                    for (int i = counter_space; i > 0; i--) loadSymbol(&newToken, ' ', &init_count);
                    ungetc(c, SOURCE);
                    escape_Sequence(newToken, &init_count, c);
                }
                else{
                    for (int i = counter_space; i > 0; i--) loadSymbol(&newToken, ' ', &init_count);
                    loadSymbol(&newToken, c, &init_count);
                }
            }
            else if (c == '\\')
            {
                escape_Sequence(newToken, &init_count, c);
            }
            else if (c == '"' && (init_count))
            {
                newToken.data.u8->data[newToken.data.u8->size] = '\0';  // Uzavíráme řetězec
                init_count = 0;
                newToken.type = T_STRING_TYPE;
                insert_in_list_of_tokens(list, newToken);
                return newToken;
            }
            else loadSymbol(&newToken, c, &init_count);  // Standardní načítání znaku do řetězce

            break;
        /* _a-zA-Z */
        case S_LETTER:

            if ((!isalpha(c)) && (!isdigit(c)) && (c != '_')){
                /* check for reserved words */
                newToken.type = isKeyWord(newToken.data.u8->data);
                ungetc(c, SOURCE);
                init_count = 0;
                insert_in_list_of_tokens(list, newToken);
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
                insert_in_list_of_tokens(list, newToken);
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
                insert_in_list_of_tokens(list, newToken);
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
                insert_in_list_of_tokens(list, newToken);
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
                exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);                
            }
            break;
        
        case S_ERROR:
            newToken.type = T_ERROR;
            exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
            break;
        default:
            //printf("bread pit\n");
            newToken.type = T_UNKNOW;
            insert_in_list_of_tokens(list, newToken);
            return newToken;

        }
        
        c = fgetc(SOURCE);
        //printf("Read character: %c\n", c);  
    }


    if(STATE == S_INT_NUM || STATE == S_EXP_NUM || STATE == S_FLOAT_NUM) stringToNum(&newToken);
    if (STATE == S_QUOTE) exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
    else if(STATE == S_LETTER) newToken.type = isKeyWord(newToken.data.u8->data);
    else if (STATE == S_TYPE_ID) isNullType(&newToken);
    else if (c == EOF) newToken.type = T_EOF;
    else if(newToken.type == T_UNKNOW || newToken.type == T_ERROR) exitWithError(&newToken, ERR_LEXICAL_ANALYSIS);
    insert_in_list_of_tokens(list, newToken);
    return newToken;
}

/*--------------konec-hlavni-funkce-souboru--------------*/