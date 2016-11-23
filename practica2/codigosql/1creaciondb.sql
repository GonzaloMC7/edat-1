--Adrian Fernandez y Emilio Cuesta

--Este archivo contiene los comandos para crear la bd de la libreria
--correspondiente a la P2 de EDAT 2016.



CREATE DATABASE libreria
  WITH OWNER = alumnodb
       ENCODING = 'UTF8'
       TABLESPACE = pg_default
       LC_COLLATE = 'en_US.UTF-8'
       LC_CTYPE = 'en_US.UTF-8'
       CONNECTION LIMIT = -1;
