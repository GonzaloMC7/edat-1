/*Emilio Cuesta y Adrian Fernandez*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define ERROR -1
#define OK 0
#define OUTPUT 100


int best_seller(int num){

	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */

    SQLCHAR titlebuff[512];
    SQLINTEGER ventasbuff;
    char query[512];
    char bstitle[OUTPUT][200];
    int bsventas[OUTPUT];
    int i;
   
    /* CONNECT */
	ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);


    /*Crea una tabla con cada isbn y las veces que ha sido vendido y la ordena de mayor a menor número de ventas*/
    sprintf(query, "SELECT L.titulo, COUNT(R.idventa) as nventas\n" 
					"FROM rincluido as R , libro as L\n"
					"WHERE L.isbn=R.isbn\n" 
					"GROUP BY L.isbn\n" 
					"ORDER by nventas  DESC;\n");
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &titlebuff, 200*sizeof(SQLCHAR), NULL);
    SQLBindCol(stmt, 2, SQL_C_SLONG, &ventasbuff, sizeof(SQLINTEGER), NULL);
    
    ret = SQLFetch(stmt);

    /*Guarda los datos de las n primeras filas*/
    for(i=0; SQL_SUCCEEDED(ret) && i<num ; i++){

		strcpy(bstitle[i], (char*)titlebuff);
		bsventas[i]=ventasbuff;
		ret = SQLFetch(stmt);
    }

	SQLCloseCursor(stmt);

	/*Imprime los datos guardados anteriormente*/
	for(i=0; i<num; i++){

		fprintf(stdout,"Best seller #%d:\n", i+1);
		fprintf(stdout,"\t Titulo: %s\n", bstitle[i]);
		fprintf(stdout,"\t Numero de ventas: %d\n", bsventas[i]);
	}

    /* Free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    return OK;
}

int main(int argc, char** argv) {



	printf("Practica numero 2\n");
    printf("Realizada por: Emilio Cuesta y Adrian Fernandez\n");
    printf("Grupo: 1201\n\n");

    if(argc != 2 || atoi(argv[1]) < 1){
        fprintf(stderr, "Error en los parametros de entrada:\n");
        fprintf(stderr, "Si quiere obtener los n libros más vendidos introduzca:\n");
        fprintf(stderr, "%s  <n>\n", argv[0]);
        fprintf(stderr, "Donde n es un número entero mayor que 0\n");
        return ERROR;
    }
	if(best_seller(atoi(argv[1]))==OK){
		fprintf(stdout, "Operacion realizada correctamente\n");
		return OK;
	}    

	fprintf(stdout, "Operacion fallida\n");
	return ERROR;
}
