#
# Makefile - Makefile to build pic16 C Library
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

include ../Makefile.rules

SRCS    =       iscntrl \
                isdigit \
                isgraph \
                islower \
                isprint \
                ispunct \
                isspace \
                isupper \
                isxdigit


CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))


DEBUG=
#COMPILE_FLAGS   += --pomit-config-words --pomit-ivt --denable-peeps --optimize-goto --obanksel=2
COMPILE_FLAGS	+= $(DEBUG)

CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))

%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

build-library: $(OFILES)

