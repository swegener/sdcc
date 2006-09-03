CLEANALLPORTS = ds390 ds400 gbz80 hc08 large mcs51 medium pic pic16 small z80
include $(srcdir)/incl.mk

# Deleting all files created by building the program
# --------------------------------------------------
clean:
	rm -f *core *[%~] *.[oa] *.rel *.lst *.sym *.asm *.ihx *.dump* *.cdb
	rm -f .[a-z]*~
	rm -f *.dump*
	rm -rf build
	for model in $(MODELS); do \
	  if [ -d $$model ]; then \
	    cd $$model && for name in *; do if [ -f $$name -a $$name != Makefile -a $$name != Makefile.in ]; then rm $$name; fi; done; \
	  fi;\
	done
	for port in $(CLEANALLPORTS) ; do\
	  if [ -f $$port/Makefile ]; then\
	    $(MAKE) -C $$port clean ;\
	  fi;\
	done


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	rm -f Makefile *.dep
	for port in $(CLEANALLPORTS) ; do\
	  if [ -f $$port/Makefile ]; then\
	    $(MAKE) -C $$port distclean ;\
	  fi;\
	done

# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean
