#include "operation.h"

/* INDEX SCAN OPERATION: retrieves the records of a table with an index */

typedef struct {
    index_t* index;
    table_t* table;
    long* poss;
    int i;
    int n;
    record_t* record;
} operation_index_args_t;

void
operation_index_reset(void* vargs) {
    operation_index_args_t* args = vargs;

    args->i = -1;
    record_free(args->record);
    args->record = NULL;
}

int operation_index_next(void* vargs) {
    operation_index_args_t* args = vargs;

    if (args->i == args->n - 1) {
        return 0;
    }

    args->i++;

    record_free(args->record);
    args->record = table_read_record(args->table, args->poss[args->i]);

    return args->record != NULL;
}

void* operation_index_get(int col, void* vargs) {
    operation_index_args_t* args = vargs;

    return record_get(args->record, col);
}

void operation_index_close(void* vargs) {
    operation_index_args_t* args = vargs;

    free(args->poss);
    record_free(args->record);
    free(args);
}

operation_t*
operation_index_create(table_t* table, index_t* index, int key) {
    operation_t* operation;
    operation_index_args_t* args;

    args = malloc(sizeof (operation_index_args_t));
    args->index = index;
    args->table = table;
    args->poss = NULL;
    args->n = 0;
    args->record = NULL;

    operation = malloc(sizeof (operation_t));
    operation->args = args;
    operation->reset = operation_index_reset;
    operation->next = operation_index_next;
    operation->get = operation_index_get;
    operation->close = operation_index_close;
    operation->ncols = table_ncols(table);
    operation->types = malloc(operation->ncols * sizeof (type_t));
    memcpy(operation->types, table_types(table), operation->ncols * sizeof (type_t));

    args->i = -1;
    index_get(args->index, key, &(args->poss), &(args->n));

    return operation;
}


