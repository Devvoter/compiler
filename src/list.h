
#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <string.h>

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
void List_Init(ParamList *list) {
    list->firstElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

/**
 * @brief Smazání a uvolnění všech prvků seznamů
 * 
 * @param list Ukazatel na seznam
 */
void List_Dispose(ParamList *list ) {
    ParamListElementPtr currentElement = list->firstElement;
	while (currentElement != NULL) {
		ParamListElementPtr next = currentElement->nextElement;
		free(currentElement);
		currentElement = next;
		list->currentLength--;
	}
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

/**
 * @brief Vkládání prvku za začatek seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
void List_InsertFirst(ParamList *list, int type, char *name) {
    ParamListElementPtr newFirstElement = (ParamListElementPtr) malloc(sizeof(struct ParamListElement));
	if (newFirstElement == NULL) {
		exit(99);
	}
	newFirstElement->nextElement = list->firstElement;
	newFirstElement->type = type;
    newFirstElement->name = (char *) malloc(strlen(name) + 1);
    strcpy(newFirstElement->name, name);
	list->firstElement = newFirstElement;
	list->currentLength++;
}

/**
 * @brief Nastaví aktivní prvek na první prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_First(ParamList *list) {
    list->activeElement = list->firstElement;
}

/**
 * @brief Vrací typ a jméno prvního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Ukazatel na proměnnou kam vkládáme typ prvku
 * @param name Ukazatel na proměnnou kam vkládáme název (ID) prvku
 */
void List_GetFirst(ParamList *list, int *type, char **name) {
    if (list->currentLength == 0) {
		exit(99);
		return;
	}
	*type = list->firstElement->type;
    *name = list->firstElement->name;
}

/**
 * @brief Mazání prvního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_DeleteFirst(ParamList *list) {
    if (list->currentLength == 0) {
		return;
	}
	if (list->activeElement == list->firstElement) {
		list->activeElement = NULL;
	}
	ParamListElementPtr secondElement = list->firstElement->nextElement;
    free(list->firstElement->name);
	free(list->firstElement);
	list->firstElement = secondElement;
	list->currentLength--;
}

/**
 * @brief Mazání prvku za aktivním prvkem v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_DeleteAfter(ParamList *list) {
    if (list->activeElement == NULL || list->activeElement->nextElement == NULL) {
		return;
	}

	ParamListElementPtr elementToDelete = list->activeElement->nextElement;
    list->activeElement->nextElement = elementToDelete->nextElement;
    free(elementToDelete->name);
    free(elementToDelete);
    list->currentLength--;
}

/**
 * @brief Vkládání prvku za aktivní prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
void List_InsertAfter(ParamList *list, int type, char *name) {
    if (list->activeElement == NULL) {
		return;
	}
	ParamListElementPtr newElement = (ParamListElementPtr) malloc(sizeof(struct ParamListElement));
	if (newElement == NULL) {
		exit(99);
		return;
	}

	newElement->nextElement = list->activeElement->nextElement;
	newElement->type = type;
    newElement->name = (char *) malloc(strlen(name) + 1);
    strcpy(newElement->name, name);
	list->activeElement->nextElement = newElement;
	list->currentLength++;
}

/**
 * @brief Přesun aktivity na další prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 */
void List_Next(ParamList *list) {
    	if(list->activeElement == NULL) {
		return;
	}
	list->activeElement = list->activeElement->nextElement;
}

/**
 * @brief Vrací typ a jméno aktivního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Ukazatel na proměnnou kam vkládáme typ prvku
 * @param name Ukazatel na proměnnou kam vkládáme název (ID) prvku
 */
void List_GetValue( ParamList *list, int *type, char **name) {
    if (list->activeElement == NULL) {
        exit(99);
    }
    *type = list->activeElement->type;
    *name = list->activeElement->name;
}

/**
 * @brief Nastaví typ a jméno aktivního prvku v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param data Nový datový typ
 * @param name Nový název (ID)
 */
void List_SetValue( ParamList *list, int data, char *name) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement->type = data;
    free(list->activeElement->name);
    list->activeElement->name = (char *) malloc(strlen(name) + 1);
    strcpy(list->activeElement->name, name);
}

/**
 * @brief Vkládání prvku za aktivní prvek v seznamu
 * 
 * @param list Ukazatel na seznam
 * @param type Datový typ vkládaného prvku
 * @param name Název (ID) vkládaného prvku
 */
int List_IsActive(ParamList *list) {
    return (list->activeElement != NULL);
}

#endif //_LIST_H_