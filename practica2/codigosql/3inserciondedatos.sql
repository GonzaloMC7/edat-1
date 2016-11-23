--Adrian Fernandez y Emilio Cuesta

--Para cada fichero dado, creamos una tabla auxiliar con la misma cabecera que el archivo para facilitar
--así la inserción de datos.

--LibroTemporal
CREATE TABLE librotemporal(autor VARCHAR(200), titulo VARCHAR(300),
formato VARCHAR(100), numpag VARCHAR(100), editorial VARCHAR(100),
fecha VARCHAR(100), idioma VARCHAR(100), ISBN VARCHAR(100));

--Mediante la función COPY, relllenamos la tabla auxiliar con los datos del fichero.

COPY librotemporal 
FROM '/home/emi/Desktop/EDAT/practica2/Tablas/LIBROS_FINAL.txt' WITH DELIMITER '	' ENCODING 'ISO-8859-1';

--LibroPrecioTemp
CREATE TABLE libropreciotemp( ISBN VARCHAR(30), precio FLOAT);

COPY libropreciotemp 
FROM '/home/emi/Desktop/EDAT/practica2/Tablas/isbns_precios.txt' WITH DELIMITER '	' ENCODING 'ISO-8859-1';

--UsuariosTemp
CREATE TABLE usuariostemp(uid INTEGER, scrname VARCHAR(50), nombre VARCHAR(50),joindate VARCHAR(20), ccard VARCHAR(30), expiration VARCHAR(20));

COPY usuariostemp 
FROM '/home/emi/Desktop/EDAT/practica2/Tablas/usuarios.txt' WITH DELIMITER '	' ENCODING 'ISO-8859-1';

--VentasTemp
CREATE TABLE ventastemp(idventa INTEGER, uid INTEGER, ISBN VARCHAR(30), fecha VARCHAR(20));

COPY ventastemp 
FROM '/home/emi/Desktop/EDAT/practica2/Tablas/ventas.txt' WITH DELIMITER '	' ENCODING 'ISO-8859-1';

--Ahora hace falta mover los datos de nuestras tablas auxiliares a las tablas originales de nuestra base de datos. Para ello usamos el comando INSERT INTO (), que rellena todas las columnas que se indican en los parentesis pero pone el resto a NULL. 
--Ademas, como el fichero contenía isbns duplicados, se utiliza la función min para elegir los 
--valores de cada tupla del isbn. Esto modifica los datos originales pero tal y como estaban no podiamos insertarlos en nuestra DB.

INSERT INTO libro(idioma,fecha,isbn,editorial,formato,titulo,autor,numpag) 
(SELECT min(idioma), min(fecha),  isbn, min(editorial), min(formato), min(titulo), min(autor), min(numpag) 
 FROM LibroTemporal
 GROUP BY isbn); 

--Sin embargo, como el campo libro venía dado en otra tabla, para insertarlo necesitamos usar el comando UPDATE, de la siguiente manera:

UPDATE libro
SET precio=libropreciotemp.precio
FROM  libropreciotemp
WHERE libro.isbn=libropreciotemp.isbn;
 
--Realizamos lo mismo con usuario, venta y la relacion rincluido.

INSERT INTO usuario(idusuario,nombre,ccard,scrname,joindate,expdate, exists) 
(SELECT uid, min(nombre), min(ccard), min(scrname), min(joindate), min(expiration), '1'
 FROM usuariostemp
 GROUP BY uid) ;


INSERT INTO venta(idventa,idusuario,fecha) 
SELECT idventa, min(uid), min(fecha)
FROM ventastemp
GROUP BY idventa;

INSERT INTO rincluido(isbn,idventa) 
SELECT V.isbn, V.idventa
FROM ventastemp as V;

--Eliminamos las tablas auxiliares
DROP TABLE librotemporal;
DROP TABLE libropreciotemp;
DROP TABLE usuariostemp;
DROP TABLE ventastemp;



