PRJDIR = ../..

include $(PRJDIR)/Makefile.common

OBJ = i186.o ralloc.o gen.o tlcs900h.o gen_generic.o aop.o util.o
LIB = port.a

CFLAGS += -I.. -I. -I../..
LDFLAGS = -ggdb

all: $(LIB)

$(LIB): $(OBJ)
	rm -f $(LIB)
	ar r $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

%.rul: %.def
	$(AWK) -f ../SDCCpeeph.awk $< > $@

main.o: main.c peeph.rul peeph-z80.rul peeph-gbz80.rul mappings.i

i186.o: i186.c i186_mappings.i

include clean.mk
