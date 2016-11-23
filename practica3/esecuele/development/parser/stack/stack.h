#include <stdlib.h>

typedef struct stack_ stack_t;

/* AUXILIAR STACK, NO NEED TO EXPLAIN IT, RIGHT?*/

stack_t* stack_create(int max_size);

void stack_push(stack_t* stack, void* elem);

void* stack_pop(stack_t* stack);

void* stack_peek(stack_t* stack);

int stack_size(stack_t* stack);

void stack_free(stack_t* stack);