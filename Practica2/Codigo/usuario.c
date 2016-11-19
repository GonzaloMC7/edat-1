#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define ERROR -1
#define OK 0

int usuario_mas(char* scrn, char* name){

    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */

    SQLINTEGER idusuario;
    char query[512];

    time_t rawtime;
    struct tm * timeinfo;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Query*/
    sprintf(query, "SELECT max(usuario.uid) FROM usuario;");
    fprintf(stdout, "%s", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idusuario, sizeof(SQLINTEGER), NULL);
    ret = SQLFetch(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }
    SQLCloseCursor(stmt);

    /*Get the current time*/
  	time ( &rawtime );
 	timeinfo = localtime ( &rawtime );

    /*Query*/
    sprintf(query, "INSERT INTO usuario(idusuario,nombre,ccard,scrname,joindate,expdate,exists) VALUES(%d, %s, NULL, %s, %s, 1/1/10000, 1);", idusuario+1, name, scrn, asctime (timeinfo));
    fprintf(stdout, "%s", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLCloseCursor(stmt);

    /* Free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    return OK;
}

int usuario_menos(char* scrn){

	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */

    char query[512];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Query*/
    sprintf(query, "UPDATE usuario SET exists = 0 WHERE usuario.srcname = %s;", srcn);
    fprintf(stdout, "%s", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLCloseCursor(stmt);

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
   
    if(argc>1){
     	if(strcmp(argv[1], '+')==0){
		    if(argc==4){
				if(usuario_mas(argv[2], argv[3])==OK){
				    fprintf(stdout, "OK");
				    return 0;
				}
		    }
		} else if(strcmp(argv[1], '-')==0){
		    if(argc==3){
				if(usuario_menos(argv[2])==OK){
				    fprintf(stdout, "OK");
				    return 0;
				}
		    }
		}  
    }
	
    return 1;
}
