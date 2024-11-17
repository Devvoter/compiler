/**
 * @file parser.h
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Hlavickovy soubor s pomocnymi strukturami pro parser
 * 
 * @date 2024-10-04
 * 
 */

#include "scanner.h"

/**
 * @brief Funkce pro získání aktuálního tokenu a uložení do globální proměnné
 */
Token getCurrentToken();

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
 * @brief Funkce pro analýzu kódu
 * 
 * @param[in] token Token, který bude zpracován
 * @return Token pro další zpracování
 */
Token code(Token token);

/**
 * @brief Funkce pro parsování definice funkce
 * 
 * @return Token pro další zpracování
 */
Token parse_function_definition();

/**
 * @brief Funkce pro parsování definice proměnné
 * 
 * @return Token pro další zpracování
 */
Token parse_variable_definition();

/**
 * @brief Funkce pro parsování konstrukce if
 * 
 * @return Token pro další zpracování
 */
Token parse_if();

/**
 * @brief Funkce pro parsování konstrukce while
 * 
 * @return Token pro další zpracování
 */
Token parse_while();

/**
 * @brief Funkce pro parsování příkazu return
 * 
 * @return Token pro další zpracování
 */
Token parse_return();

/**
 * @brief Funkce pro parsování přiřazení nebo volání funkce (obě začínájí ID)
 * 
 * @return Token pro další zpracování
 */
Token parse_assignment_or_function_call();

/**
 * @brief Funkce pro parsování volání vestavěné funkce
 * 
 * @return Token pro další zpracování
 */
Token parse_standard_function_call();

/**
 * @brief Funkce pro parsování argumentů funkce
 * 
 * @param[in] token Token, který bude zpracován
 */
void arguments(Token token);

/**
 * @brief Funkce pro parsování výrazů
 */
void expression();

/**
 * @brief Funkce pro parsování volání funkce
 */
void function();