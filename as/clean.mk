clean:
	$(MAKE) -C z80 -f clean.mk clean BUILDDIR=$(BUILDDIR)

distclean:
	$(MAKE) -C z80 -f clean.mk distclean BUILDDIR=$(BUILDDIR)

