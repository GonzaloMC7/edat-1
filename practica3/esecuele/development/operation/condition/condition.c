#include <stdlib.h>
#include <string.h>
#include "condition.h"
#include "../../type/type.h"
#include "../operation.h"

/* INTERNALS OF THE CONDITION MODULE */

int
condition_evaluation(condition_t* condition, operation_t* operation) {
    return condition->evaluation(operation, condition->args);
}

void
condition_close(condition_t* condition) {
    condition->close(condition->args);
    free(condition);
}
