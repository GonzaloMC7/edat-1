#include "operation.h"

/* LIMIT OPERATION: operation whose result is the first N results of another source operation */

typedef struct {
    /* to be implemented */
} operation_limit_args_t;

void
operation_limit_reset(void* vargs) {
    /* to be implemented */
}

int operation_limit_next(void* vargs) {
    /* to be implemented */
}

void* operation_limit_get(int col, void* vargs) {
    /* to be implemented */
}

void operation_limit_close(void* vargs) {
    /* to be implemented */
}

operation_t*
operation_limit_create(operation_t* suboperation, int limit) {
    /* to be implemented */
}


