
#include "stack.h"
#include "clogger.h"

int isEmptyStack(Stack *s) { return s == NULL || s->next == NULL; }

Stack *createStack(void)
{
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
    {
        logE("insuffisant memory for allocate stack s\n");
        exit(EXIT_FAILURE);
    }
    s->next = NULL;
    return s;
}

Stack *pop(Stack *s)
{
    if (isEmptyStack(s))
    {
        logE("impossible to pop because the stack is empty\n");
        exit(EXIT_FAILURE);
    }
    Stack *tmp;
    tmp = s;
    s = s->next;
    free(tmp);
    return s;
}

void push(Stack **s, void *new_num, size_t size)
{
    Stack *new_node = createStack();
    new_node->data = malloc(size);
    memcpy(new_node->data, new_num, size);
    //new_node->data = new_num;
    new_node->next = *s;
    *s = new_node;
}

void *peek(Stack *s)
{
    if (isEmptyStack(s))
    {
        logE("impossible to peek because the stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return s->data;
}
