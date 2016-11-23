CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

EXE =  usuario oferta compra best_seller

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o odbc.o
