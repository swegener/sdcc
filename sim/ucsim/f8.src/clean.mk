# f8.src/clean.mk

srcdir		= .

# Deleting all files created by building the program
# --------------------------------------------------
clean:
	rm -f *core *[%~] *.[oa] *.map
	rm -f .[a-z]*~
	rm -f sf8$(EXEEXT) sf8.exe
	rm -f smcs6502$(EXEEXT) smcs6502.exe
	rm -f ucsim_f8$(EXEEXT) ucsim_f8.exe
	rm -f ucsim_mcs6502$(EXEEXT) ucsim_mcs6502.exe
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

# End of f8.src/clean.mk
