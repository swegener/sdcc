# libc/z80 Makefile

TOPDIR = ../../..

SCC = $(TOPDIR)/bin/sdcc -mz80
SAS = as-z80

OBJ = div.o mul.o putchar.o string.o printf.o # asm_strings.o
LIB = z80.lib
CC = $(SCC)
AS = $(SAS)
CFLAGS = -I../include -I.

all: $(LIB) crt0.o

$(LIB): $(OBJ) Makefile _dummy
	rm -f $(LIB)
	for i in $(OBJ); do echo $$i >> $(LIB); done

.c.o:
	$(CC) $(CFLAGS) -c $@

_dummy:

clean:
	rm -f $(OBJ) *~ $(CLEANSPEC) *.dump*
