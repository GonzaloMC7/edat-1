#include "operation.h"

/* SEQUENTIAL SCAN OPERATION: iterative access over the records of a table */

typedef struct {
    table_t* table;
    long pos;
    record_t* record;
} operation_sequential_args_t;

void
operation_sequential_reset(void* vargs) {
    operation_sequential_args_t* args = vargs;

    args->pos = -1;
    record_free(args->record);
    args->record = NULL;
}

int operation_sequential_next(void* vargs) {
    operation_sequential_args_t* args = vargs;
    
    if (args->pos == -1) {
        args->pos = table_first_pos(args->table);
    } else {
        args->pos = record_next(args->record);
    }
    
    if (args->record)
      record_free(args->record);
    args->record = table_read_record(args->table, args->pos);

    return args->record != NULL;
}

void* operation_sequential_get(int col, void* vargs) {
    operation_sequential_args_t* args = vargs;
    
    return record_get(args->record, col);
}

void operation_sequential_close(void* vargs) {
    operation_sequential_args_t* args = vargs;
    
    record_free(args->record);
    free(args);
}

operation_t*
operation_sequential_create(table_t* table) {
    operation_t* operation;
    operation_sequential_args_t* args;

    args = malloc(sizeof (operation_sequential_args_t));
    args->table = table;
    args->pos = -1;
    args->record = NULL;

    operation = malloc(sizeof (operation_t));
    operation->args = args;
    operation->reset = operation_sequential_reset;
    operation->next = operation_sequential_next;
    operation->get = operation_sequential_get;
    operation->close = operation_sequential_close;
    operation->ncols = table_ncols(table);
    operation->types = malloc(operation->ncols * sizeof (type_t));
    memcpy(operation->types, table_types(table), operation->ncols * sizeof (type_t));

    return operation;
}

