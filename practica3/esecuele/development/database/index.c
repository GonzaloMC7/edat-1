#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"

struct index_ {
    /* to be implemented */
};

typedef struct {
} irecord;

int mycmp(const void *kptr, const void *e) {
  /*
  irecord *rec = *((irecord **) e);
  if (key < rec->key)
    return -1;
  else if (key > rec->key)
    return 1;
  else 
    return 0;
  */
}

int index_create(char* path) {
    /* to be implemented */
}

int index_save(index_t *index, char* path) {
    /* to be implemented */
}

index_t* index_open(char* path) {
    /* to be implemented */
}

int index_put(index_t *index, int key, long pos) {
    /* to be implemented */
}

void index_get(index_t *index, int key, long** offsets, int* noffsets) {
}

void index_close(index_t *index) {
    /* to be implemented */
}
