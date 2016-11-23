--Adrian Fernandez y Emilio Cuesta

--1. Dado un título, ¿Cuántas ediciones tiene?

SELECT  COUNT (isbn) as Ediciones
FROM    libro as L 
WHERE L.titulo='Contando Idiomas';

--¿En cuántos idiomas?

SELECT COUNT(Caux) as Idiomas
FROM   (SELECT DISTINCT idioma 
  FROM libro 
  WHERE titulo='Contando Idiomas') as Caux


                          
--2. ¿Cuántos libros se han vendido de un autor dado?


SELECT  COUNT(R.isbn) as LibrosVendidos
FROM libro as L, rincluido as R
WHERE L.autor='Arturo Perez Reverte' AND L.isbn=R.isbn


--3. ¿Cuántos libros de un autor dado se han vendido en oferta?

SELECT  COUNT(O.libro) as LibrosVendidos
FROM libro as L, rincluido as R, raplicada as O
WHERE L.autor='Arturo Perez Reverte' AND L.isbn=R.isbn AND L.isbn=O.libro 

--4. ¿Cuánto dinero se ha ganado vendiendo libros de un editor dado? 

SELECT SUM(Q.Precios) as Ganancia
FROM  (SELECT L.precio as Precios
       FROM rincluido as R, venta AS V, libro as L 
       WHERE L.editorial='Penguin ' AND L.isbn=R.isbn AND R.idventa=V.idventa 
       ) as Q
      
      
--5. ¿Cuántos libros han comprado los usuarios fidelizados?

SELECT COUNT(I.isbn) as Libros
FROM  venta as V, rincluido as I
WHERE  V.idusuario IS NOT NULL AND V.idventa=I.idrincluida 


--6. ¿Cuántos usuarios fidelizados han comprado libros en inglés?

SELECT COUNT(Q.Usuarios)
FROM  (SELECT DISTINCT V.idusuario as Usuarios
       FROM venta as V, rincluido as I, libro as L
       WHERE  V.idusuario IS NOT NULL AND V.idventa=I.idventa AND I.isbn=L.isbn AND L.idioma='Inglés' ) as Q 


--7. ¿Cuánto dinero se ha ganado vendiendo libros en Francés?

SELECT SUM(Q.Ganancia)
FROM  (SELECT L.precio as Ganancia
       FROM venta as V, rincluido as I,  libro as L
       WHERE   V.idventa=I.idventa AND I.isbn=L.isbn AND L.idioma='Francés' ) as Q

--8. ¿En que días hubo ofertas de libros de la editorial Adelpi?

SELECT O.fechai, O.fechaf
FROM libro as L, oferta as O, raplicada as R
WHERE  L.editorial ='Adelpi' AND L.isbn=R.libro AND R.oferta=O.idoferta  

--9. ¿Qué usuarios fidelizados no han comprado nunca libros de bolsillo?



(SELECT DISTINCT U.scrname
	FROM  usuario as U)
EXCEPT
     (SELECT DISTINCT  Us.scrname as Excluir
      FROM venta as V, libro as L, rincluido as R, usuario as Us
      WHERE   L.formato='De bolsillo' AND L.isbn=R.isbn AND  R.idventa=V.idventa AND V.idusuario IS NOT NULL AND V.idusuario=Us.idusuario)
      

