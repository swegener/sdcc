# m68hc11.src/clean.mk

# Deleting all files created by building the program
# --------------------------------------------------
clean:
	rm -f *core *[%~] *.[oa] *.map
	rm -f .[a-z]*~
	rm -f sm68hc11$(EXEEXT) sm68hc11.exe
	rm -f ucsim_m68hc11$(EXEEXT) ucsim_m68hc11.exe
ifneq ($(shell test -f test/Makefile && echo ok), )
	$(MAKE) -C test clean
endif


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f config.cache config.log config.status
	rm -f Makefile *.dep
	rm -f *.obj *.list *.lst *.hex
	rm -f test/Makefile


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean

# End of m68hc11.src/clean.mk
