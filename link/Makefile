PRJDIR = ..

include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all: 
	$(MAKE) -C z80 _link-z80 _link-gbz80 E=$(E) BUILDDIR=../../bin/

clean:
	$(MAKE) -C z80 clean


# dummy install target to prevent warnings
install:
