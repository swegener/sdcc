clean:
	$(MAKE) -C z80 clean

distclean: clean
	$(MAKE) -C z80 distclean
	rm -f Makefile
