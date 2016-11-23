#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack/stack.h"
#include "../operation/operation.h"
#include "../operation/condition/condition.h"
#include "../operation/projection/projection.h"
#include "../database/database.h"

#define MAX_TOKENS 1000

int process_token(char* token, stack_t* stack, database_t* db) {
    if (strcmp("SEQUENTIAL", token) == 0) {
        /* table_name SEQUENTIAL */
        table_t* table;
        operation_t* operation;
        char* table_name = stack_pop(stack);
        table = database_get_table(db, table_name);
        if (table == NULL) {
            fprintf(stderr, "ERROR: Table %s does not exist.\n", table_name);
            return -1;
        }
        operation = operation_sequential_create(table);
        stack_push(stack, operation);
    } else if (strcmp("INDEX", token) == 0) {
        /* table_name col key INDEX */
        table_t* table;
        operation_t* operation;
        index_t* index;
        int key = atoi(stack_pop(stack));
        int col = atoi(stack_pop(stack));
        char* table_name = stack_pop(stack); /*table name*/
        index = database_get_index(db, table_name, col);
        if (index == NULL) {
            fprintf(stderr, "ERROR: Index for table %s on column %d does not exist.\n", table_name, col);
            return -1;
        }
        table = database_get_table(db, table_name);
        if (table == NULL) {
            fprintf(stderr, "ERROR: Table %s does not exist.\n", table_name);
            return -1;
        }
        operation = operation_index_create(table, index, key);
        stack_push(stack, operation);
    } else if (strcmp("SELECT", token) == 0) {
        /* operation condition SELECT */
        operation_t* operation;
        condition_t* condition;
        condition = stack_pop(stack);
        operation = stack_pop(stack);
        operation = operation_select_create(operation, condition);
        stack_push(stack, operation);
    } else if (strcmp("C_TRUE", token) == 0) {
        /* C_TRUE */
        condition_t* condition;
        condition = condition_true_create();
        stack_push(stack, condition);
    } else if (strcmp("C_AND", token) == 0) {
        /* condition1 condition2 C_AND */
        condition_t* condition;
        condition_t* condition2 = stack_pop(stack);
        condition_t* condition1 = stack_pop(stack);
        condition = condition_and_create(condition1, condition2);
        stack_push(stack, condition);
    } else if (strcmp("C_OR", token) == 0) {
        /* condition1 condition2 C_OR */
        condition_t* condition;
        condition_t* condition2 = stack_pop(stack);
        condition_t* condition1 = stack_pop(stack);
        condition = condition_or_create(condition1, condition2);
        stack_push(stack, condition);
    } else if (strcmp("C_NOT", token) == 0) {
        /* condition C_NOT */
        condition_t* condition = stack_pop(stack);
        condition = condition_not_create(condition);
        stack_push(stack, condition);
    } else if (strcmp("C_COLEQCOL", token) == 0) {
        /* col1 col2 C_COLEQCOL */
        condition_t* condition;
        int col2 = atoi(stack_pop(stack));
        int col1 = atoi(stack_pop(stack));
        condition = condition_coleqcol_create(col1, col2);
        stack_push(stack, condition);
    } else if (strcmp("C_COLEQCTE", token) == 0) {
        /* col type cte C_COLEQCTE */
        condition_t* condition;
        char* cte = stack_pop(stack);
        type_t type = type_parse(stack_pop(stack));
        int col = atoi(stack_pop(stack));
        void* value = value_parse(type, cte);
        condition = condition_coleqcte_create(col, value);
        stack_push(stack, condition);
    } else if (strcmp("PROJECT", token) == 0) {
        /* operation proj1 proj2 ... projN N PROJECT */
        operation_t* operation;
        int i;
        projection_t** projections;
        int nprojections = atoi(stack_pop(stack));
        projections = malloc(nprojections * sizeof (projection_t*));
        for (i = nprojections - 1; i >= 0; i--) {
            projections[i] = stack_pop(stack);
        }
        operation = stack_pop(stack);
        operation = operation_project_create(operation, projections, nprojections);
        stack_push(stack, operation);
    } else if (strcmp("P_COL", token) == 0) {
        /* type col P_COL */
        projection_t* projection;
        int col = atoi(stack_pop(stack));
        type_t type = type_parse(stack_pop(stack));
        projection = projection_col_create(col, type);
        stack_push(stack, projection);
    } else if (strcmp("P_SUM", token) == 0) {
        /* col1 col2 P_SUM */
        projection_t* projection;
        int col2 = atoi(stack_pop(stack));
        int col1 = atoi(stack_pop(stack));
        projection = projection_sum_create(col1, col2);
        stack_push(stack, projection);
    } else if (strcmp("PRODUCT", token) == 0) {
        /* operation1 operation2 PRODUCT */
        operation_t* operation;
        operation_t* operation2 = stack_pop(stack);
        operation_t* operation1 = stack_pop(stack);
        operation = operation_product_create(operation1, operation2);
        stack_push(stack, operation);
    } else if (strcmp("COUNT", token) == 0) {
        /* operation COUNT */
        operation_t* operation;
        operation = stack_pop(stack);
        operation = operation_count_create(operation);
        stack_push(stack, operation);
    } else if (strcmp("UNION", token) == 0) {
        /* operation1 operation2 UNION */
        operation_t* operation;
        operation_t* operation2 = stack_pop(stack);
        operation_t* operation1 = stack_pop(stack);
        operation = operation_union_create(operation1, operation2);
        stack_push(stack, operation);
    } else if (strcmp("LIMIT", token) == 0) {
        /* operation limit LIMIT */
        operation_t* operation;
        int limit = atoi(stack_pop(stack));
        operation = stack_pop(stack);
        operation = operation_limit_create(operation, limit);
        stack_push(stack, operation);
    } else if (strcmp("OFFSET", token) == 0) {
        /* operation offset OFFSET */
        operation_t* operation;
        int offset = atoi(stack_pop(stack));
        operation = stack_pop(stack);
        operation = operation_offset_create(operation, offset);
        stack_push(stack, operation);
    } else {
        stack_push(stack, token);
    }

    return 0;
}

/* special tokenizer: allows escaping spaces inside double quotes */
static char* tokenizer(char* str) {
    static char *s;
    char* e;
    int escape;
    char delim = ' ';

    if (str == NULL) {
        str = s;
        if (str == NULL) {
            return NULL;
        }
    }

    /* skip leading delimiters */
    for (; *str == delim; str++);
    escape = 0;
    s = NULL;
    for (e = str; *e != '\0'; e++) {
        if (*e == '"') {
            if (!escape) {
                str++;
                escape = 1;
            } else {
                *e = '\0';
                s = e + 1;
                break;
            }
        } else if (*e == delim && !escape) {
            *e = '\0';
            s = e + 1;
            break;
        }
    }

    if (str == e) {
        return NULL;
    } else {
        return str;
    }
}

operation_t* parser_operation(char* statement, database_t* db) {
    char* token;
    stack_t* stack;
    char* copy;
    operation_t* operation;
    int ret;

    copy = malloc((strlen(statement) + 1) * sizeof (char));
    strcpy(copy, statement);

    stack = stack_create(MAX_TOKENS);
    token = tokenizer(copy);
    ret = 0;
    while (token != NULL) {
        ret = process_token(token, stack, db);
        if (ret != 0) {
            break;
        }

        token = tokenizer(NULL);
    }

    if (ret != 0 || stack_size(stack) != 1) {
        fprintf(stderr, "Malformed statement\n");
        operation = NULL;
    } else {
        operation = stack_pop(stack);
    }

    stack_free(stack);
    free(copy);

    return operation;
}
