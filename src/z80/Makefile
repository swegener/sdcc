PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = gen.o ralloc.o main.o gbz80.o
LIB = port.a

CFLAGS = -ggdb -Wall
CFLAGS += -I.. -I. -I../..

all: $(LIB)

$(LIB): peeph.rul $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	ranlib $(LIB)

peeph.rul: peeph.def
	$(AWK) -f ../SDCCpeeph.awk peeph.def > peeph.rul

peeph-gbz80.rul: peeph-gbz80.def
	$(AWK) -f ../SDCCpeeph.awk peeph-gbz80.def > peeph-gbz80.rul

main.o: main.c peeph.rul
gbz80.o: gbz80.o peeph-gbz80.rul

include clean.mk
