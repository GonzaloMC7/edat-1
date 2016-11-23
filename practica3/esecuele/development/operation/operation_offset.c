#include "operation.h"

/* OFFSET OPERATION: operation whose result all but the first N results of another source operation */

typedef struct {
    /* to be implemented */
} operation_offset_args_t;

void
operation_offset_reset(void* vargs) {
    /* to be implemented */
}

int operation_offset_next(void* vargs) {
    /* to be implemented */
}

void* operation_offset_get(int col, void* vargs) {
    /* to be implemented */
}

void operation_offset_close(void* vargs) {
    /* to be implemented */
}

operation_t*
operation_offset_create(operation_t* suboperation, int offset) {
    /* to be implemented */
}

