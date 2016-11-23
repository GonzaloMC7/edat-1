#include <stdlib.h>
#include "condition.h"
#include "../operation.h"

/* OR BETWEEN CONDITIONS */

typedef struct {
    condition_t* condition1;
    condition_t* condition2;
} condition_or_args_t;

int
condition_or_evaluation(operation_t* operation, void* vargs) {
    condition_or_args_t* args = vargs;

    return condition_evaluation(args->condition1, operation) || condition_evaluation(args->condition2, operation);
}

void
condition_or_close(void* vargs) {
    condition_or_args_t* args = vargs;

    condition_close(args->condition1);
    condition_close(args->condition2);
    free(args);
}

condition_t*
condition_or_create(condition_t* condition1, condition_t* condition2) {
    condition_t* condition;
    condition_or_args_t* args;

    args = malloc(sizeof(condition_or_args_t));
    args->condition1 = condition1;
    args->condition2 = condition2;

    condition = malloc(sizeof(condition_t));
    condition->evaluation = condition_or_evaluation;
    condition->close = condition_or_close;
    condition->args = args;

    return condition;
}

