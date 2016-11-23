#include "record.h"

struct record_ {
  /* To be implemented */
};

record_t* record_create(void** values, int ncols, long next) {
  /* To be implemented */
}

void* record_get(record_t* record, int n) {
  /* To be implemented */
}

long record_next(record_t* record) {
  /* To be implemented */
}

void record_free(record_t* record) {
  /* To be implemented */
}
