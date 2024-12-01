
#ifndef _LIST_H_
#define _LIST_H_

/** 
 * @brief Struktura prvku jednosměrně vázaného seznamu obsahující jméno a datový typ parametru
 */
typedef struct ParamListElement {
    int type;
    char *name;
    struct ParamListElement *nextElement;
} *ParamListElementPtr;

/** 
 * @brief Struktura jednosměrně vázaného seznamu pro ukládání parametrů definováné funkce
 */
typedef struct {
	ParamListElementPtr firstElement;
    ParamListElementPtr activeElement;
	int currentLength;
} ParamList;

/**
 * @brief Inicializace nově vytvořeného seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_Init(ParamList *list);

/**
 * @brief Smazání a uvolnění všech prvků seznamů
 * 
 * @param list Ukazatel na seznam
 */
void List_Dispose(ParamList *list );

/**
 * @brief Vkládání prvku za začatek seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
void List_InsertFirst(ParamList *list, int type, char *name);

/**
 * @brief Nastaví aktivní prvek na první prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_First(ParamList *list);

/**
 * @brief Vrací typ a jméno prvního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Ukazatel na proměnnou kam vkládáme typ prvku
 * @param name Ukazatel na proměnnou kam vkládáme název (ID) prvku
 */
void List_GetFirst(ParamList *list, int *type, char **name);

/**
 * @brief Mazání prvního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_DeleteFirst(ParamList *list);

/**
 * @brief Mazání prvku za aktivním prvkem v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_DeleteAfter(ParamList *list);

/**
 * @brief Vkládání prvku za aktivní prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
void List_InsertAfter(ParamList *list, int type, char *name);

/**
 * @brief Přesun aktivity na další prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_Next(ParamList *list);

/**
 * @brief Vrací typ a jméno aktivního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Ukazatel na proměnnou kam vkládáme typ prvku
 * @param name Ukazatel na proměnnou kam vkládáme název (ID) prvku
 */
void List_GetValue( ParamList *list, int *type, char **name);

/**
 * @brief Nastaví typ a jméno aktivního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param data Nový datový typ
 * @param name Nový název (ID)
 */
void List_SetValue( ParamList *list, int data, char *name);

/**
 * @brief Vkládání prvku za aktivní prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
int List_IsActive(ParamList *list);

#endif //_LIST_H_