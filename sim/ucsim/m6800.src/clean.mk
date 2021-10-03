# m6800.src/clean.mk

# Deleting all files created by building the program
# --------------------------------------------------
clean:
	rm -f *core *[%~] *.[oa] *.map
	rm -f .[a-z]*~
	rm -f sm6800$(EXEEXT) sm6800.exe
	rm -f sm68hc11$(EXEEXT) sm68hc11.exe
	rm -f ucsim_m6800$(EXEEXT) ucsim_m6800.exe
	rm -f ucsim_m68hc11$(EXEEXT) ucsim_m68hc11.exe
	$(MAKE) -C test -f clean.mk clean


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f config.cache config.log config.status
	rm -f Makefile *.dep
	rm -f *.obj *.list *.lst *.hex
	$(MAKE) -C test -f clean.mk distclean


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean

# End of m6800.src/clean.mk
