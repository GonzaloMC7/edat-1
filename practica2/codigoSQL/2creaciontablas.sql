--Creamos un par de secuencias que se utilizaran como pkey de las relaciones.
-- DROP SEQUENCE public.idraplicada;

CREATE SEQUENCE public.idraplicada
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 23477878578423489
  START 1
  CACHE 1;
ALTER TABLE public.idraplicada
  OWNER TO alumnodb;


-- Sequence: public.idrincluido

-- DROP SEQUENCE public.idrincluido;

CREATE SEQUENCE public.idrincluido
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 39489328494390
  START 1
  CACHE 1;
ALTER TABLE public.idrincluido
  OWNER TO alumnodb;


--Creamos la tabla libro

-- Table: public.libro

-- DROP TABLE public.libro;

CREATE TABLE public.libro
(
  precio double precision,
  idioma character varying(50),
  fecha character varying(50),
  isbn character varying(20) NOT NULL,
  editorial character varying(100),
  formato character varying(100),
  titulo character varying(200),
  autor character varying(100) NOT NULL,
  numpag character varying(100),
  CONSTRAINT isbn PRIMARY KEY (isbn)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.libro
  OWNER TO alumnodb;


--Creamos la tabla oferta:

 -- Table: public.oferta

-- DROP TABLE public.oferta;

CREATE TABLE public.oferta
(
  idoferta integer NOT NULL,
  descuento integer,
  fechai character varying(15),
  fechaf character varying(15),
  CONSTRAINT idoferta PRIMARY KEY (idoferta)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.oferta
  OWNER TO alumnodb;

--Creamos la tabla usuario

-- Table: public.usuario

-- DROP TABLE public.usuario;

CREATE TABLE public.usuario
(
  idusuario integer NOT NULL,
  registrodegastos double precision,
  nombre character varying(50),
  dni character varying(20),
  ccard character varying(30),
  scrname character varying(50),
  joindate character varying(20),
  expdate character varying(20),
  CONSTRAINT idusuario PRIMARY KEY (idusuario),
  exists bit
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.usuario
  OWNER TO alumnodb;

-- Creamos la tabla venta

-- Table: public.venta

-- DROP TABLE public.venta;

CREATE TABLE public.venta
(
  importe double precision,
  idventa integer NOT NULL,
  idusuario integer,
  metododepago character varying(20),
  fecha character varying(15),
  CONSTRAINT idventa PRIMARY KEY (idventa),
  CONSTRAINT descuentouser FOREIGN KEY (idusuario)
      REFERENCES public.usuario (idusuario) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.venta
  OWNER TO alumnodb;

-- Index: public."fki_10%"

-- DROP INDEX public."fki_10%";

CREATE INDEX "fki_10%"
  ON public.venta
  USING btree
  (idusuario);

--Creamos la tabla raplicada

-- Table: public.raplicada

-- DROP TABLE public.raplicada;

CREATE TABLE public.raplicada
(
  oferta integer NOT NULL,
  libro character varying(200) NOT NULL,
  idraplicada integer NOT NULL DEFAULT nextval('idraplicada'::regclass),
  CONSTRAINT id PRIMARY KEY (idraplicada),
  CONSTRAINT isbn FOREIGN KEY (libro)
      REFERENCES public.libro (isbn) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT oferta FOREIGN KEY (oferta)
      REFERENCES public.oferta (idoferta) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.raplicada
  OWNER TO alumnodb;

-- Index: public.fki_isbn

-- DROP INDEX public.fki_isbn;

CREATE INDEX fki_isbn
  ON public.raplicada
  USING btree
  (libro COLLATE pg_catalog."default");

-- Index: public.fki_oferta

-- DROP INDEX public.fki_oferta;

CREATE INDEX fki_oferta
  ON public.raplicada
  USING btree
  (oferta);


--Creamos la tabla rincluido

-- Table: public.rincluido

-- DROP TABLE public.rincluido;

CREATE TABLE public.rincluido
(
  idventa integer NOT NULL,
  isbn character varying(20) NOT NULL,
  idrincluida integer NOT NULL DEFAULT nextval('idrincluido'::regclass),
  CONSTRAINT idrincluida PRIMARY KEY (idrincluida),
  CONSTRAINT libro FOREIGN KEY (isbn)
      REFERENCES public.libro (isbn) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT venta FOREIGN KEY (idventa)
      REFERENCES public.venta (idventa) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.rincluido
  OWNER TO alumnodb;

-- Index: public."fki_Edicion"

-- DROP INDEX public."fki_Edicion";

CREATE INDEX "fki_Edicion"
  ON public.rincluido
  USING btree
  (isbn COLLATE pg_catalog."default");

-- Index: public."fki_ISBN"

-- DROP INDEX public."fki_ISBN";

CREATE INDEX "fki_ISBN"
  ON public.rincluido
  USING btree
  (idventa);

-- Index: public."fki_Libro"

-- DROP INDEX public."fki_Libro";

CREATE INDEX "fki_Libro"
  ON public.rincluido
  USING btree
  (isbn COLLATE pg_catalog."default");


