/* PROJECTION MODULE */
/* PROJECTION STRUCTURE TO OBTAIN THE DERIVED COLUMNS OF A PROJECT OPERATION */
#ifndef PROJECTION_H
#define PROJECTION_H

#include "../../type/type.h"

struct operation_;

struct projection_ {
    void* (* project)(struct operation_* operation, void* args);
    void (* close)(void* args);
    type_t type;
    void* args;
};

typedef struct projection_ projection_t;

/* makes a projection based on the current result of the operation */
void*
projection_project(projection_t* projection, struct operation_* operation);

/* closes the projection, freeing the allocated resources */
void
projection_close(projection_t* projection);

/* returns the datatype of the result of the projection */
type_t
projection_type(projection_t* projection);

/* creates a P_COL projection */
projection_t*
projection_col_create(int ncol, type_t type);

/* creates a P_SUM projection */
projection_t*
projection_sum_create(int col1, int col2);

#endif
