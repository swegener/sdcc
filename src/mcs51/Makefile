PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = gen.o ralloc.o main.o
LIB = port.a

CFLAGS = -ggdb -Wall
CFLAGS += -I.. -I. -I../..

all: $(LIB)

$(LIB): $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	ranlib $(LIB)

include clean.mk
