PRJDIR = ..
BUILDDIR=../../bin
include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80
DOCS = README abstra.doc appendk.txt asmlnk.doc asxhtm.html format.txt

all:
	$(MAKE) -C z80 _as-z80 _as-gbz80 E=$(E) BUILDDIR=$(BUILDDIR)

install: all install-doc
	$(INSTALL) $(PRJDIR)/bin/as-z80$(EXEEXT) `echo $(bindir)/as-z80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(bindir)/as-z80$(EXEEXT)|sed '$(transform)'`
	$(INSTALL) $(PRJDIR)/bin/as-gbz80$(EXEEXT) `echo $(bindir)/as-gbz80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(bindir)/as-gbz80$(EXEEXT)|sed '$(transform)'`

install-doc:
	$(INSTALL) -d $(docdir)/aslink
	cp -f `find doc -maxdepth 1 -not -type d` $(docdir)/aslink

uninstall:
	cd $(bindir); rm -f as-z80$(EXEEXT) as-gbz80$(EXEEXT)

include clean.mk
