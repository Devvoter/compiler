/**
 * @file parser.h
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Hlavickovy soubor s pomocnymi strukturami pro parser
 * 
 * @date 2024-10-04
 * 
 */

#include "scanner.h"
#include "list.h"

typedef struct expressionValue{
    TokenType dataType;
    bool zeroDecimal;
}tExprVal;

/**
 * @brief Funkce pro získání aktuálního tokenu z listu a uložení do globální proměnné
 */
Token getCurrentToken();

/**
 * @brief Funkce pro přídání věstavených funkcí do tabulky symbolu
 */
void addStandardFunctionsToTS();

/**
 * @brief Funkce pro spuštění syntaktické analýzy
 */
void syntax_analysis();

/**
 * @brief Funkce pro parsování prologu
 * 
 * @return návratová hodnota 0 pokud je prolog správný, jinak 1
 */
int parse_prolog();

/**
 * @brief Funkce pro analýzu jednotlivých příkazů
 * 
 * @param[in] token Aktualní token, podle kterého rozhodneme, jaký příkaz analýzujeme teď
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token code(Token token);

/**
 * @brief Funkce pro parsování definice funkce
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_function_definition();

/**
 * @brief Funkce pro parsování definice proměnné
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_variable_definition();

/**
 * @brief Funkce pro parsování bloku příkazů if
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_if();

/**
 * @brief Funkce pro parsování bloku příkazů while
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_while();

/**
 * @brief Funkce pro parsování příkazu return
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_return();

/**
 * @brief Funkce pro parsování přiřazení hodnoty k proměnné nebo volání funkce (obě začínájí z ID)
 * 
 * @return Token pro další zpracování pomocí LL1 analýzy
 */
Token parse_assignment_or_function_call();

/**
 * @brief Funkce pro parsování volání vestavěné funkce
 * 
 * @return Název (ID) zpracováné funkce
 */
char *parse_standard_function_call();

/**
 * @brief Funkce pro parsování argumentů u definice funkce
 * 
 * @param token Token, který bude zpracován
 * @param list Ukazatel na seznam parametrů pro generování kódu
 */
void arguments(Token token, ParamList *list);

/**
 * @brief Funkce pro zpracování výrazů
 * 
 * @return Struktura, zahrnujíćí datový typ zpracováného výrazů a jeho hodnotu, pokud je výraz konstantní
 */
tExprVal expression();

/**
 * @brief Funkce pro parsování argumentů volání funkce uvnitř výrazu
 */
void parse_function_call();