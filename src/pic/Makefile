PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = gen.o ralloc.o main.o glue.o pcode.o
LIB = port.a

CFLAGS = -ggdb -Wall -O2
CFLAGS += -I.. -I. -I../..

all: $(LIB)

#hmm, now why don't we use automake instead of hardcoding
#the dependencies?
pcode.o: pcode.h
gen.o:	pcode.h

main.o: main.c peeph.rul

$(LIB): peeph.rul $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	ranlib $(LIB)

peeph.rul: peeph.def
	$(AWK) -f ../SDCCpeeph.awk peeph.def > peeph.rul

include clean.mk
