/* INDEX MODULE */
/* INDEX STRUCTURE FOR STORING AND RETRIEVING KEY-POSITIONS PAIRS EFFICENTLY IN MEMORY */
#ifndef INDEX_H
#define INDEX_H

typedef struct index_ index_t;

/* Creates a file for saving an empty index */
int index_create();

/* Saving the current state of index in the database directory */
int index_save(index_t* index, char* path);

/* Opens a previously created index */
index_t* index_open(char* path);

/* Puts a pair key-position in the index */
int index_put(index_t *index, int key, long pos);

/* Retrieves all the positions associated with the key in the index */
void index_get(index_t *index, int key, long** poss, int* nposs);

/* Closes the index by freeing the allocated resources */
void index_close(index_t *index);

#endif