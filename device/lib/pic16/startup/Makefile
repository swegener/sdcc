#
# Makefile - Makefile to build startup files
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


PRJDIR	= ../../../..
include ../Makefile.common


SRCS	=	crt0	\
		crt0i	\
		crt0iz


# add nostdinc and nostdlib for this device libraries
COMPILE_FLAGS	+= --no-peep --pomit-config-words --pomit-ivt
COMPILE_FLAGS	+= $(MODELFLAGS) --nostdinc --nostdlib

CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))


%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<


all: build-library
	@$(CP) -v $(OFILES) ../bin

build-library: compile-sources

compile-sources: $(OFILES)


clean:
	rm -f *.o *.lst *.asm


clean-intermediate:
	$(RM) -f *.lst *.asm *.dump*
