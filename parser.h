/**
 * @file parser.h
 * @author Mariia Sydorenko (xsydorm00)
 * @brief Hlavickovy soubor s pomocnymi strukturami pro parser
 * 
 * @date 2024-10-04
 * 
 */

#include "scanner.h"

Token getCurrentToken();
void unexpected_token(Token token);
void parse();
int parse_prolog();
Token code(Token token);
Token parse_function_definition();
Token parse_variable_definition();
Token parse_if();
Token parse_while();
Token parse_return();
Token parse_assignment_or_function_call();
Token parse_standard_function_call();
void arguments(Token token);
void expression();
void function();