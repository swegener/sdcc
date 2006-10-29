clean:
	$(MAKE) -C z80 clean BUILDDIR=$(BUILDDIR)
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f *.dep
	rm -rf obj

distclean:
	$(MAKE) -C z80 distclean BUILDDIR=$(BUILDDIR)
	rm -f Makefile
