# Deleting all files created by building the program
# --------------------------------------------------
PRJDIR = ../..
include $(PRJDIR)/Makefile.common

clean:
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f $(PRJDIR)/bin/link-z80$(EXEEXT) link-z80$(EXEEXT) \
	      $(PRJDIR)/bin/link-gbz80$(EXEEXT) link-gbz80$(EXEEXT)
	rm -f *.dep
	rm -rf obj

# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean

# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean
