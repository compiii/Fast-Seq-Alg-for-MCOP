#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack_T Stack;
struct Stack_T
{
    void *data;
    Stack *next;
};

int isEmptyStack(Stack *s);
Stack *createStack(void);
void push(Stack **s, void *new_value, size_t size);
Stack *pop(Stack *s);
void *peek(Stack *s);

#endif /* STACK_H */
