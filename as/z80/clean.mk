# Deleting all files created by building the program
# --------------------------------------------------
include ../../Makefile.common

clean:
	rm -rf obj
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f $(BUILDDIR)/as-z80$(EXEEXT)   as-z80$(EXEEXT)
	rm -f $(BUILDDIR)/as-gbz80$(EXEEXT) as-gbz80$(EXEEXT)

# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f *.dep


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean
