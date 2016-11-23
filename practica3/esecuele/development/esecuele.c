#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type/type.h"
#include "database/table.h"
#include "operation/operation.h"
#include "operation/condition/condition.h"
#include "parser/parser.h"
#include "database/database.h"

#define MAX_LEN_STATEMENT 1024

void createdb(char* name) {
    database_create(name);
}

void define(char* db_name) {
    database_t* db;
    char statement[MAX_LEN_STATEMENT];
    char* token;

    /* opens the database */
    db = database_open(db_name);
    if (db == NULL) {
        return;
    }

    /* interactive prompt, ends with Ctrl+D*/
    printf("d> ");
    fflush(stdout);
    while (fgets(statement, MAX_LEN_STATEMENT, stdin) != NULL) {
        *(strchr(statement, '\n')) = '\0';

        token = strtok(statement, " ");
        if (strcmp(token, "TABLE") == 0) {
            /* definition of a new table */
            char* table_name;
            int ncols;
            type_t* types;
            int i;

            table_name = strtok(NULL, " ");
            ncols = atoi(strtok(NULL, " "));
            types = malloc(ncols * sizeof (type_t));
            for (i = 0; i < ncols; i++) {
                types[i] = type_parse(strtok(NULL, " "));
            }

            /* the table is created and added to the database */
            database_add_table(db, table_name, ncols, types);

            fprintf(stderr, "Creating table %s\n", table_name);


            free(types);
        } else if (strcmp(token, "INDEX") == 0) {
            /* definition of a new index */
            char* index_name;
            char* table_name;
            int col;

            index_name = strtok(NULL, " ");
            table_name = strtok(NULL, " ");
            col = atoi(strtok(NULL, " "));

            /* the index is created and added to the database */
            database_add_index(db, index_name, table_name, col);
        } else {
            fprintf(stderr, "Unrecognized statement %s\n", token);
        }

        printf("\nd> ");
        fflush(stdout);
    }
    printf("\n");

    database_save(db, db_name);
    database_close(db);
}

void insert(char* db_name) {
    database_t* db;
    char statement[MAX_LEN_STATEMENT];
    char* token;
    int nrows;

    /* the database is opened */
    db = database_open(db_name);
    if (db == NULL) {
        return;
    }

    /* interactive prompt, ends with Ctrl+D*/
    printf("i> ");
    fflush(stdout);
    while (fgets(statement, MAX_LEN_STATEMENT, stdin) != NULL) {
        *(strchr(statement, '\n')) = '\0';

        token = strtok(statement, " ");
        if (strcmp(token, "COPY") == 0) {
            char* table_name;
            char* input;
            FILE* f;

            table_name = strtok(NULL, " ");
            input = strtok(NULL, " ");
	    printf("copy %s\n", input);

            /* the input file is opened to be read and copied to the assigned table */
            f = fopen(input, "r");
            nrows = database_copy(db, table_name, f);
            fclose(f);
            printf("(%d rows inserted)", nrows);
        } else {
            fprintf(stderr, "Unrecognized statement %s\n", token);
        }

        printf("\ni> ");
        fflush(stdout);
    }
    printf("\n");

    database_save(db, db_name);
    database_close(db);
}

void query(char* db_name) {
    database_t* db;
    char statement[MAX_LEN_STATEMENT];
    operation_t* operation;
    int i, ncols, nrows;
    type_t* types;

    /* opens de database */
    db = database_open(db_name);
    if (db == NULL) {
        return;
    }

    /* interactive prompt, ends with Ctrl+D*/
    printf("q> ");
    fflush(stdout);
    while (fgets(statement, MAX_LEN_STATEMENT, stdin) != NULL) {
        *(strchr(statement, '\n')) = '\0';

        /* the operation is parsed to create an operation */
        operation = parser_operation(statement, db);
        if (operation != NULL) {
            /* if the operation is valid */
            types = operation_types(operation);
            ncols = operation_ncols(operation);
            nrows = 0;
            /* for every result of the operation*/
            while (operation_next(operation)) {
                /* the values are printed */
                nrows++;
                for (i = 0; i < ncols; i++) {
                    print_value(stdout, types[i], operation_get(i, operation));
                    printf("\t");
                }
                printf("\n");
            }
            operation_close(operation);
            printf("(%d rows retrieved)", nrows);
        }
        printf("\nq> ");
        fflush(stdout);
    }
    printf("\n");

    database_close(db);
}

int main(int argc, char** argv) {
    char* command;
    char* db_name;

    if (argc != 3) {
        fprintf(stderr, "WRONG NUMBER OF ARGUMENTS\n");
        fprintf(stderr, "(createdb|define|insert|query) db_name\n");
        return EXIT_FAILURE;
    }
    command = argv[1];
    db_name = argv[2];

    if (strcmp(command, "createdb") == 0) {
        createdb(db_name);
    } else if (strcmp(command, "define") == 0) {
        define(db_name);
    } else if (strcmp(command, "insert") == 0) {
        insert(db_name);
    } else if (strcmp(command, "query") == 0) {
        query(db_name);
    } else {
        fprintf(stderr, "OPERATION NOT RECOGNIZED\n");
        fprintf(stderr, "(createdb|define|insert|query) db_name\n");
    }

    return (EXIT_SUCCESS);
}

