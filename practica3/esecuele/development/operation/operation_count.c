#include "operation.h"

/* COUNT OPERATION: operation whose result is the number of results of the source operation */

typedef struct {
    operation_t* suboperation;
    int count;
} operation_count_args_t;

void
operation_count_reset(void* vargs) {
    operation_count_args_t* args = vargs;
    args->count = -1;
    operation_reset(args->suboperation);

    return;
}

int operation_count_next(void* vargs) {
    operation_count_args_t* args = vargs;
    operation_t* suboperation = args->suboperation;

  if(args->count != -1) return 0;

	else{
        args->count=0;
		    while(operation_next(suboperation)){
		        args->count++;
	   }
	}

    return 1;
}

void* operation_count_get(int col, void* vargs) {
    operation_count_args_t* args = vargs;

    return &(args->count);
}

void operation_count_close(void* vargs) {
    operation_count_args_t* args = vargs;

    operation_close(args->suboperation);
    free(args);
    return;
}

operation_t*
operation_count_create(operation_t* suboperation) {
    operation_t* operation;
    operation_count_args_t* args;

    args = malloc(sizeof(operation_count_args_t));
    args->suboperation = suboperation;
    args->count = -1;

    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_count_reset;
    operation->next = operation_count_next;
    operation->get = operation_count_get;
    operation->close = operation_count_close;
    operation->ncols = 1;
    operation->types = malloc(sizeof(type_t));
    operation->types[0]=INT;

    return operation;
}
