#include <stdio.h>
#include "database/table.h"
#include "database/record.h"
#include "type/type.h"

int main(){

  table_t* tabla;
  type_t tipos[4], *types;
  int ncols = 4, i, v1 = 17;
  long v4 = 1234567;
  void* values[4], *returns[4];
  char v2[] = "prueba";
  double v3 = 12.5;
  record_t* record;

  tipos[0] = INT;
  tipos[1] = STR;
  tipos[2] = DBL;
  tipos[3] = LNG;

  values[0] = &v1;
  values[1] = v2;
  values[2] = &v3;
  values[3] = &v4;


  printf("Probando table_create\n");

  table_create("testtr.table", ncols, tipos);

  printf("Probando table_open\n");

  tabla = table_open("testtr.table");
  if(!tabla)
    return EXIT_FAILURE;
  printf("La tabla se creó correctamente\n");

  printf("La tabla tiene: %d columnas\n", table_ncols(tabla));
  types = table_types(tabla);
  if(!types)
    return EXIT_FAILURE;

  for(i = 0 ; i < table_ncols(tabla) ; i++)
    switch (types[i]) {
      case INT:
        printf("El type %d es un INT.\n", i + 1);
        break;
      case STR:
        printf("El type %d es un STR.\n", i + 1);
        break;
      case DBL:
        printf("El type %d es un DBL.\n", i + 1);
        break;
      case LNG:
        printf("El type %d es un LNG.\n", i + 1);
    }

  printf("Primera posición de la tabla: %ld\n", table_first_pos(tabla));
  printf("Última posición de la tabla: %ld\n", table_last_pos(tabla));

  printf("Probando table_insert_record\n");
  printf("Se insertarán los siguientes valores al registro\n");
  printf("%d\n", *((int*)values[0]));
  printf("%s\n", (char*)values[1]);
  printf("%lf\n", *((double*)values[2]));
  printf("%ld\n", *((long*)values[3]));
  table_insert_record(tabla, values);
  printf("Probando table_read_record\n");
  record = table_read_record(tabla, table_first_pos(tabla));
  if(!record){
    printf("Error al leer el registro\n");
    return EXIT_FAILURE;
  }

  for(i = 0; i < 4 ; i++){
    returns[i] = record_get(record, i);
  }
  printf("Los valores asignados al registro son:\n");
  printf("%d\n", *((int*)returns[0]));
  printf("%s\n", (char*)returns[1]);
  printf("%lf\n", *((double*)returns[2]));
  printf("%ld\n", *((long*)returns[3]));

  printf("Primera posición de la tabla: %ld\n", table_first_pos(tabla));
  printf("Última posición de la tabla: %ld\n", table_last_pos(tabla));

  printf("El siguiente registro se escribirá en: %ld\n", record_next(record));

  record_free(record);
  table_close(tabla);

  return EXIT_SUCCESS;
}
