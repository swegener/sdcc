PRJDIR = ..
include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all:
	for i in $(PORTS); do make _as THIS=$$i; done

_as:
ifeq ($(THIS), gbz80)
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS)$(E) OPTS=-DGAMEBOY wary
else
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS)$(E) wary
endif

clean:
	make -C z80 clean
