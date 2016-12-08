#include "operation.h"

/* COUNT OPERATION: operation whose result is the number of results of the source operation */

typedef struct {
    operation_t* suboperation;
} operation_count_args_t;

void
operation_count_reset(void* vargs) {
    operation_count_args_t* args = vargs;

    operation_reset(args->suboperation);

    return;
}

int operation_count_next(void* vargs) {
    int ret;
    operation_count_args_t* args = vargs;

    operation_t* operation = args->suboperation;

    if (!operation_next(operation)) return 0;

    return 1;
}

void* operation_count_get(int col, void* vargs) {
    void* value;
    operation_count_args_t* args = vargs;

    operation_t* operation = args->suboperation;

    if (col < operation->ncols) {
        value = operation_get(col, args->operation);
    }

    return value;
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
    args->operation = suboperation;

    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_count_reset;
    operation->next = operation_count_next;
    operation->get = operation_count_get;
    operation->close = operation_count_close;
    operation->ncols = operation_ncols(suboperation);
    operation->types = malloc(suboperation->ncols * sizeof(type_t));
    memcpy(operation->types, suboperation->types, suboperation->ncols * sizeof(type_t));

    return operation;
}
