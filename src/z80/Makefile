PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = gen.o ralloc.o main.o
LIB = port.a

CFLAGS = -ggdb -Wall
CFLAGS += -I.. -I. -I../..

all: $(LIB)

$(LIB): peeph.rul peeph-z80.rul peeph-gbz80.rul $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	ranlib $(LIB)

%.rul: %.def
	$(AWK) -f ../SDCCpeeph.awk $< > $@

main.o: main.c peeph.rul peeph-z80.rul peeph-gbz80.rul

include clean.mk
