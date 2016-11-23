/* TABLE MODULE */
/* TABLE STRUCTURE TO HANDLE THE DEFINITION OF RELATIONAL TABLES */
#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../type/type.h"
#include "index.h"
#include "record.h"

typedef struct table_ table_t;

/* Creates the files to store an empty table */
void
table_create(char* path, int ncols, type_t* types);

/* Opens a previously created table */
table_t* 
table_open(char* path);

/* Closes a table freeing the alloc'ed resources */
void
table_close(table_t* table);

/* returns the number of columns of the table */
int
table_ncols(table_t* table);

/* path to the file containing the records of the file */
char*
table_data_path(table_t* table);

/* returns the data types of the columns of the table */
type_t*
table_types(table_t* table);

/* returns the position of the first record of the table */
long table_first_pos(table_t* table);

/* returns the last position available, where a new record should be inserted */
long table_last_pos(table_t* table);

/* reads the record starting in the specified position */
record_t* table_read_record(table_t* table, long pos);

/* inserts a record in the last available position */
void table_insert_record(table_t* table, void** values);

#endif