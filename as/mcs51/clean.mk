# Deleting all files created by building the program
# --------------------------------------------------
include ../../Makefile.common
PRJDIR = ../..

clean:
	rm -f *core *[%~] *.[oa]
	rm -f .[a-z]*~
	rm -f $(PRJDIR)/bin/asx8051$(EXEEXT) $(PRJDIR)/bin/aslink$(EXEEXT) asx8051$(EXEEXT) aslink$(EXEEXT)


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f Makefile *.dep


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean
