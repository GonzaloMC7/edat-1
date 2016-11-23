#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "operation.h"
#include "../database/table.h"
#include "condition/condition.h"

/* INTERNALS OF THE OPERATION MODULE */

void
operation_reset(operation_t* operation) {
    operation->reset(operation->args);
}

int
operation_next(operation_t* operation) {
    return operation->next(operation->args);
}

void*
operation_get(int col, operation_t* operation) {
    return operation->get(col, operation->args);
}

void
operation_close(operation_t* operation) {
    operation->close(operation->args);
    free(operation->types);
    free(operation);
}

int
operation_ncols(operation_t* operation) {
    return operation->ncols;
}

type_t*
operation_types(operation_t* operation) {
    return operation->types;
}



