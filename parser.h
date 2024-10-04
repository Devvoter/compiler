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
 * @brief struktura pro reprezentaci AST uzlu
 * 
 */
typedef struct{
    tASTNode* parent;   // < ukazatel na rodice
    tASTNode* left;     // < ukazatel na levy podstrom
    tASTNode* right;    // < ukazatel na pravy podstrom
    Token token;      // < ukazatel na token
} tASTNode;

/**
 * @brief struktura pro reprezentaci AST (Abstract Syntax Tree)
 * 
 */
typedef struct{
    tASTNode* root; // < ukazatel na korenovy uzel
    tASTNode* current; // < ukazatel na aktivní uzel
    tASTNode* last; // < ukazatel na posledni uzel
    int count; // < pocet uzlu
} tAST;

Token getNextToken(); // < funkce pro ziskani dalsiho tokenu od LA