PRJDIR = ..
include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all:
	$(MAKE) -C z80 _as-z80 _as-gbz80 E=$(E) BUILDDIR=../../bin/

clean:
	$(MAKE) -C z80 clean
