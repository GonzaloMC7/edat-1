#include <stdlib.h>
#include "condition.h"
#include "../operation.h"

/* ALWAYS TRUE */

int
condition_true_evaluation(operation_t* operation, void* args) {
    (void)(operation);
    (void)(args);
    return 1;
}

void
condition_true_close(void* args) {
    (void)(args);
}

condition_t*
condition_true_create() {
    condition_t* condition;

    condition = malloc(sizeof(condition_t));
    condition->evaluation = condition_true_evaluation;
    condition->close = condition_true_close;
    condition->args = NULL;

    return condition;
}
