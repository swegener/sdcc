PRJDIR = ..

include $(PRJDIR)/Makefile.common

PORTS = z80 gbz80

all: 
	$(MAKE) -C z80 _link-z80 _link-gbz80 E=$(E) BUILDDIR=../../bin/

install: all
	$(INSTALL) $(PRJDIR)/bin/link-z80$(EXEEXT) `echo $(DESTDIR)$(bindir)/link-z80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/link-z80$(EXEEXT)|sed '$(transform)'`
	$(INSTALL) $(PRJDIR)/bin/link-gbz80$(EXEEXT) `echo $(DESTDIR)$(bindir)/link-gbz80$(EXEEXT)|sed '$(transform)'`
	$(STRIP) `echo $(DESTDIR)$(bindir)/link-gbz80$(EXEEXT)|sed '$(transform)'`

uninstall:
	cd $(DESTDIR)$(bindir); rm -f link-z80$(EXEEXT) link-gbz80$(EXEEXT)

include clean.mk
