#include <stdlib.h>
#include "stack.h"

struct stack_ {
    void** elemns;
    int max_size;
    int size;
};

stack_t* stack_create(int max_size) {
    stack_t* stack;
    
    stack = malloc(sizeof(stack_t));
    stack->elemns = malloc(max_size * sizeof(void*));
    stack->max_size = max_size;
    stack->size = 0;
    
    return stack;
}

void stack_push(stack_t* stack, void* elem) {
    stack->elemns[stack->size] = elem;
    stack->size++;
}

void* stack_pop(stack_t* stack) {
    stack->size--;
    return stack->elemns[stack->size];
}

void* stack_peek(stack_t* stack) {
    return stack->elemns[stack->size - 1];
}

int stack_size(stack_t* stack) {
    return stack->size;
}

void stack_free(stack_t* stack) {
    free(stack->elemns);
    free(stack);
}