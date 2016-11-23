#include "operation.h"

/* PRODUCT OPERATION: returns the result of the cartesian product of two operations */

typedef struct {
    int first_next;
    operation_t* operation1;
    operation_t* operation2;
} operation_product_args_t;

void
operation_product_reset(void* vargs) {
    operation_product_args_t* args = vargs;

    operation_reset(args->operation1);
    operation_reset(args->operation2);
}

int operation_product_next(void* vargs) {
    int ret;
    operation_product_args_t* args = vargs;
    operation_t* operation1 = args->operation1;
    operation_t* operation2 = args->operation2;

    if (args->first_next) {
        /* if it is the first time the next function is called for the operation */
        /* the two first results of both operations (if they exists) are retrieved */
        ret = operation_next(operation1);
        if (!ret) {
            return 0;
        }
        ret = operation_next(operation2);
        if (!ret) {
            return 0;
        }
        args->first_next = 0;
    } else {
        /* otherwise, the result of the left operation is fixed and the next of the right operation is retrieved... */
        ret = operation_next(operation2);
        if (!ret) {
            /* ... unless it has no more results, in which case */
            /* we come back to the first result of the right operation... */
            operation_reset(operation2);
            ret = operation_next(operation2);
            if (!ret) {
                return 0;
            }
            /* ... and retrieve the next (if any) result of the left operation */
            ret = operation_next(operation1);
            if (!ret) {
                return 0;
            }
        }
    }

    return 1;
}

void* operation_product_get(int col, void* vargs) {
    void* value;
    operation_product_args_t* args = vargs;
    operation_t* operation1 = args->operation1;
    operation_t* operation2 = args->operation2;

    if (col < operation1->ncols) {
        /* the requested column belongs to the left operation */
        value = operation_get(col, operation1);
    } else {
        /* the requested column belong to the right operation */
        value = operation_get(col - operation1->ncols, operation2);
    }

    return value;
}

void operation_product_close(void* vargs) {
    operation_product_args_t* args = vargs;

    operation_close(args->operation1);
    operation_close(args->operation2);
    free(args);
}

operation_t*
operation_product_create(operation_t* operation1, operation_t* operation2) {
    operation_t* operation;
    operation_product_args_t* args;

    args = malloc(sizeof(operation_product_args_t));
    args->first_next = 1;
    args->operation1 = operation1;
    args->operation2 = operation2;

    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_product_reset;
    operation->next = operation_product_next;
    operation->get = operation_product_get;
    operation->close = operation_product_close;
    operation->ncols = operation_ncols(operation1) + operation_ncols(operation2);
    operation->types = malloc(operation->ncols * sizeof(type_t));
    memcpy(operation->types, operation1->types, operation1->ncols * sizeof(type_t));
    memcpy(operation->types + operation1->ncols, operation2->types, operation2->ncols * sizeof(type_t));

    return operation;
}

