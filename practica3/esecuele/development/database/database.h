/* DATABASE MODULE */
/* DATABASE STRUCTURE TO HANDLE THE COMPONENTS OF A DATABASE: TABLES AND INDICES */
#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"
#include "index.h"

typedef struct database_ database_t;

/* Creates the directory and database file for the database */
int database_create(char* name);

/* Saves the current state of the db data type to disk */
int database_save(database_t* db, char* name);

/* Opens a previously created database */
database_t* database_open(char* name);

/* Closes the database freeing the allocated resources and closing open files */
void database_close(database_t* db);

/* Adds a table table_name to the database */
int database_add_table(database_t* db, char* table_name, int ncols, type_t* types);

/* Returns the table called table_name */
table_t* database_get_table(database_t* db, char* table_name);

/* Add an index (indexing a table by a column) */
int database_add_index(database_t* db, char* index_name, char* table_name, int col);

/* Returns the index that indexes the table table_name by column col */
index_t* database_get_index(database_t* db, char* table_name, int col);

/* Copies the contents of the file into table table_name. The file must be tab-separated and its columns must correspond one-to-one to the columns of the destination table */
int database_copy(database_t* db, char* table_name, FILE* file);

#endif