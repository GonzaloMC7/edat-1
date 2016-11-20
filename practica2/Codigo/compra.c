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
    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*Comprobamos que todos los isbn dados estan en la db*/
    /*Query*/
    for(i=0; i< nisbn; i++){

    sprintf(query, "SELECT libro.isbn FROM libro WHERE libro.isbn='%s';", isbn[i]);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    ret = SQLFetch(stmt);
    if(!SQL_SUCCEEDED(ret)){
        fprintf(stderr, "El libro con isbn %s no existe\n", isbn[i]);
        return ERROR;
    }
    SQLCloseCursor(stmt);

    }

    /*Obtenemos el maximo de los idventas para asignar a esta compra uno nuevo*/
    /*Query*/
    sprintf(query, "SELECT max(venta.idventa) FROM venta;");
    fprintf(stdout, "%s\n", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idventa, sizeof(SQLINTEGER), NULL);
    ret = SQLFetch(stmt);
    if(!SQL_SUCCEEDED(ret)){
        idventa=0;
    }
    
    SQLCloseCursor(stmt);

    /*Preparamos la fecha*/
    time ( &rawtime );
 	timeinfo = localtime ( &rawtime );
    strftime(fecha, 20, "%Y-%m-%d", timeinfo);

    /*Sacamos idusuario a partir de su scrname y lo guardamos en idusuario*/
    sprintf(query, "SELECT usuario.idusuario FROM usuario WHERE usuario.scrname='%s' AND usuario.exists='1';", scrn);
    fprintf(stdout, "%s\n", query);
    SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &idusuario, sizeof(SQLINTEGER), NULL);   
    ret = SQLFetch(stmt);
    if (!SQL_SUCCEEDED(ret)) {
    	fprintf(stderr, "No se encontro ningun usuario con ese scrname.\n");
        return ERROR;
    }
      
    SQLCloseCursor(stmt);

   

    /*El programa imprimirá en pantalla el coste de cada libro (uno por
     línea) teniendo en cuenta el descuento, y el coste total de la compra.*/

    for(i=0; i < nisbn; i++){
        /*Query*/
    	sprintf(query,  "SELECT O.fechai, O.fechaf, O.idoferta, O.descuento \n" 
						"FROM  oferta as O, libro as L, raplicada as R\n"
						"WHERE L.isbn='%s' AND L.isbn=R.isbn AND R.oferta=O.idoferta" , isbn[i]);
        fprintf(stdout, "%s\n", query);
        SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        SQLBindCol(stmt, 1, SQL_C_CHAR, &auxfechai, 20*sizeof(SQLCHAR), NULL);
    	SQLBindCol(stmt, 2, SQL_C_CHAR, &auxfechaf, 20*sizeof(SQLCHAR), NULL);   
    	SQLBindCol(stmt, 3, SQL_C_SLONG, &auxidoferta, sizeof(SQLINTEGER), NULL);
    	SQLBindCol(stmt, 4, SQL_C_SLONG, &auxdescuento, sizeof(SQLINTEGER), NULL);

    	ret = SQLFetch(stmt);
        
        /*Caso en que un libro tiene ofertas, válidas o no*/

        if(SQL_SUCCEEDED(ret)){


    		for(j=0; SQL_SUCCEEDED(ret); j++){

        		strcpy(fechasi[j], auxfechai);
    			strcpy(fechasf[j], auxfechaf);
        		idofertas[j]=auxidoferta;
    			descuentos[j]=auxdescuento;
    			ret = SQLFetch(stmt);

    		}
    	
    		SQLCloseCursor(stmt);
        
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

   			fprintf(stdout, "Oferta aplicada de %d%% con id %d\n", auxdescuento, auxidoferta);
   			/*el libro tiene ofertas en fecha*/
   			if(auxdescuento!=0){
				    
        		sprintf(query,  "SELECT L.precio as precio, O.descuento as descuento\n" 
							"FROM libro as L, oferta as O, raplicada as R\n" 
							"WHERE L.isbn='%s' AND L.isbn=R.isbn AND R.oferta=O.idoferta AND O.idoferta=%d\n" , isbn[i], auxidoferta);
       			fprintf(stdout, "%s\n", query);
        		SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        		SQLBindCol(stmt, 1, SQL_C_DOUBLE, &precio, sizeof(SQLDOUBLE), NULL);
    			SQLBindCol(stmt, 2, SQL_C_SLONG, &auxdescuento, sizeof(SQLDOUBLE), NULL);   
   				ret = SQLFetch(stmt);
   				SQLCloseCursor(stmt);
   			}
   			/*el libro no tiene ofertas en fecha*/
   			else{
   				fprintf(stdout,"No hay ofertas en fecha para el libro con isbn %s\n", isbn[i]);
   			}
   		}
   		
   		/*Caso en que el libro no tiene ofertas, ni en fecha ni fuera de fecha*/
   		else{
        
        	SQLCloseCursor(stmt);
        	fprintf(stdout, "No hay ofertas  para el libro con isbn %s\n", isbn[i]);
        	sprintf(query, "SELECT L.precio as precio\n" 
						   "FROM libro as L\n" 
						   "WHERE L.isbn='%s'", isbn[i]);
        	fprintf(stdout, "%s\n", query);
        	SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        	SQLBindCol(stmt, 1, SQL_C_DOUBLE, &precio, sizeof(SQLDOUBLE), NULL);
        	auxdescuento=0;
        	ret = SQLFetch(stmt);
        	if (!SQL_SUCCEEDED(ret)){
        		return ERROR;
        	}
        	SQLCloseCursor(stmt);
    	}
    	
    	
    	fprintf(stdout,"Libro con isbn: %s", isbn[i]);
    	fprintf(stdout,"\tSu precio es: %.2f\n", precio);
    	fprintf(stdout,"\tSu descuento es: %d%%\n", auxdescuento);
        importe=precio-precio*auxdescuento/100;
        total+=importe;
    	fprintf(stdout,"\tSu coste es: %.2f €\n", importe);
    }

    fprintf(stdout,"El coste total de la compra es de %.2f €\n", total);

     /*Insertamos los datos en venta*/
    /*Query*/
    fprintf(stdout, "Idusuario es: %d\n", idusuario);
    fprintf(stdout, "Idventa es: %d\n", idventa);

    sprintf(query, "INSERT INTO venta(idventa,idusuario,metododepago,fecha, importe) VALUES(%d, %d, 'ccard', '%s', %.2f);", idventa+1, idusuario, fecha, importe);
    fprintf(stdout, "%s\n", query);
    ret=SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    }

    SQLCloseCursor(stmt);

    /*Insertamos los datos en rincluido*/
    for(i=0; i < nisbn; i++){
        /*Query*/
        sprintf(query, "INSERT INTO rincluido(idventa,isbn) VALUES(%d, '%s');", idventa+1, isbn[i]);
        fprintf(stdout, "%s\n", query);
        ret=SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
        return ERROR;
    	}

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


