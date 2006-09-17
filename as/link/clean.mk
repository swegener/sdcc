clean:
	$(MAKE) -C z80 clean
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f *.dep
	rm -rf obj

distclean: clean
	$(MAKE) -C z80 distclean
	rm -f Makefile
