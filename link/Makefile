TOPDIR = ..
include $(TOPDIR)/Makefile.common

PORTS = z80 gbz80

all: 
	for i in $(PORTS); do make $$i; done

z80: clean
	make -C z80
	cp z80/link$(E) $(TOPDIR)/bin/link-z80$(E)

gbz80: clean
	make -C z80 "OPTS=-DGAMEBOY"
	cp z80/link$(E) $(TOPDIR)/bin/link-gbz80$(E)

clean:
	make -C z80 clean
