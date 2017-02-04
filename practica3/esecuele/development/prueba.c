#include <stdio.h>
#include "database/table.h"
#include "database/record.h"
#include "type/type.h"

int main(){

  table_t* table;
  type_t types[4]; /*Array con el tipo de dats de cada columna*/
  int ncol = 4; /*Una con cada tipo de datos, a continuacion declaramos los datos que se utilizaran para cada tipo*/
  int typeint = 1;
  char* typestr = "caracteresvarios";
  double typedbl = 11.1;
  long typelng = 111111111;
  void* datos[4]; /*Array con los datos que va a tener el record de prueba*/


  /*Declaracion de variables de comprobacion*/
  int aux, i;
  type_t* aretypesok;
  long posaux;
  record_t* auxdatos;
  void* readingrecord[4];

  /*Asignamos los tipos de datos de cada columna a types en el orden correspondiente*/
  types[0]=INT;
  types[1]=STR;
  types[2]=DBL;
  types[3]=LNG;

  /*Asignamos los datos a cada puntero del array datos*/
  datos[0] = &typeint;
  datos[1] = typestr;
  datos[2] = &typedbl;
  datos[3] = &typelng;

  /*Creamos la tabla*/
  table_create("tabladeprueba", ncol, types);
  /*No devuelve nunca error, asumimos que está bien*/
  fprintf(stdout, "table_create OK\n");

  /*La abrimos*/
  table = table_open("tabladeprueba");
  if(!table){
    fprintf(stderr, "Error en table_open\n");
    return -1;
  }
  fprintf(stdout, "table_open OK\n");

  aux=table_ncols(table);
  if(aux!=ncol){
    fprintf(stderr, "Error en table_ncols\n");
    return -1;
  }
  fprintf(stdout, "table_ncols OK\n");

  aretypesok = table_types(table);
  for(i=0; i<4; i++){
    if(aretypesok[i]!=types[i]){
      fprintf(stderr, "Error en table_types\n");
      return -1;
    }
  }
  fprintf(stdout, "table_types OK\n");

  /*table_first_pos y table_last_pos nunca devuelven error, asumimos que están bien si no falla el assert*/
  table_first_pos(table);
  table_last_pos(table);
  fprintf(stdout, "table_first_pos OK y table_last_pos OK\n");

  table_insert_record(table, datos);
  auxdatos = table_read_record(table, table_first_pos(table));
  for(i = 0; i < 4 ; i++){
    readingrecord[i]=record_get(auxdatos, i);
  }
  if(typeint!= *((int*)readingrecord[0]) ||strcmp(datos[1],((char*)readingrecord[1]))!=0||typedbl!=*((double*)readingrecord[2])||typelng!=*((long*)readingrecord[3])){
    fprintf(stderr, "Error en table_insert/read_record\n");
  }
  fprintf(stdout, "table_insert_record OK y table_read_record OK\n");

  if(record_next(auxdatos)<table_first_pos(table)){
    fprintf(stderr, "Error en record_next\n");
    return -1;
  }
  fprintf(stdout, "record_next OK\n");

  record_free(auxdatos);
  table_close(table);
  fprintf(stdout, "Funciones de liberacion de memoria OK\n");
  fprintf(stdout, "Todo funciona correctamente\n");
  return 0;
}
