#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define ERROR -1
#define OK 0

int oferta_anadir(int discount, char* from, char* to, char** isbn, int nisbn){

    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */

    SQLINTEGER idoferta;
    char query[512];
    int i;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Query*/
    sprintf(query, "SELECT max(oferta.idoferta) FROM oferta;");
    fprintf(stdout, "%s\n", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idoferta, sizeof(SQLINTEGER), NULL);    
    ret = SQLFetch(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }
    SQLCloseCursor(stmt);

    /*Query*/
    sprintf(query, "INSERT INTO oferta(idoferta,descuento,fechai,fechaf) VALUES(%d, %d, %s, %s);", idoferta+1, discount, from, to);
    fprintf(stdout, "%s\n", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLCloseCursor(stmt);

    for(i=0; i < nisbn; i++){
        /*Query*/
        sprintf(query, "UPDATE raplicada SET oferta = %d WHERE raplicada.libro = %s;", idoferta+1, isbn[i]);
        fprintf(stdout, "%s\n", query);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        SQLCloseCursor(stmt);
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
    
    int i, nisbn;
    char* isbn[100];

    printf("Practica numero 2\n");
    printf("Realizada por: Emilio Cuesta y Adrian Fernandez\n");
    printf("Grupo: 1201\n\n");
    
    if(argc < 5){
        fprintf(stderr, "Error en los parametros de entrada:\n");
        fprintf(stderr, "Si quiere añadir una oferta inserte:\n");
        fprintf(stderr, "%s <descuento> <de> <a> <isbn> <isbn> .... <isbn>\n", argv[0]);
        return ERROR;
    }

    if(atoi(argv[1]) <= 100 && atoi(argv[1]) >= 0){

        for (i = 0; i < argc-4 && i < 100; i++){
            isbn[i] = argv[i+4];
        }
        nisbn = i;
        while (i < 100){
            isbn[i] = NULL;
            i++;
        }

	    if(oferta_anadir(atoi(argv[1]), argv[2], argv[3], isbn, nisbn) == OK){
		    fprintf(stdout, "Oferta añadida correctamente\n");
		    return OK;
	    }
        
	}  

    fprintf(stdout, "Operacion fallida\n");
    return ERROR;  	
}
