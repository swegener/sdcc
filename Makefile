#
#
#

SHELL		= /bin/sh
AUTOCONF	= autoconf

PRJDIR		= .
PKGS		= support/gc support/cpp \
		  src as/mcs51 debugger/mcs51 \
		  device/include device/lib sim/mcs51
PRJS		= sim/mcs51

srcdir          = .


# Compiling entire program or any subproject
# ------------------------------------------
all: checkconf
	for pkg in $(PKGS); do $(MAKE) -C $$pkg; done
	$(MAKE) -f main.mk all


# Compiling and installing everything and runing test
# ---------------------------------------------------
install:
	$(MAKE) -f main.mk install
	@for pkg in $(PKGS); do\
	  cd $$pkg && $(MAKE) install; cd ..;\
	done


# Deleting all the installed files
# --------------------------------
uninstall:
	$(MAKE) -f main.mk uninstall
	@for pkg in $(PKGS); do\
	  cd cmd && $(MAKE) uninstall; cd ..;\
	done


# Deleting all files created by building the program
# --------------------------------------------------
clean:
	$(MAKE) -f clean.mk clean
	for pkg in $(PKGS); do $(MAKE) -C $$pkg clean; done
	@for prj in $(PRJS); do\
	  cd $$prj && $(MAKE) clean; cd ..;\
	done
 

# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean
	$(MAKE) -f clean.mk distclean
	@for pkg in $(PKGS); do\
	  cd $$pkg && $(MAKE) -f clean.mk distclean; cd ..;\
	done
	@for prj in $(PRJS); do\
	  cd $$prj && $(MAKE) distclean; cd ..;\
	done
 

# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean
	$(MAKE) -f clean.mk mostlyclean
	@for pkg in $(PKGS); do\
	  cd $$pkg && $(MAKE) -f clean.mk mostlyclean; cd ..;\
	done
	@for prj in $(PRJS); do\
	  cd $$prj && $(MAKE) mostlyclean; cd ..;\
	done


# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, stc.
# -----------------------------------------------------------------------
realclean: distclean
	$(MAKE) -f clean.mk realclean
	@for pkg in $(PKGS); do\
	  cd $$pkg && $(MAKE) -f clean.mk realclean; cd ..;\
	done
	@for prj in $(PRJS); do\
	  cd $$prj && $(MAKE) realclean; cd ..;\
	done


# Creating distribution
# ---------------------
dist: distclean
	@if [ -f devel ]; then\
	  rm -f devel; mkdist sdcc; touch devel;\
	else\
	  mkdist sdcc;\
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
	  cd $$pkg && $(MAKE) dep; cd ..;\
	done


# My rules
# --------

putcopyright:
	'put(c)' -s $(STARTYEAR) *.cc *.h *.y *.l


# Remaking configuration
# ----------------------
configure: configure.in
	$(SHELL) $(AUTOCONF)

main.mk: $(srcdir)/main_in.mk $(srcdir)/configure.in config.status
	$(SHELL) ./config.status

Makefiles: makefiles

makefiles: config.status

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
