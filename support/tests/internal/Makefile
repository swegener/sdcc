TOPDIR = ../../..

LIBSOURCE = \
	$(TOPDIR)/src/SDCChasht.c \
	$(TOPDIR)/support/Util/NewAlloc.c \
	$(TOPDIR)/support/Util/SDCCerr.c \
	$(TOPDIR)/src/SDCCmacro.c \
	stubs.c

SOURCES = testmacro.c $(LIBSOURCE)

include $(TOPDIR)/Makefile.common

CFLAGS += -I$(TOPDIR)/src -I$(TOPDIR)/support/Util -I$(TOPDIR)

all: all-tests

all-tests: $(OBJ)
	$(CC) -o $@ $(OBJ)

