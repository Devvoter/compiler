
#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <string.h>

/** 
 * @brief Struktura prvku jednosměrně vázaného seznamu
 */
typedef struct ListElement {
    int type;
    char *name;
    struct ListElement *nextElement;
} *ListElementPtr;

/** 
 * @brief Struktura jednosměrně vázaného seznamu
 */
typedef struct {
	ListElementPtr firstElement;
    ListElementPtr activeElement;
	int currentLength;
} List;

void List_Init( List * list) {
    list->firstElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

void List_Dispose(List *list ) {
    ListElementPtr currentElement = list->firstElement;
	while (currentElement != NULL) {
		ListElementPtr next = currentElement->nextElement;
		free(currentElement);
		currentElement = next;
		list->currentLength--;
	}
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

void List_InsertFirst(List *list, int type, char *name) {
    ListElementPtr newFirstElement = (ListElementPtr) malloc(sizeof(struct ListElement));
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

void List_First(List *list) {
    list->activeElement = list->firstElement;
}

void List_GetFirst(List *list, int *type, char **name) {
    if (list->currentLength == 0) {
		exit(99);
		return;
	}
	*type = list->firstElement->type;
    *name = list->firstElement->name;
}

void List_DeleteFirst(List *list) {
    if (list->currentLength == 0) {
		return;
	}
	if (list->activeElement == list->firstElement) {
		list->activeElement = NULL;
	}
	ListElementPtr secondElement = list->firstElement->nextElement;
    free(list->firstElement->name);
	free(list->firstElement);
	list->firstElement = secondElement;
	list->currentLength--;
}

void List_DeleteAfter(List *list) {
    if (list->activeElement == NULL || list->activeElement->nextElement == NULL) {
		return;
	}

	ListElementPtr elementToDelete = list->activeElement->nextElement;
    list->activeElement->nextElement = elementToDelete->nextElement;
    free(elementToDelete->name);
    free(elementToDelete);
    list->currentLength--;
}

void List_InsertAfter(List *list, int type, char *name) {
    if (list->activeElement == NULL) {
		return;
	}
	ListElementPtr newElement = (ListElementPtr) malloc(sizeof(struct ListElement));
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

void List_Next(List *list) {
    	if(list->activeElement == NULL) {
		return;
	}
	list->activeElement = list->activeElement->nextElement;
}

void List_GetValue( List *list, int *type, char **name) {
    if (list->activeElement == NULL) {
        exit(99);
    }
    *type = list->activeElement->type;
    *name = list->activeElement->name;
}

void List_SetValue( List *list, int data, char *name) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement->type = data;
    free(list->activeElement->name);
    list->activeElement->name = (char *) malloc(strlen(name) + 1);
    strcpy(list->activeElement->name, name);
}

int List_IsActive(List *list) {
    return (list->activeElement != NULL);
}

#endif //_LIST_H_