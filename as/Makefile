PRJDIR = ..
include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all:
	for i in $(PORTS); do make _as THIS=$$i; done

_as: clean
ifeq ($(THIS), gbz80)
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS)$(E) OPTS=-DGAMEBOY
else
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS)$(E)
endif

clean:
	make -C z80 clean
