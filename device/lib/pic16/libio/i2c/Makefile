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

SRCS	=	i2cack \
		i2cclose \
		i2cdrdy \
		i2cidle \
		i2cnack \
		i2copen \
		i2creadc \
		i2creads \
		i2crestart \
		i2cstart \
		i2cstop \
		i2cwritec \
		i2cwrites


CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))


DEBUG=
#COMPILE_FLAGS   += --pomit-config-words --pomit-ivt --no-peep
COMPILE_FLAGS	+= $(DEBUG)

CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))

%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

build-library: $(OFILES)

