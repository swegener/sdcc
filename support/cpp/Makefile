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
CPPFLAGS	=  -I. -I$(PRJDIR) -D_FORASXXXX_
CFLAGS		= -ggdb -O2 -pipe
M_OR_MM		= -MM
LDFLAGS		= 

OBJECTS		= cppalloc.o cpperror.o cppexp.o cpphash.o cpplib.o cppmain.o 
SOURCES		= $(patsubst %.o,%.c,$(OBJECTS))

TARGET		= $(PRJDIR)/bin/sdcpp


# Compiling entire program or any subproject
# ------------------------------------------
all: checkconf $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

# Compiling and installing everything and runing test
# ---------------------------------------------------
install: all installdirs
	$(INSTALL) -s $(TARGET) $(bindir)/sdcpp


# Deleting all the installed files
# --------------------------------
uninstall:
	rm -f $(bindir)/sdcpp


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
.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<


# Remaking configuration
# ----------------------
checkconf:
	@if [ -f $(PRJDIR)/devel ]; then\
          $(MAKE) -f conf.mk srcdir="$(srcdir)" PRJDIR="$(PRJDIR)" freshconf;\
        fi

# End of cpp/Makefile
