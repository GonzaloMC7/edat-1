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




int venta_anadir(char* scrn, char** isbn, int nisbn){

	/*Declaracion de variables*/
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */

    SQLINTEGER idventa, idusuario, auxidoferta;
    SQLINTEGER idofertas[100];
    SQLDOUBLE precio;
    char query[512];
    time_t rawtime;
    struct tm * timeinfo;
    char fecha[20], auxfechai[20], auxfechaf[20];
    char fechasi[100][20];
    char fechasf[100][20];
    int i,j,k,auxdescuento;
    int descuentos[100];
    float importe, total=0;
    

    /* Mediante esta función nos conectamos a la base de datos*/
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

   /* Creamos un contenedor que guarde las consultas de SQL */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Comprobamos que todos los isbn pasados como argumento estan en la db*/
    /*Si no esta alguno, se devuelve error*/
    for(i=0; i< nisbn; i++){

    sprintf(query, "SELECT libro.isbn FROM libro WHERE libro.isbn='%s';", isbn[i]);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    ret = SQLFetch(stmt);
    if(!SQL_SUCCEEDED(ret)){
        fprintf(stderr, "El libro con isbn %s no existe\n", isbn[i]);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        odbc_disconnect(env, dbc);
        return ERROR;
    }
    SQLCloseCursor(stmt);/*Este comando es muy importante, vacia  el contenedor*/

    }

   /*Obtenemos el maximo de los idventas existentes en la base de datos*/
    /*para asignar a esta compra uno que no este dentro (maximo +1)*/

    sprintf(query, "SELECT max(venta.idventa) FROM venta;");
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idventa, sizeof(SQLINTEGER), NULL);
    ret = SQLFetch(stmt);
    if(!SQL_SUCCEEDED(ret)){
        idventa=0;
    }
    
    SQLCloseCursor(stmt);

     /*Preparamos la fecha*/
    /*Mediante estas funciones, se obtiene la fecha del dia en que se llama la funcion*/
    time ( &rawtime );
 	timeinfo = localtime ( &rawtime );
    strftime(fecha, 20, "%Y-%m-%d", timeinfo);

    /*Sacamos el idusuario a partir de su scrname y lo guardamos en idusuario*/
    /*Es necesario comprobar que el usuario no ha sido borrado de la base de datos*/
    /*Si no hay usuario con ese scrname y que exista, se devuelve error*/

    sprintf(query, "SELECT usuario.idusuario FROM usuario WHERE usuario.scrname='%s' AND usuario.exists='1';", scrn);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idusuario, sizeof(SQLINTEGER), NULL);   
    ret = SQLFetch(stmt);
    if (!SQL_SUCCEEDED(ret)) {
    	fprintf(stderr, "No se encontro ningun usuario con ese scrname.\n");
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        odbc_disconnect(env, dbc);
        return ERROR;
    }
      
    SQLCloseCursor(stmt);

   
    for(i=0; i < nisbn; i++){
        /*Query*/
    	sprintf(query,  "SELECT O.fechai, O.fechaf, O.idoferta, O.descuento \n" 
						"FROM  oferta as O, libro as L, raplicada as R\n"
						"WHERE L.isbn='%s' AND L.isbn=R.libro AND R.oferta=O.idoferta" , isbn[i]);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        SQLBindCol(stmt, 1, SQL_C_CHAR, auxfechai, sizeof(auxfechai), NULL);
    	SQLBindCol(stmt, 2, SQL_C_CHAR, auxfechaf, sizeof(auxfechaf), NULL);   
    	SQLBindCol(stmt, 3, SQL_C_SLONG, &auxidoferta, sizeof(SQLINTEGER), NULL);
    	SQLBindCol(stmt, 4, SQL_C_SLONG, &auxdescuento, sizeof(SQLINTEGER), NULL);
        ret = SQLFetch(stmt);

        /*Caso en que un libro tiene ofertas, válidas o no (se comprueba luego si estan en fecha o no)*/
        if(SQL_SUCCEEDED(ret)){

            /*Se guardan las fechas de todas las ofertas en dos arrays (uno para inicio y otro para final)*/
            /*Tambien guardamos el descuento y el id de la oferta en arrays similares*/
    		for(j=0; SQL_SUCCEEDED(ret); j++){

        		strcpy(fechasi[j], auxfechai);
    			strcpy(fechasf[j], auxfechaf);
        		idofertas[j]=auxidoferta;
    			descuentos[j]=auxdescuento;
    			ret = SQLFetch(stmt);

    		}
    	
    		SQLCloseCursor(stmt);
        

            /*Con este bucle, hallamos el máximo descuento de las ofertas EN FECHA*/
            /*una vez hallado, sabemos que sus datos (fechas, id) estan en la misma posicion de los otros arrays*/

    		auxdescuento=0;
    		auxidoferta=0;
           
   			for(k=0; k<j; k++){
   				if(strcmp(fechasi[k], fecha)<0 && strcmp(fecha, fechasf[k])<0 ){
   					if(auxdescuento<descuentos[k]){
   						auxdescuento=descuentos[k];
   						auxidoferta=idofertas[k];
   					}
   				}
   			}

   			/*caso en el que el libro tiene ofertas en fecha*/
   			if(auxdescuento!=0){
				/*con esta query seleccionamos el precio y el descuento que se va a utilizar*/
        		sprintf(query,  "SELECT L.precio as precio, O.descuento as descuento\n" 
							     "FROM libro as L, oferta as O, raplicada as R\n" 
							     "WHERE L.isbn='%s' AND L.isbn=R.libro AND R.oferta=O.idoferta AND O.idoferta=%d\n" , isbn[i], auxidoferta);
        		SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        		SQLBindCol(stmt, 1, SQL_C_DOUBLE, &precio, sizeof(SQLDOUBLE), NULL);
    			SQLBindCol(stmt, 2, SQL_C_SLONG, &auxdescuento, sizeof(SQLDOUBLE), NULL);   
   				ret = SQLFetch(stmt);
   				SQLCloseCursor(stmt);
   			}
   			/*caso el libro no tiene ofertas en fecha. imprimimos mensaje y seleccionamos su precio normal*/
   			else{
                sprintf(query, "SELECT L.precio as precio\n" 
                                "FROM libro as L\n" 
                                "WHERE L.isbn='%s'", isbn[i]);
                SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
                SQLBindCol(stmt, 1, SQL_C_DOUBLE, &precio, sizeof(SQLDOUBLE), NULL);
                auxdescuento=0;
                ret = SQLFetch(stmt);
                if (!SQL_SUCCEEDED(ret)){
                    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                    odbc_disconnect(env, dbc);
                    return ERROR;
                }
                SQLCloseCursor(stmt);
   			}
   		}
   		
   		/*Caso en que el libro no tiene ofertas, ni en fecha ni fuera de fecha. Imprimimos mensaje y seleccionamos su precio normal*/
   		else{
            
        	SQLCloseCursor(stmt);
        	sprintf(query, "SELECT L.precio as precio\n" 
						   "FROM libro as L\n" 
						   "WHERE L.isbn='%s'", isbn[i]);
        	SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        	SQLBindCol(stmt, 1, SQL_C_DOUBLE, &precio, sizeof(SQLDOUBLE), NULL);
        	auxdescuento=0;
        	ret = SQLFetch(stmt);
        	if (!SQL_SUCCEEDED(ret)){
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                odbc_disconnect(env, dbc);
        		return ERROR;
        	}
        	SQLCloseCursor(stmt);
    	}
    	
    	/*Impresion de resultados por pantalla*/
    	fprintf(stdout,"Libro con isbn: %s\n", isbn[i]);
    	fprintf(stdout,"\tSu precio es: %.2f\n", precio);
    	fprintf(stdout,"\tSu descuento es: %d%%\n", auxdescuento);
        importe=precio-precio*auxdescuento/100;
        total+=importe;
    	fprintf(stdout,"\tSu coste es: %.2f €\n", importe);
    }

    fprintf(stdout,"El coste total de la compra es de %.2f €\n\n", total);

     /*Insertamos los datos en la tabla venta, incluyendo el importe total*/
    sprintf(query, "INSERT INTO venta(idventa,idusuario,metododepago,fecha, importe) VALUES(%d, %d, 'ccard', '%s', %.2f);", idventa+1, idusuario, fecha, total);
    ret=SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        odbc_disconnect(env, dbc);
        return ERROR;
    }

    SQLCloseCursor(stmt);

    /*Insertamos los datos en la tabla rincluido. Se rellenera una tupla por libro incluido*/
    for(i=0; i < nisbn; i++){
        /*Query*/
        sprintf(query, "INSERT INTO rincluido(idventa,isbn) VALUES(%d, '%s');", idventa+1, isbn[i]);
        ret=SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            odbc_disconnect(env, dbc);
            return ERROR;
    	}

        SQLCloseCursor(stmt);
    }



    /* Liberamos el contenedor*/
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* Desconectamos la base de datos */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    return OK;

}







int main(int argc, char** argv){

	char* isbn[100];
	int i, nisbn;

	printf("Practica numero 2\n");
    printf("Realizada por: Emilio Cuesta y Adrian Fernandez\n");
    printf("Grupo: 1201\n\n");
    
    if(argc<3){
        fprintf(stderr, "Error en los parametros de entrada:\n");
        fprintf(stderr, "Si quiere añadir una venta introduzca:\n");
        fprintf(stderr, "%s  <screen_name> <isbn> <isbn> .... <isbn>\n", argv[0]);
        return ERROR;
    }

   for (i = 0; i < argc-2 && i < 100; i++){
            isbn[i] = argv[i+2];
        }
        nisbn = i;

        while (i < 100){
            isbn[i] = NULL;
            i++;
        }

	if(venta_anadir(argv[1], isbn, nisbn) == OK){
		fprintf(stdout, "Compra añadida correctamente\n");
		return OK;
	}
        

	fprintf(stderr,"Operacion fallida.\n");
	return ERROR;

 }  
