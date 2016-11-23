#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"

struct table_ {
  /* To be implemented */
};

void table_create(char* path, int ncols, type_t* types) {
  /* To be implemented */
}

table_t* table_open(char* path) {
  /* To be implemented */
}

void table_close(table_t* table) {
  /* To be implemented */
}

int table_ncols(table_t* table) {
  /* To be implemented */
}

type_t* table_types(table_t* table) {
  /* To be implemented */
}

long table_first_pos(table_t* table) {
  /* To be implemented */
}

long table_last_pos(table_t* table) {
  /* To be implemented */
}

record_t* table_read_record(table_t* table, long pos) {
  /* To be implemented */
}

void table_insert_record(table_t* table, void** values) {
  /* To be implemented */
}
