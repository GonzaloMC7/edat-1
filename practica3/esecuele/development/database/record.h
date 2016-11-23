#ifndef RECORD_H
#define	RECORD_H

#include <stdio.h>
#include "../type/type.h"

typedef struct record_ record_t;

/* create a new record */
record_t* record_create(void** values, int ncols, long next);

/* gets the col-th value of the record */
void* record_get(record_t* record, int n);

/* gets the position of the next record */
long record_next(record_t* record);

/* free the resources allocated for the record */
void record_free(record_t* record);

#endif
