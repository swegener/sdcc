# libc/z80 Makefile

TOPDIR = ../../..

SCC = $(TOPDIR)/bin/sdcc -mgbz80 -v
SAS = as-gb

OBJ = putchar.o string.o printf.o div.o mul.o asm_strings.o
LIB = z80.lib
CC = $(SCC)
AS = $(SAS)
CFLAGS = -I../include -I. --dumpall

all: $(LIB) crt0.o

$(LIB): $(OBJ) Makefile _dummy
	rm -f $(LIB)
	for i in $(OBJ); do echo $$i >> $(LIB); done

_dummy:

clean:
	rm -f $(OBJ) *~ $(CLEANSPEC)


