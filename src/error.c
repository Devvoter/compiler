/**
 * @file error.c
 * @author Denys Pylypenko (xpylypd00)
 * @brief
 * 
 * @date 2024-10-21
 * 
 */


#include "error.h"
#include "generator.h"


void exitWithError(Token *token, int code){
    
    switch (code)
    {
    case ERR_LEXICAL_ANALYSIS:
        fprintf(stderr, "ERROR: chyba v programu v rámci lexikální analýzy! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SYNTAX_ANALYSIS:
        fprintf(stderr, "ERROR: chyba v programu v rámci syntaktické analýzy! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_UNDEFINED_FUNC_VAR:
        fprintf(stderr, "ERROR: sémantická chyba - nedefinovaná funkce či proměnná! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_INVALID_FUNC_PARAMS:
        fprintf(stderr, "ERROR: sémantická chyba - špatný počet/typ parametrů nebo návratové hodnoty! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_REDEFINITION:
        fprintf(stderr, "ERROR: sémantická chyba - redefinice proměnné nebo funkce! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_RETURN_EXPRESSION:
        fprintf(stderr, "ERROR: sémantická chyba - chybějící/přebývající výraz v návratu z funkce! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_TYPE_COMPATIBILITY:
        fprintf(stderr, "ERROR: sémantická chyba typové kompatibility! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_TYPE_DERIVATION:
        fprintf(stderr, "ERROR: sémantická chyba odvození typu! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_UNUSED_VAR:
        fprintf(stderr, "ERROR: sémantická chyba - nevyužitá proměnná! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_SEM_OTHER:
        fprintf(stderr, "ERROR: ostatní sémantické chyby! Řádek: %ld.\n",token->line+1);
        break;
    case ERR_INTERNAL_COMPILER:

        if (token == NULL) fprintf(stderr, "interní chyba překladače!\n");
        else fprintf(stderr, "ERROR: interní chyba překladače! Řádek: %ld.\n",token->line+1);
        break;
    default:
        break;
    }
    disposeGen(false);
    exit(code);
}