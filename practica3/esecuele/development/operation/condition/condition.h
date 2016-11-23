/* CONDITION MODULE */
/* DATA STRUCTURE CONDITION FOR THE SELECT OPERATION */
#ifndef CONDITION_H
#define CONDITION_H

#include "../../type/type.h"

struct operation_;

struct condition_ {
    int (* evaluation)(struct operation_* operation, void* args);
    void (* close)(void* args);
    void* args;
};

typedef struct condition_ condition_t;

/* Returns a boolean value to check if the current result of the operation fulfills the condition */
int
condition_evaluation(condition_t* condition, struct operation_* operation);

/* Closes the condition freeing the resources allocated*/
void
condition_close(condition_t* condition);

/* Creates a C_TRUE condition */
condition_t*
condition_true_create();

/* Creates a C_NOT condition */
condition_t*
condition_not_create(condition_t* condition);

/* Creates a C_AND condition */
condition_t*
condition_and_create(condition_t* condition1, condition_t* condition2);

/* Creates a C_OR condition */
condition_t*
condition_or_create(condition_t* condition1, condition_t* condition2);

/* Creates a C_COLEQCTE condition */
condition_t*
condition_coleqcte_create(int ncol, void* constant);

/* Creates a C_COLEQCOL condition */
condition_t*
condition_coleqcol_create(int ncol1, int ncol2);

#endif
