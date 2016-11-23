#include "projection.h"
#include "../operation.h"

/* SUM PROJECTION: projects the sum of two INT columns of the input operation */

typedef struct {
    int col1;
    int col2;
} projection_sum_args_t;

void*
projection_sum_project(operation_t* operation, void* vargs) {
    projection_sum_args_t* args = vargs;
    int v1 = *((int*) operation_get(args->col1, operation));
    int v2 = *((int*) operation_get(args->col2, operation));

    int* sum = malloc(sizeof(int));
    *sum = v1 + v2;

    return sum;
}

void
projection_sum_close(void* vargs) {
    free(vargs);
}

projection_t*
projection_sum_create(int col1, int col2) {
    projection_t* projection;
    projection_sum_args_t* args;

    args = malloc(sizeof(projection_sum_args_t));
    args->col1 = col1;
    args->col2 = col2;

    projection = malloc(sizeof(projection_t));
    projection->project = projection_sum_project;
    projection->close = projection_sum_close;
    projection->type = INT;
    projection->args = args;

    return projection;
}
