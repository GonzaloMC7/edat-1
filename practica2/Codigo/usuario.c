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
    char fecha [20];
    time_t rawtime;
    struct tm * timeinfo;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    sprintf(query, "SELECT usuario.scrname FROM usuario WHERE usuario.scrname='%s' AND usuario.exists='1';", scrn);
    fprintf(stdout, "%s\n", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idusuario, sizeof(SQLINTEGER), NULL);    
    ret = SQLFetch(stmt);
    if (SQL_SUCCEEDED(ret)) {
        fprintf(stderr, "Scrn no disponible\n");
        return ERROR;
    }
    SQLCloseCursor(stmt);

    /*Query*/
    sprintf(query, "SELECT max(usuario.idusuario) FROM usuario;");
    fprintf(stdout, "%s\n", query);
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
    strftime(fecha, 20, "%Y-%m-%d", timeinfo );
    
    /*Query*/
    sprintf(query, "INSERT INTO usuario(idusuario,nombre,ccard,scrname,joindate,expdate,exists) VALUES(%d, '%s', NULL, '%s', '%s', '1/1/10000', '1');", idusuario+1, name, scrn, fecha );
    fprintf(stdout, "%s\n", query);
    ret=SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
     if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }
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
    sprintf(query, "SELECT usuario FROM usuario WHERE usuario.scrname = '%s' AND usuario.exists='1';", scrn);
    fprintf(stdout, "%s\n", query);

    ret = SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        fprintf(stderr,"No existen usuarios con ese scrn\n");
        return ERROR;
    }
    
    SQLCloseCursor(stmt);


    sprintf(query, "UPDATE usuario SET exists = '0' WHERE usuario.scrname = '%s' AND usuario.exists='1';", scrn);
    fprintf(stdout, "%s\n", query);

    ret = SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }
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
    

    printf("Practica numero 2\n");
    printf("Realizada por: Emilio Cuesta y Adrian Fernandez\n");
    printf("Grupo: 1201\n\n");
    
    if(argc<3||(*argv[1]!='+' && *argv[1]!= '-')||(*argv[1]=='+' && argc!=4)||(*argv[1]=='-' && argc!=3)){
        fprintf(stderr, "Error en los parametros de entrada:\n");
        fprintf(stderr, "Si quiere añadir un usuario inserte:\n");
        fprintf(stderr, "%s + <screen_name> %c<full_name>%c\n", argv[0], 34, 34);
        fprintf(stderr, "Si quiere eliminar un usuario inserte:\n");
        fprintf(stderr, "%s - <screen_name> \n", argv[0]);
        return ERROR;
    }

    if(*argv[1]=='+'){
		  if(argc==4){
			if(usuario_mas(argv[2], argv[3])==OK){
				fprintf(stdout, "Usuario añadido correctamente!\n");
				return OK;
			}
		  }
	} 
    else if(*argv[1]=='-'){
		  if(argc==3){
			if(usuario_menos(argv[2])==OK){
				fprintf(stdout, "Usuario eliminado correctamente!\n");
				return OK;
			}
		  }
		}  


    fprintf(stdout, "Operacion fallida\n");

    return ERROR;
  	
    }
