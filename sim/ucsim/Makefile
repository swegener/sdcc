#
# uCsim Makefile
#
# (c) Drotos Daniel, Talker Bt. 1997,99
#

STARTYEAR	= 1997

SHELL		= /bin/sh

PRJDIR		= .
PKGS		= cmd.src sim.src s51.src avr.src z80.src doc
#gui.src

srcdir          = .


# Compiling entire program or any subproject
# ------------------------------------------
all: main.mk
	$(MAKE) -f main.mk all
	@for pkg in $(PKGS); do\
	  cd $$pkg && $(MAKE) $$pkg ; cd ..;\
	done

libs: main.mk
	$(MAKE) -f main.mk libs


# Compiling and installing everything and runing test
# ---------------------------------------------------
.PHONY: install INSTALL Install
install:
	$(MAKE) -f main.mk install
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg install ;\
	done


# Deleting all the installed files
# --------------------------------
uninstall:
	$(MAKE) -f main.mk uninstall
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg uninstall ;\
	done


# Deleting all files created by building the program
# --------------------------------------------------
clean:
	$(MAKE) -f clean.mk clean
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg -f clean.mk clean ;\
	done


# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	$(MAKE) -f clean.mk distclean
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg -f clean.mk distclean ;\
	done
	rm -rf doc/*~ doc/*.bak


# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean
	$(MAKE) -f clean.mk mostlyclean
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg -f clean.mk mostlyclean ;\
	done


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, stc.
# -----------------------------------------------------------------------
realclean: distclean
	$(MAKE) -f clean.mk realclean
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg -f clean.mk realclean ;\
	done


# Creating distribution
# ---------------------
dist: distclean
	@if [ -f devel ]; then\
	  rm -f devel; mkdist ucsim; touch devel;\
	else\
	  mkdist ucsim;\
	fi


# Performing self-test
# --------------------
check:


# Performing installation test
# ----------------------------
installcheck:


# Creating dependencies
# ---------------------
dep:
	$(MAKE) -f main.mk dep
	@for pkg in $(PKGS); do\
	  $(MAKE) -C $$pkg dep ;\
	done


# My rules
# --------
putcopyright:
	'put(c)' -s $(STARTYEAR) *.cc *.h *.y *.l

start:
	date '+%Y.%m.%d-%H:%M' >.start

newer: distclean
	@if [ -f .start ]; then \
	  tar cvf - \
	    `find . -newer .start -type f -print` |\
	  gzip -9c >ucsim-newer-`cat .start`_`date '+%Y.%m.%d-%H:%M'`_`hostname`.tgz; \
	else \
	  echo ".start file not found.\n"; \
	  exit 1; \
	fi

print-newer:
	@if [ -f .start ]; then \
	  find . -newer .start -type f -print ;\
	else \
	  echo ".start file not found.\n" ;\
	  exit 1 ;\
	fi

new_files:
	diff -rNu $$HOME/clean-source/sdcc/sim/ucsim .|\
	grep '^diff' | grep -v CVS | awk '{print $$4}'

lines:
	@find . \( -name '*.[ch]' -o -name '*.cc' -o -name '*.hh' \) \
	-exec cat {} \; | clines


# Remaking configuration
# ----------------------
configure: configure.in
	$(SHELL) autoconf

main.mk: $(srcdir)/main_in.mk $(srcdir)/configure.in config.status
	$(SHELL) ./config.status

config.status: configure
	$(SHELL) ./config.status --recheck

makefiles:
	$(SHELL) ./config.status

freshconf: main.mk

checkconf:
	@if [ -f devel ]; then\
	  $(MAKE) freshconf;\
	fi

# End of Makefile
