#include "projection.h"
#include "../operation.h"

/* SIMPLE COLUMN PROJECTION: projects a single column of the input operation */

typedef struct {
    int col;
} projection_col_args_t;

void*
projection_col_project(operation_t* operation, void* vargs) {
    projection_col_args_t* args = vargs;
    void* value1;
    void* value2;
    size_t size;
    
    value1 = operation_get(args->col, operation);
    size = value_length(operation_types(operation)[args->col], value1);
    value2 = malloc(size);
    memcpy(value2, value1, size);

    return value2;
}

void
projection_col_close(void* args) {
    free(args);
}

projection_t*
projection_col_create(int col, type_t type) {
    projection_t* projection;
    projection_col_args_t* args;

    args = malloc(sizeof(projection_col_args_t));
    args->col = col;

    projection = malloc(sizeof(projection_t));
    projection->project = projection_col_project;
    projection->close = projection_col_close;
    projection->type = type;
    projection->args = args;

    return projection;
}

