#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"
#include "record.h"
#include "../type/type.h"

/*#DEFINE NDEBUG*/

struct table_ {

  FILE* file; /*Tambien se habria podido poner el path pero seria mas problematico para fopen y flcose*/
  int ncols;
  type_t* types; /*Tipos de cada columna*/
  long first_pos; /*Final de la cabecera*/
  long last_pos; /*Donde estas*/


};

void table_create(char* path, int ncols, type_t* types) {
  int i;
  FILE* f;
  f = fopen(path, "w");

  /*fwrite ncols;*/
  fwrite(&ncols, sizeof(int), 1, f);
  /*fwrite types;*/
  fwrite(types, sizeof(type_t), ncols, f);
  fclose(f);
}

/*Deja el archivo abierto*/
table_t* table_open(char* path) {

  table_t* table;

  table=(table_t*)malloc(sizeof(table_t));
  if(!table) return NULL;

  table->file=fopen(path, "r+");
  fread(&table->ncols,sizeof(int),1,table->file);

  table->types=(type_t*)malloc(table->ncols*sizeof(type_t));

  fread(table->types, sizeof(type_t), table->ncols ,table->file);

  /*crear memoria para types*/
  /*Esta es la primera posicion util, todo lo de antes es cabecera*/
  table->first_pos=ftell(table->file);

  fseek(table->file, feof(table->file), SEEK_SET);
  table->last_pos=ftell(table->file);

  return table;
}

/*Cierra el archivo y libera memoria*/
void table_close(table_t* table) {
  int i;
  assert(table!=NULL);


  free(table->types);
  fclose(table->file);
  free(table);

  return;
}

int table_ncols(table_t* table) {
  assert(table!=NULL);

  return table->ncols;

}

type_t* table_types(table_t* table) {
  assert(table!=NULL);
  return table->types;
}

long table_first_pos(table_t* table) {
  assert(table!=NULL);
  return table->first_pos;
}

long table_last_pos(table_t* table) {
  assert(table!=NULL);
  return table->last_pos;
}

record_t* table_read_record(table_t* table, long pos) {

  int i;
  void** values;
  size_t size;
  long next;
  record_t* record;

  assert(table!=NULL);

  /*el fichero de la tabla que pasamos ya esta abierto*/
  /*nos colocamos donde toca*/

  fseek(table->file, pos, SEEK_SET);

  values = (void**) malloc (sizeof (void*) * table->ncols);

  for(i=0; i<table->ncols; i++){
    if(fread(&size, sizeof(size_t), 1 , table->file)<1)
      return NULL;
    values[i] = (void*) malloc (size);
    fread(values[i], size, 1, table->file);
  }

  next = ftell(table->file);
  record = record_create(values, table->ncols, next);

  return record;

}

void table_insert_record(table_t* table, void** values) {
    int i;
    size_t size;
    assert(table!=NULL);
    fseek(table->file, 0L, SEEK_END);

    for(i=0; i<table->ncols; i++){
      size = value_length(table->types[i], values[i]);
      fwrite(&size , sizeof(size_t) , 1 , table->file);
      fwrite(values[i] , size , 1 , table->file);
    }

    table->last_pos=ftell(table->file);

    return;
}
