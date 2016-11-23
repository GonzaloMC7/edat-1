#include "operation.h"

/* COUNT OPERATION: operation whose result is the number of results of the source operation */

typedef struct {
    /* to be implemented */
} operation_count_args_t;

void
operation_count_reset(void* vargs) {
    /* to be implemented */
}

int operation_count_next(void* vargs) {
    /* to be implemented */
}

void* operation_count_get(int col, void* vargs) {
    /* to be implemented */
}

void operation_count_close(void* vargs) {
    /* to be implemented */
}

operation_t*
operation_count_create(operation_t* suboperation) {
    /* to be implemented */
}

