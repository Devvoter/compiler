#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../../src/stack.h"

typedef struct {
    int id;
    char name[50];
} Person;

int main() {
    // Initialize the stack
    Stack stack;
    S_Init(&stack);
    printf("Stack initialized successfully.\n");

    // Check if the stack is empty
    if (S_IsEmpty(&stack)) {
        printf("Stack is empty.\n");
    } else {
        printf("Stack is not empty.\n");
    }

    // Test with integers
    int value1 = 10, value2 = 20, value3 = 30;
    S_Push(&stack, &value1);
    S_Push(&stack, &value2);
    S_Push(&stack, &value3);
    printf("Pushed integers: %d, %d, %d\n", value1, value2, value3);

    // Pop integers
    while (!S_IsEmpty(&stack)) {
        int *intValue = (int *)S_Top(&stack);
        printf("Popping integer: %d\n", *intValue);
        S_Pop(&stack);
    }

    // Test with strings
    char *string1 = "Hello";
    char *string2 = "World";
    char *string3 = "Stack";
    S_Push(&stack, string1);
    S_Push(&stack, string2);
    S_Push(&stack, string3);
    printf("Pushed strings: %s, %s, %s\n", string1, string2, string3);

    // Pop strings
    while (!S_IsEmpty(&stack)) {
        char *strValue = (char *)S_Top(&stack);
        printf("Popping string: %s\n", strValue);
        S_Pop(&stack);
    }

    // Test with structs
    Person person1 = {1, "Alice"};
    Person person2 = {2, "Bob"};
    Person person3 = {3, "Charlie"};
    S_Push(&stack, &person1);
    S_Push(&stack, &person2);
    S_Push(&stack, &person3);
    printf("Pushed structs: %s, %s, %s\n", person1.name, person2.name, person3.name);

    // Pop structs
    while (!S_IsEmpty(&stack)) {
        Person *person = (Person *)S_Top(&stack);
        printf("Popping struct: {id: %d, name: %s}\n", person->id, person->name);
        S_Pop(&stack);
    }

    // Final check if stack is empty
    if (S_IsEmpty(&stack)) {
        printf("Stack is now empty.\n");
    } else {
        printf("Stack is not empty.\n");
    }

    return 0;
}
