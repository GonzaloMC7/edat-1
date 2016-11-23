#include <stdlib.h>
#include "condition.h"
#include "../operation.h"

/* NEGATE ANOTHER CONDITION */

int
condition_not_evaluation(operation_t* operation, void* args) {
    condition_t* condition1 = args;

    return !(condition_evaluation(condition1, operation));
}

void
condition_not_close(void* args) {
    condition_t* condition1 = args;

    condition_close(condition1);
}

condition_t*
condition_not_create(condition_t* condition1) {
    condition_t* condition2;

    condition2 = malloc(sizeof(condition_t));
    condition2->evaluation = condition_not_evaluation;
    condition2->close = condition_not_close;
    condition2->args = condition1;

    return condition2;
}

