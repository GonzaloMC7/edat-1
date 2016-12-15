#
# Esecuele Makefile (non-NetBeans version)
#
# (C) Simone Santini, 2016
#

CC = gcc -c -std=c99
LNK = gcc

SRC=.
DBDIR=$(SRC)/database
OPDIR=$(SRC)/operation
CONDDIR=$(OPDIR)/condition
PROJDIR=$(OPDIR)/projection
TYPEDIR=$(SRC)/type
PARSEDIR=$(SRC)/parser
STACK=$(PARSEDIR)/stack

INCLUDE=-I$(DBDIR) -I$(OPDIR) -I$(CONDDIR) -I$(PROJDIR) -I$(TYPEDIR) -I$(PARSEDIR) -I$(STACK)

MAINOBJS= esecuele.o \
	database.o \
	index.o \
	record.o \
	table.o \
	operation.o \
	operation_count.o \
	operation_index.o \
	operation_limit.o \
	operation_offset.o \
	operation_product.o \
	operation_project.o \
	operation_select.o \
	operation_sequential.o \
	operation_union.o \
	condition_and.o \
	condition.o \
	condition_coleqcol.o \
	condition_coleqcte.o \
	condition_not.o \
	condition_or.o \
	condition_true.o \
	projection.o \
	projection_col.o \
	projection_sum.o \
	parser.o \
	stack.o \
	type.o


ALTOBJS= esecuele.o \
	database.o \
	index.o \
	objs/srecord.o \
	objs/stable.o \
	operation.o \
	operation_count.o \
	operation_index.o \
	operation_limit.o \
	operation_offset.o \
	operation_product.o \
	operation_project.o \
	operation_select.o \
	operation_sequential.o \
	operation_union.o \
	condition_and.o \
	condition.o \
	condition_coleqcol.o \
	condition_coleqcte.o \
	condition_not.o \
	condition_or.o \
	condition_true.o \
	projection.o \
	projection_col.o \
	projection_sum.o \
	parser.o \
	stack.o \
	type.o

#
# Algebra operations
#
operation.o:	$(OPDIR)/operation.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation.c

operation_count.o:	$(OPDIR)/operation_count.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_count.c

operation_index.o:	$(OPDIR)/operation_index.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_index.c

operation_limit.o:	$(OPDIR)/operation_limit.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_limit.c

operation_offset.o:	$(OPDIR)/operation_offset.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_offset.c

operation_product.o:	$(OPDIR)/operation_product.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_product.c

operation_project.o:	$(OPDIR)/operation_project.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_project.c

operation_select.o:	$(OPDIR)/operation_select.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_select.c

operation_sequential.o:	$(OPDIR)/operation_sequential.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_sequential.c

operation_union.o:	$(OPDIR)/operation_union.c $(OPDIR)/operation.h
	$(CC) $(OPDIR)/operation_union.c

condition.o:	$(CONDDIR)/condition.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition.c

condition_and.o:	$(CONDDIR)/condition_and.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_and.c

condition_coleqcol.o:	$(CONDDIR)/condition_coleqcol.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_coleqcol.c

condition_coleqcte.o:	$(CONDDIR)/condition_coleqcte.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_coleqcte.c

condition_not.o:	$(CONDDIR)/condition_not.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_not.c

condition_or.o:	$(CONDDIR)/condition_or.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_or.c

condition_true.o:	$(CONDDIR)/condition_true.c $(CONDDIR)/condition.h
	$(CC) $(CONDDIR)/condition_true.c

projection.o:	$(PROJDIR)/projection.c $(PROJDIR)/projection.h
	$(CC) $(PROJDIR)/projection.c

projection_col.o:	$(PROJDIR)/projection_col.c $(PROJDIR)/projection.h
	$(CC) $(PROJDIR)/projection_col.c

projection_sum.o:	$(PROJDIR)/projection_sum.c $(PROJDIR)/projection.h
	$(CC) $(PROJDIR)/projection_sum.c


#
# Parser
#
parser.o:	$(PARSEDIR)/parser.c $(PARSEDIR)/parser.h
	$(CC) $(PARSEDIR)/parser.c

stack.o:	$(STACK)/stack.c $(STACK)/stack.h
	$(CC) $(STACK)/stack.c



#
# Basic database management
#
database.o:	$(DBDIR)/database.c $(DBDIR)/database.h
	$(CC) $(DBDIR)/database.c

index.o:	$(DBDIR)/index.c $(DBDIR)/index.h
	$(CC) $(DBDIR)/index.c

record.o:	$(DBDIR)/record.c $(DBDIR)/record.h
	$(CC) $(DBDIR)/record.c

table.o:	$(DBDIR)/table.c $(DBDIR)/table.h
	$(CC) $(DBDIR)/table.c

type.o:	$(TYPEDIR)/type.c $(TYPEDIR)/type.o
	$(CC) $(TYPEDIR)/type.c

#
# main esecuele creation (with the table and record you made)
#

esecuele:	$(MAINOBJS)
	$(LNK) -o ../install/esecuele $(MAINOBJS)
	cp bank.bash ../install
	cp -r example_files ../install/


wvs:	$(ALTOBJS)
	$(LNK) -o ../install/esecuele $(ALTOBJS)
	cp bank.bash ../install
	cp -r example_files ../install/


clean:
	rm *.o
	rm -r ../install/*
