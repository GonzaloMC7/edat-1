#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "database.h"
#include "table.h"
#include "index.h"

#define MAX_LONG_NAME 512
#define MAX_TABLES 512
#define MAX_INDEXES 512

struct database_ {
    char* path;
    /* tables */
    int ntables;
    char** table_names;
    table_t** tables;
    /* indexes */
    int nindexes;
    char** index_names;
    index_t** indexes;
    /* relationships between indexes and tables */
    int* table_nindexes;
    char*** table_indexes;
    int** table_cols;
};

static char* get_file_path(char* path, char* name, char* suf) {
    char* file_path = malloc((strlen(path) + strlen(name) + strlen(suf) + 2) * sizeof (char));
    strcpy(file_path, path);
    strcat(file_path, "/");
    strcat(file_path, name);
    strcat(file_path, suf);

    /* path + "/" + name + suf */
    return file_path;
}

int database_create(char* name) {
    database_t* database;
    int ret;

    /* create directory called after the database name */
    ret = mkdir(name, S_IRWXU);
    if (ret != 0) {
        fprintf(stderr, "createdb: database %s already exists\n", name);
        return -1;
    }

    /* creates an empty database */

    database = malloc(sizeof (database_t));
    database->path = NULL;
    /* tables */
    database->ntables = 0;
    database->table_names = NULL;
    database->tables = NULL;
    /* indexes */
    database->nindexes = 0;
    database->index_names = NULL;
    database->indexes = NULL;
    /* relationships between tables and indexes */
    database->table_indexes = NULL;
    database->table_nindexes = NULL;
    database->table_cols = NULL;

    /* saves the empty database */
    database_save(database, name);

    database_close(database);

    fprintf(stderr, "Creating database %s\n", name);
    return 0;
}

int database_save(database_t* db, char* name) {
    FILE* f;
    char* database_path;
    char* index_path;
    int i, j;

    /* tables */
    database_path = get_file_path(name, "db", "");
    f = fopen(database_path, "w");
    free(database_path);
    fwrite(&(db->ntables), sizeof (int), 1, f);
    for (i = 0; i < db->ntables; i++) {
        fwrite(db->table_names[i], MAX_LONG_NAME * sizeof (char), 1, f);
    }
    /* indexes */
    fwrite(&(db->nindexes), sizeof (int), 1, f);
    for (i = 0; i < db->nindexes; i++) {
        fwrite(db->index_names[i], sizeof (char) * MAX_LONG_NAME, 1, f);
        if (db->indexes[i] != NULL) {
            index_path = get_file_path(db->path, db->index_names[i], ".index");
            index_save(db->indexes[i], index_path);
            free(index_path);
        }
    }
    /* relationships between tables and indexes */
    for (i = 0; i < db->ntables; i++) {
        fwrite(&(db->table_nindexes[i]), sizeof (int), 1, f);
        for (j = 0; j < db->table_nindexes[i]; j++) {
            fwrite(db->table_indexes[i][j], MAX_LONG_NAME * sizeof (char), 1, f);
            fwrite(&(db->table_cols[i][j]), sizeof (int), 1, f);
        }
    }

    fclose(f);

    return 0;
}

