#
# Makefile - Makefile to build pic16 support libraries
#
# This file is part of the GNU PIC Library.
#
# January, 2004
# The GNU PIC Library is maintained by,
# 	Vangelis Rokas <vrokas@otenet.gr>
#
# $Id$
#
#


DIRS	=	ctype \
		stdlib \
		stdio \
		string \
		utils

LOBJS	= $(patsubst %,%/*.o,$(DIRS))

LIB	= libc18f.lib

all: build-libraries

make-target: build-libraries

build-libraries:
	for dir in $(DIRS) ; do	 \
		$(MAKE) -C $$dir ; \
	done;
	gplib -c $(LIB) $(LOBJS)
	mv -v $(LIB) ../bin
	

clean-intermediate:
	@for dir in $(DIRS) ; do \
		$(MAKE) -C $$dir clean-intermediate ; \
	done ;
	
clean:
	for dir in $(DIRS) ; do \
		$(MAKE) -C $$dir clean;	\
	done ;
	rm -fv $(LIB)


real-clean: clean
	find -name *.adb -print | xargs -- rm -fv ;
	find -name *.p -print | xargs -- rm -fv ;
	find -name *.d -print | xargs -- rm -fv ;
	find -name *.dump* -print | xargs -- rm -fv ;
	

dep:
	for dir in $(DIRS) ; do \
		$(MAKE) -C $$dir dep; \
	done
