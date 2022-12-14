#include "stacklib.h"

#include <stdio.h>
// #define SHOW_PUSH_POP
void initializeStack(Stack *stack) {
    stack->top = -1;
}
bool isEmpty(Stack *stack) {
    if (stack->top == -1)
        return 1;
    else
        return 0;
}

bool isFull(Stack *stack) {
    if (stack->top == MAX_STACK_SIZE)
        return 1;
    else
        return 0;
}

char *peek(Stack *stack) {
    return stack->stack[stack->top];
}

char *pop(Stack *stack) {
    char *data;

    if (!isEmpty(stack)) {
        data = stack->stack[stack->top];
        stack->top = stack->top - 1;
#ifdef SHOW_PUSH_POP
        printf("%s popped from stack.\n", data);
#endif
        return data;
    } else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

void push(char *data, Stack *stack) {
    if (!isFull(stack)) {
        stack->top = stack->top + 1;
        strcpy(stack->stack[stack->top], data);
#ifdef SHOW_PUSH_POP
        printf("%s pushed to stack.\n", data);
#endif
    } else {
        printf("Could not insert data, Stack is full.\n");
    }
}
void inspectStack(Stack *stack) {
    for (int i = stack->top; i >= 0; i--) {
        if (!isEmpty(stack))
            printf("E:: ");
        puts(stack->stack[i]);
    }
}
void reverse(Stack *stack) {
    Stack *tempStack = malloc(1 * sizeof(Stack));
    initializeStack(tempStack);
    while (!isEmpty(stack)) {
        push(pop(stack), tempStack);
    }
    *stack = *tempStack;
    // inspectStack(tempStack);
}
