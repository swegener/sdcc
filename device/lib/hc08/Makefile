# libc/hc08 Makefile

TOPDIR = ../../..

SCC = $(TOPDIR)/bin/sdcc -mhc08
SAS = $(TOPDIR)/bin/as6808

OBJ = _ret.rel _divuint.rel _divulong.rel

LIB = hc08.lib
CC = $(SCC)
AS = $(SAS)
ASFLAGS = -plosgff

CFLAGS = -I../../include -I.

all: $(LIB)

$(LIB): $(OBJ) Makefile _dummy
	rm -f $(LIB)
	for i in $(OBJ); do echo $$i >> $(LIB); done

%.rel: %.c
	$(CC) $(CFLAGS) -c $<

%.rel: %.asm
	$(AS) $(ASFLAGS) $@ $<

_dummy:

clean:
	rm -f *.o *.sym *.lst *~ $(CLEANSPEC) *.dump* *.asm *.lib