database_t* database_open(char* name) {
    FILE* f;
    char* database_path;
    char* table_path;
    char* index_path;
    int i, j;
    database_t* db;

    db = malloc(sizeof (database_t));
    db->path = malloc((strlen(name) + 1) * sizeof (char));
    strcpy(db->path, name);

    database_path = get_file_path(name, "db", "");
    f = fopen(database_path, "r");
    free(database_path);

    if (f == NULL) {
        fprintf(stderr, "ERROR: Database %s does not exist\n", name);
        free(db->path);
        free(db);
        return NULL;
    }

    /* tables */
    fread(&(db->ntables), sizeof (int), 1, f);
    db->table_names = malloc(MAX_TABLES * sizeof (char*));
    db->tables = malloc(MAX_TABLES * sizeof (table_t*));
    for (i = 0; i < db->ntables; i++) {
        db->table_names[i] = calloc(MAX_LONG_NAME, sizeof (char));
        fread(db->table_names[i], sizeof (char), MAX_LONG_NAME, f);
        table_path = get_file_path(db->path, db->table_names[i], ".table");
        db->tables[i] = table_open(table_path);
        free(table_path);
    }
    /* indexes */
    fread(&(db->nindexes), sizeof (int), 1, f);
    db->index_names = malloc(MAX_INDEXES * sizeof (char*));
    db->indexes = malloc(MAX_INDEXES * sizeof (index_t*));
    for (i = 0; i < db->nindexes; i++) {
        db->index_names[i] = calloc(MAX_LONG_NAME, sizeof (char));
        fread(db->index_names[i], sizeof (char), MAX_LONG_NAME, f);
        index_path = get_file_path(db->path, db->index_names[i], ".index");
        db->indexes[i] = index_open(index_path);
        free(index_path);
    }

    /* relationships between tables and indexes */
    db->table_nindexes = malloc(MAX_TABLES * sizeof (int));
    db->table_indexes = malloc(MAX_TABLES * sizeof (char**));
    db->table_cols = malloc(MAX_TABLES * sizeof (int*));
    for (i = 0; i < db->ntables; i++) {
        fread(&(db->table_nindexes[i]), sizeof (int), 1, f);
        db->table_indexes[i] = malloc(MAX_INDEXES * sizeof (char*));
        db->table_cols[i] = malloc(MAX_INDEXES * sizeof (int));
        for (j = 0; j < db->table_nindexes[i]; j++) {
            db->table_indexes[i][j] = calloc(MAX_LONG_NAME, sizeof (char));
            fread(db->table_indexes[i][j], MAX_LONG_NAME * sizeof (char), 1, f);
            fread(&(db->table_cols[i][j]), sizeof (int), 1, f);
        }
    }

    fclose(f);

    return db;
}

void database_close(database_t* db) {
    int i, j;

    free(db->path);

    /* tables */
    for (i = 0; i < db->ntables; i++) {
        free(db->table_names[i]);
        if (db->tables[i] != NULL) {
            table_close(db->tables[i]);
        }
    }
    free(db->table_names);
    free(db->tables);

    /* indexes */
    for (i = 0; i < db->nindexes; i++) {
        free(db->index_names[i]);
        if (db->indexes[i] != NULL) {
            index_close(db->indexes[i]);
        }
    }
    free(db->index_names);
    free(db->indexes);

    /* relationships between tables and indexes */
    for (i = 0; i < db->ntables; i++) {
        for (j = 0; j < db->table_nindexes[i]; j++) {
            free(db->table_indexes[i][j]);
        }
        free(db->table_cols[i]);
        free(db->table_indexes[i]);
    }
    free(db->table_cols);
    free(db->table_indexes);
    free(db->table_nindexes);

    free(db);
}

/* adds a table to the database */
int database_add_table(database_t* db, char* table_name, int ncols, type_t* types) {
    char* table_path;
    
    table_path = get_file_path(db->path, table_name, ".table");
    table_create(table_path, ncols, types);
    free(table_path);
    
    db->table_names[db->ntables] = calloc(MAX_LONG_NAME, sizeof (char));
    strcpy(db->table_names[db->ntables], table_name);
    db->tables[db->ntables] = NULL;

    /* relationships between tables and indexes */
    db->table_nindexes[db->ntables] = 0;
    db->table_indexes[db->ntables] = malloc(MAX_INDEXES * sizeof (char*));
    db->table_cols[db->ntables] = malloc(MAX_INDEXES * sizeof (int));

    db->ntables++;

    return 0;
}

/* gets the table structure by name */
table_t* database_get_table(database_t* db, char* table_name) {
    int i;

    for (i = 0; i < db->ntables; i++) {
        if (strcmp(table_name, db->table_names[i]) == 0) {
            return db->tables[i];
        }
    }

    return NULL;
}

