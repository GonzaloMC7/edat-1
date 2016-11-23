/* OPERATION MODULE */
/* OPERATION DATA STRUCTURE FOR GENERIC QUERYING ITERATIVE OPERATIONS OVER A DATABASE */
#ifndef OPERATION_H
#define OPERATION_H

#include "../database/table.h"
#include "../operation/condition/condition.h"
#include "../operation/projection/projection.h"

struct operation_ {
    void (* reset)(void* args);
    int (* next)(void* args);
    void* (* get)(int col, void* args);
    void (* close)(void* args);
    int ncols; /* number of columns of the operation */
    type_t* types; /* types of the columns of the operation */
    void* args;
};

typedef struct operation_ operation_t;

/* resets the iterator of the operation, so it goes back again to the first retrieved result*/
void
operation_reset(operation_t* operation);

/* moves forward the next result of the operation, return 0 if there are no more results, !=0 otherwise */
int
operation_next(operation_t* operation);

/* gets the value of the column col of the current result of the operation */
void*
operation_get(int col, operation_t* operation);

/* closes the operation freeing the allocated resources */
void
operation_close(operation_t* operation);

/* number of columns of the results of the operation */
int
operation_ncols(operation_t* operation);

/* types of the columns of the results of the operation */
type_t*
operation_types(operation_t* operation);

/* creates an SEQUENTIAL operation to iterate over all the records of a table */
operation_t*
operation_sequential_create(table_t* table);

/* creates a INDEX operation to iterate over the records that contain the value of the key indexed */
operation_t*
operation_index_create(table_t* table, index_t* index, int key);

/* creates a SELECT operation */
operation_t*
operation_select_create(operation_t* suboperation, condition_t* condition);

/* creates a PROJECT operation */
operation_t*
operation_project_create(operation_t* suboperation, projection_t** projections, int nprojections);

/* creates a PRODUCT operation */
operation_t*
operation_product_create(operation_t* operation1, operation_t* operation2);

/* creates a COUNT operation */
operation_t*
operation_count_create(operation_t* suboperation);

/* creates a UNION operation */
operation_t*
operation_union_create(operation_t* operation1, operation_t* operation2);

/* creates a LIMIT operation */
operation_t*
operation_limit_create(operation_t* suboperation, int N);

/* creates an OFFSET operation */
operation_t*
operation_offset_create(operation_t* suboperation, int offset);

#endif
