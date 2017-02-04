#!/bin/bash

COMMAND="./esecuele"

rm -rf libros_db

$COMMAND createdb libros_db

$COMMAND define libros_db << EOF
TABLE isbns_precios 2 STR DBL
TABLE libros 8 STR STR STR STR STR STR STR STR
TABLE usuarios 6 INT STR STR STR LNG STR
TABLE ventas 4 INT LNG STR STR
EOF

$COMMAND insert libros_db << EOF
COPY isbns_precios ./files/isbns_precios.txt
COPY libros ./files/LIBROS_FINAL.txt
COPY usuarios ./files/usuarios.txt
COPY ventas ./files/ventas.txt
EOF

# Test the following queries:
# - List of books bought by 'jack'
$COMMAND query libros_db << EOF
usuarios SEQUENTIAL 1 STR jack C_COLEQCTE SELECT INT 0 P_COL STR 1 P_COL 2 PROJECT ventas SEQUENTIAL PRODUCT 0 3 C_COLEQCOL SELECT STR 1 P_COL STR 4 P_COL 2 PROJECT libros SEQUENTIAL PRODUCT 1 9 C_COLEQCOL SELECT STR 0 P_COL STR 3 P_COL 2 PROJECT
EOF

# - Number of books bought by 'jack'
$COMMAND query libros_db << EOF
usuarios SEQUENTIAL 1 STR jack C_COLEQCTE SELECT INT 0 P_COL STR 1 P_COL 2 PROJECT ventas SEQUENTIAL PRODUCT 0 3 C_COLEQCOL SELECT STR 1 P_COL STR 4 P_COL 2 PROJECT libros SEQUENTIAL PRODUCT 1 9 C_COLEQCOL SELECT STR 0 P_COL STR 3 P_COL 2 PROJECT STR 1 P_COL 1 PROJECT COUNT
EOF

$COMMAND query libros_db