/* adds an index associated with a column of a table */
int database_add_index(database_t* db, char* index_name, char* table_name, int col) {
    int i;
    char* index_path;

    index_path = get_file_path(db->path, index_name, ".index");
    index_create(index_path);
    free(index_path);

    db->index_names[db->nindexes] = calloc(MAX_LONG_NAME, sizeof (char));
    strcpy(db->index_names[db->nindexes], index_name);
    db->indexes[db->nindexes] = NULL;
    db->nindexes++;

    /* relationships between tables and indexes */
    /* retrieve the position of the indexed table */
    for (i = 0; i < db->ntables; i++) {
        if (strcmp(table_name, db->table_names[i]) == 0) {
            break;
        }
    }
    db->table_indexes[i][db->table_nindexes[i]] = calloc(MAX_LONG_NAME, sizeof (char));
    strcpy(db->table_indexes[i][db->table_nindexes[i]], index_name);
    db->table_cols[i][db->table_nindexes[i]] = col;
    db->table_nindexes[i]++;

    return 0;
}

/* get index by name, not used outside this module */
index_t* database_get_index2(database_t* db, char* index_name) {
    int i;

    for (i = 0; i < db->nindexes; i++) {
        if (strcmp(index_name, db->index_names[i]) == 0) {
            return db->indexes[i];
        }
    }

    return NULL;
}

/* get index by the table and column it indexes */
index_t* database_get_index(database_t* db, char* table_name, int col) {
    int i, j;
    char* index_name;

    /* retrieve the position of the indexed table */
    for (i = 0; i < db->ntables; i++) {
        if (strcmp(table_name, db->table_names[i]) == 0) {
            break;
        }
    }
    if (i == db->ntables) {
        return NULL;
    }
    index_name = NULL;
    for (j = 0; j < db->table_nindexes[i]; j++) {
        if (db->table_cols[i][j] == col) {
            index_name = db->table_indexes[i][j];
            break;
        }
    }
    if (index_name == NULL) {
        return NULL;
    }

    return database_get_index2(db, index_name);
}

#define MAX_LEN_ROW 2048

/* special tokenizer that does not group consecutive identifiers as a single one, unlike strtok */
static char* tokenizer(char* str) {

    static char *lastToken = NULL; /* UNSAFE SHARED STATE! */
    char *tmp;
    char *delimiter = "\t";

    /* Skip leading delimiters if new string. */
    if (str == NULL) {
        str = lastToken;
        if (str == NULL) /* End of story? */
            return NULL;
    }

    /* Find end of segment */
    tmp = strpbrk(str, delimiter);
    if (tmp == NULL) {
        lastToken = NULL;
    } else {
        *tmp = '\0';
        lastToken = tmp + 1;
    }

    return str;
}

int database_copy(database_t* db, char* table_name, FILE* file) {
    void** row;
    int i;
    table_t* table;
    char line[MAX_LEN_ROW];
    char* token;
    type_t* types;
    int ncols;
    int c;
    index_t* index;
    int key;
    long pos;

    table = database_get_table(db, table_name);

    types = table_types(table);
    ncols = table_ncols(table);

    row = malloc(sizeof (void*) * table_ncols(table));
    c = 0;
    while (fgets(line, MAX_LEN_ROW, file) != NULL) {
        char* eol;
        /* lines starting with '#' are ignored */
        if (*line == '#') {
            continue;
        }
        /* removes the '\n' at the end of the line */
        eol = strchr(line, '\n');
        if (eol != NULL) {
            *eol = '\0';
        }
        i = 0;
        token = tokenizer(line);
        while (token != NULL && i < table_ncols(table)) {
            row[i] = value_parse(types[i], token);
            i++;
            token = tokenizer(NULL);
        }
        
        if (i != table_ncols(table)) {
            printf("%s\n", line);
        }

        /* get the position where the record will be inserted */
        pos = table_last_pos(table);
        /* insert the record at the end of the file */
        table_insert_record(table, row);
        /* updates the indexes */
        for (i = 0; i < ncols; i++) { /* for every column in the table */
            index = database_get_index(db, table_name, i);
            if (index != NULL) { /* if it is indexed, the index is updated */
                key = *((int*) row[i]);
                index_put(index, key, pos);
            }
        }

        for (i = 0; i < ncols; i++) {
            free(row[i]);
        }
        c++;
    }

    free(row);

    return c;
}
