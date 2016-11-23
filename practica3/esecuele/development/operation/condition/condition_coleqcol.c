#include <stdlib.h>
#include "condition.h"
#include "../operation.h"

/* COLUMN TO COLUMN EQUALITY (x = y IN SQL)*/

typedef struct {
    int ncol1;
    int ncol2;
} condition_coleqcol_args_t;

int
condition_coleqcol_evaluation(operation_t* operation, void* vargs) {
    void* value1;
    void* value2;
    type_t type;
    condition_coleqcol_args_t* args = vargs;

    value1 = operation_get(args->ncol1, operation);
    value2 = operation_get(args->ncol2, operation);
    type = operation_types(operation)[args->ncol1];
    
    return value_cmp(type, value1, value2) == 0;
}

void
condition_coleqcol_close(void* vargs) {
    condition_coleqcol_args_t* args = vargs;

    free(args);
}

condition_t*
condition_coleqcol_create(int ncol1, int ncol2) {
    condition_t* condition;
    condition_coleqcol_args_t* args;

    args = malloc(sizeof(condition_coleqcol_args_t));
    args->ncol1 = ncol1;
    args->ncol2 = ncol2;

    condition = malloc(sizeof(condition_t));
    condition->evaluation = condition_coleqcol_evaluation;
    condition->close = condition_coleqcol_close;
    condition->args = args;

    return condition;
}
