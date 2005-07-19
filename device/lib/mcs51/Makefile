
TOPDIR = ../../..

SAS = $(TOPDIR)/bin/asx8051
SCC = $(TOPDIR)/bin/sdcc

OBJ = crtstart.rel crtxinit.rel crtxclear.rel crtclear.rel crtxstack.rel \
      crtpagesfr.rel crtbank.rel

LIB = mcs51.lib

CC = $(SCC)
AS = $(SAS)
ASFLAGS = -plosgff

CFLAGS = -I../../include -I.

all: $(LIB)

$(LIB): $(OBJ) Makefile
	rm -f $(LIB)
	for i in $(OBJ); do echo $$i >> $(LIB); done

%.rel: %.c
	$(CC) $(CFLAGS) -c $<

%.rel: %.asm
	$(AS) $(ASFLAGS) $<

clean:
	rm -f *.rel *.sym *.lst *~ $(CLEANSPEC) *.dump* *.lib
