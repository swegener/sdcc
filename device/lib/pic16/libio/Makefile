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

MCUS	= $(shell cat ../pics.build)

PROCESSORS	= $(MCUS)


DIRS	= adc i2c

LOBJS	= $(patsubst %,%/*.o,$(DIRS))

# library name example libio18f8720.lib
LIB	= libio18f$(MMCU).lib

all: build-libraries

make-target: build-libraries

build-libraries:
	for proc in $(PROCESSORS) ; do \
		echo "Building libraries for processor pic18f $$proc" ; \
		$(MAKE) -C . build-processor-library MMCU=$$proc ;  \
	done ;

# Should have externally specified PROC
build-processor-library:
	for dir in $(DIRS) ; do	 \
		$(MAKE) -C $$dir clean ; \
		$(MAKE) -C $$dir build-mcu-library MCU=18f$(MMCU); \
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
