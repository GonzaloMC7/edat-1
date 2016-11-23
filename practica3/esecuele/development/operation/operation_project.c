#include "operation.h"

/* PROJECT CONDITION: creates a projection of a previous operation */

typedef struct {
    operation_t* suboperation;
    projection_t** projections;
    int nprojections;
    void** values;
} operation_project_args_t;

void
operation_project_reset(void* vargs) {
    int i;
    operation_project_args_t* args = vargs;

    operation_reset(args->suboperation);
    for (i = 0; i < args->nprojections; i++) {
        free(args->values[i]);
        args->values[i] = NULL;
    }
}

int operation_project_next(void* vargs) {
    int i;
    operation_project_args_t* args = vargs;

    for (i = 0; i < args->nprojections; i++) {
        free(args->values[i]);
        args->values[i] = NULL;
    }

    return operation_next(args->suboperation);
}

void* operation_project_get(int col, void* vargs) {
    operation_project_args_t* args = vargs;

    if (args->values[col] == NULL) {
        args->values[col] = projection_project(args->projections[col], args->suboperation);
    }

    return args->values[col];
}

void operation_project_close(void* vargs) {
    int i;
    operation_project_args_t* args = vargs;

    operation_close(args->suboperation);
    for (i = 0; i < args->nprojections; i++) {
        free(args->values[i]);
        projection_close(args->projections[i]);
    }
    free(args->projections);
    free(args->values);

    free(args);
}

operation_t*
operation_project_create(operation_t* suboperation, projection_t** projections, int nprojections) {
    int i;
    operation_t* operation;
    operation_project_args_t* args;

    args = malloc(sizeof(operation_project_args_t));
    args->suboperation = suboperation;
    args->projections = projections;
    args->nprojections = nprojections;
    args->values = malloc((args->nprojections) * sizeof(void*));
    for (i = 0; i < args->nprojections; i++) {
        args->values[i] = NULL;
    }


    operation = malloc(sizeof(operation_t));
    operation->args = args;
    operation->reset = operation_project_reset;
    operation->next = operation_project_next;
    operation->get = operation_project_get;
    operation->close = operation_project_close;
    operation->ncols = nprojections;
    operation->types = malloc(operation->ncols * sizeof(type_t));
    for (i = 0; i < operation->ncols; i++) {
        operation->types[i] = projection_type(projections[i]);
    }

    return operation;
}

