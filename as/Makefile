PRJDIR = ..
BUILDDIR=../../bin
include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80 hc08
DOCS = README abstra.doc appendk.txt asmlnk.doc asxhtm.html format.txt

all:
	$(MAKE) -C z80 _as-z80 _as-gbz80 E=$(E) BUILDDIR=$(BUILDDIR)
	$(MAKE) -C hc08 all E=$(E) BUILDDIR=$(BUILDDIR)

install: all install-doc
	$(INSTALL) $(PRJDIR)/bin/as-z80$(EXEEXT) `echo $(DESTDIR)$(bindir)/as-z80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/as-z80$(EXEEXT)|sed '$(transform)'`
	$(INSTALL) $(PRJDIR)/bin/as-gbz80$(EXEEXT) `echo $(DESTDIR)$(bindir)/as-gbz80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/as-gbz80$(EXEEXT)|sed '$(transform)'`
	$(INSTALL) $(PRJDIR)/bin/as-hc08$(EXEEXT) `echo $(DESTDIR)$(bindir)/as-hc08$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/as-hc08$(EXEEXT)|sed '$(transform)'`

install-doc:
	$(INSTALL) -d $(DESTDIR)$(docdir)/aslink
	cp -f `find doc -maxdepth 1 -type f` $(DESTDIR)$(docdir)/aslink

uninstall:
	cd $(DESTDIR)$(bindir); rm -f as-z80$(EXEEXT) as-gbz80$(EXEEXT) as-hc08$(EXEEXT)

include clean.mk
