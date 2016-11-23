#include "operation.h"

/* SELECT OPERATION: scans the results of an operation and select those that fulfill a specified condition */

typedef struct {
    operation_t* suboperation;
    condition_t* condition;
} operation_select_args_t;

void
operation_select_reset(void* vargs) {
    operation_select_args_t* args = vargs;

    operation_reset(args->suboperation);
}

int operation_select_next(void* vargs) {
    int ret;
    operation_select_args_t* args = vargs;
    operation_t* suboperation = args->suboperation;

    /* results from the previous operation not satisfying the condition are ignored */
    while ((ret = operation_next(suboperation)) && !condition_evaluation(args->condition, suboperation));

    return ret;
}

void* operation_select_get(int col, void* vargs) {
    void* value;
    operation_select_args_t* args = vargs;

    value = operation_get(col, args->suboperation);

    return value;
}

void operation_select_close(void* vargs) {
    operation_select_args_t* args = vargs;

    condition_close(args->condition);
    operation_close(args->suboperation);
    free(args);
}

operation_t*
operation_select_create(operation_t* suboperation, condition_t* condition) {
    operation_t* operation;
    operation_select_args_t* args;

    args = malloc(sizeof(operation_select_args_t));
    args->suboperation = suboperation;
    args->condition = condition;

    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_select_reset;
    operation->next = operation_select_next;
    operation->get = operation_select_get;
    operation->close = operation_select_close;
    operation->ncols = suboperation->ncols;
    operation->types = malloc(operation->ncols * sizeof(type_t));
    memcpy(operation->types, suboperation->types, operation->ncols * sizeof(type_t));

    return operation;
}

