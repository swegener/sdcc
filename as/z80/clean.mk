# Deleting all files created by building the program
# --------------------------------------------------
include $(top_builddir)/Makefile.common

clean: mostlyclean
	rm -f *.dep

# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f Makefile

# Like clean but some files may still exist
# -----------------------------------------
mostlyclean:
	rm -rf obj
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f $(top_builddir)/bin/as-z80$(EXEEXT)   as-z80$(EXEEXT)
	rm -f $(top_builddir)/bin/as-gbz80$(EXEEXT) as-gbz80$(EXEEXT)

# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean
