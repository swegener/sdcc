# Generated automatically from Makefile.in by configure.
#
#
#

VERSION         = 2.1.9Ga
VERSIONHI       = 2
VERSIONLO       = 1
VERSIONP        = 9Ga

SHELL		= /bin/sh
CC		= gcc
CPP		= gcc -E
INSTALL		= /usr/bin/install -c

PRJDIR		= ../..

srcdir		= .
prefix		= /usr/local
exec_prefix     = ${prefix}
bindir          = ${exec_prefix}/bin
libdir          = ${exec_prefix}/lib
datadir         = ${prefix}/share
includedir      = ${prefix}/include
mandir          = ${prefix}/man
man1dir         = $(mandir)/man1
man2dir         = $(mandir)/man2
infodir         = ${prefix}/info

STD_INC		= @sdcc_include_dir@
CPPFLAGS	=  -I. -I$(PRJDIR) -I$(PRJDIR)/support
CFLAGS		= -ggdb -O2 -pipe
M_OR_MM		= -MM
LDFLAGS		= 
LIBS            = -lgc 
LIBDIRS         = -L$(PRJDIR)/support/gc


OBJECTS		= sdcdb.o symtab.o simi.o $(PRJDIR)/src/SDCCset.o \
		  break.o cmd.o $(PRJDIR)/src/SDCChasht.o
SOURCES		= $(patsubst %.o,%.c,$(OBJECTS))

TARGET		= $(PRJDIR)/bin/sdcdb


# Compiling entire program or any subproject
# ------------------------------------------
all: checkconf $(TARGET)

$(PRJDIR)/support/gc/libgc.a:
	cd $(PRJDIR)/support/gc && $(MAKE)

# Compiling and installing everything and runing test
# ---------------------------------------------------
install: all installdirs
	$(INSTALL) -s $(TARGET) $(bindir)/sdcdb
	cp $(PRJDIR)/sdcdb/sdcdb.el $(bindir)/sdcdb.el
	cp $(PRJDIR)/sdcdb/sdcdbsrc.el $(bindir)/sdcdbsrc.el


# Deleting all the installed files
# --------------------------------
uninstall:
	rm -f $(bindir)/sdcdb


# Performing self-test
# --------------------
check:


# Performing installation test
# ----------------------------
installcheck:


# Creating installation directories
# ---------------------------------
installdirs:
	$(INSTALL) -d $(bindir)


# Creating dependencies
# ---------------------
dep: Makefile.dep

Makefile.dep: $(SOURCES) *.h $(PRJDIR)/*.h
	$(CPP) $(CPPFLAGS) $(M_OR_MM) $(SOURCES) >Makefile.dep

include Makefile.dep
include clean.mk

# My rules
# --------
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBDIRS) $(LIBS)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<


# Remaking configuration
# ----------------------
checkconf:
	@if [ -f $(PRJDIR)/devel ]; then\
          $(MAKE) -f conf.mk srcdir="$(srcdir)" PRJDIR="$(PRJDIR)" freshconf;\
        fi

# End of cpp/Makefile
