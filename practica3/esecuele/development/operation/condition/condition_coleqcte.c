#include <stdlib.h>
#include "condition.h"
#include "../operation.h"

/* COLUMN EQUALITY TO CONSTANT (x = 3 in SQL)*/

typedef struct {
    int ncol;
    void* constant;
} condition_coleqcte_args_t;

int
condition_coleqcte_evaluation(operation_t* operation, void* vargs) {
    void* value;
    type_t type;
    condition_coleqcte_args_t* args = vargs;

    value = operation_get(args->ncol, operation);
    type = operation_types(operation)[args->ncol];
    
    return value_cmp(type, value, args->constant) == 0;
}

void
condition_coleqcte_close(void* vargs) {
    condition_coleqcte_args_t* args = vargs;

    free(args->constant);
    free(args);
}

condition_t*
condition_coleqcte_create(int ncol, void* constant) {
    condition_t* condition;
    condition_coleqcte_args_t* args;

    args = malloc(sizeof(condition_coleqcte_args_t));
    args->ncol = ncol;
    args->constant = constant;

    condition = malloc(sizeof(condition_t));
    condition->evaluation = condition_coleqcte_evaluation;
    condition->close = condition_coleqcte_close;
    condition->args = args;

    return condition;
}

