TOPDIR = ..
include $(TOPDIR)/Makefile.common

PORTS = z80 gbz80

all: 
	for i in $(PORTS); do make $$i; done

z80: clean
	make -C z80
	cp z80/as $(TOPDIR)/bin/as-z80

gbz80: clean
	make -C z80 "OPTS=-DGAMEBOY"
	cp z80/as $(TOPDIR)/bin/as-gb

clean:
	make -C z80 clean
