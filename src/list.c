#include <stdlib.h>
#include <string.h>
#include "list.h"

void List_Init(ParamList *list) {
    list->firstElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

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

void List_First(ParamList *list) {
    list->activeElement = list->firstElement;
}

void List_GetFirst(ParamList *list, int *type, char **name) {
    if (list->currentLength == 0) {
		exit(99);
		return;
	}
	*type = list->firstElement->type;
    *name = list->firstElement->name;
}

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

void List_Next(ParamList *list) {
    	if(list->activeElement == NULL) {
		return;
	}
	list->activeElement = list->activeElement->nextElement;
}

void List_GetValue( ParamList *list, int *type, char **name) {
    if (list->activeElement == NULL) {
        exit(99);
    }
    *type = list->activeElement->type;
    *name = list->activeElement->name;
}

void List_SetValue( ParamList *list, int data, char *name) {
    if (list->activeElement == NULL) {
        return;
    }
    list->activeElement->type = data;
    free(list->activeElement->name);
    list->activeElement->name = (char *) malloc(strlen(name) + 1);
    strcpy(list->activeElement->name, name);
}

int List_IsActive(ParamList *list) {
    return (list->activeElement != NULL);
}