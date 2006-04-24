clean:
	$(MAKE) -C z80 clean BUILDDIR=$(BUILDDIR)

distclean:
	$(MAKE) -C z80 distclean BUILDDIR=$(BUILDDIR)
	rm -f Makefile
