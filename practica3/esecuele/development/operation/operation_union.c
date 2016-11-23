
#include "operation.h"

/* UNION OPERATION: concatenation of the results of two operations with the same columns */

typedef struct {
    /* to be implemented */
} operation_union_args_t;

void
operation_union_reset(void* vargs) {
    /* to be implemented */
}

int operation_union_next(void* vargs) {
    /* to be implemented */
}

void* operation_union_get(int col, void* vargs) {
    /* to be implemented */
}

void operation_union_close(void* vargs) {
    /* to be implemented */
}

operation_t*
operation_union_create(operation_t* operation1, operation_t* operation2) {
    /* to be implemented */
}

