TOPDIR = ..
include $(TOPDIR)/Makefile.common

PORTS = _z80 _gbz80

all: 
	for i in $(PORTS); do make $$i; done

_z80: clean
	make -C z80
	cp z80/as $(TOPDIR)/bin/as-z80

_gbz80: clean
	make -C z80 "OPTS=-DGAMEBOY"
	cp z80/as $(TOPDIR)/bin/as-gb

clean:
	make -C z80 clean
