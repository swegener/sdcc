PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = icode.o gen.o ralloc.o main.o glue.o mem.o
LIB = port.a

CFLAGS = -ggdb -Wall
CFLAGS += -I.. -I. -I../..

all: $(LIB)

$(LIB): $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	ranlib $(LIB)

clean:
	rm -f $(LIB) *.o *~
