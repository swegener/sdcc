PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = gen.o ralloc.o main.o
LIB = port.a

CFLAGS = -g -Wall -O2
CFLAGS += -I.. -I. -I../..

all: $(LIB)

main.o: main.c peeph.rul

$(LIB): peeph.rul $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

peeph.rul: peeph.def
	$(AWK) -f ../SDCCpeeph.awk peeph.def > peeph.rul

include clean.mk
