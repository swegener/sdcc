
# Deleting all files created by building the program
# --------------------------------------------------
clean:
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f spblaze spblaze.exe
	rm -f ucsim_pblaze ucsim_pblaze.exe
	rm -f ucsim_spblaze ucsim_spblaze.exe
ifneq ($(shell test -f test/Makefile && echo ok), )
	$(MAKE) -C test clean
endif


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f config.cache config.log config.status
	rm -f Makefile *.dep
	rm -f test/Makefile


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean

# End of pblaze.src/clean.mk
