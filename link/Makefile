TOPDIR = ..
include $(TOPDIR)/Makefile.common

PORTS = _z80 _gbz80

all: 
	for i in $(PORTS); do make $$i; done

_z80: clean
	make -C z80
	cp z80/link$(E) $(TOPDIR)/bin/link-z80$(E)

_gbz80: clean
	make -C z80 "OPTS=-DGAMEBOY"
	cp z80/link$(E) $(TOPDIR)/bin/link-gbz80$(E)

clean:
	make -C z80 clean
