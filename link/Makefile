PRJDIR = ..

include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all: 
	for i in $(PORTS); do make _link THIS=$$i; done

_link: clean
ifeq ($(THIS), gbz80)
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS) OPTS=-DGAMEBOY
else
	make -C z80 BUILDDIR=../../bin EXT=-$(THIS)
endif

clean:
	make -C z80 clean
