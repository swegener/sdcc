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

SRCS	= 	atof \
		atoi \
		atol \
		calloc \
		free \
		ltoa \
		malloc \
		memfree \
		memfreemax \
		memmisc \
		realloc \
		x_ftoa \
		putchar \
		rand \
		crc16

S_SRCS	= g_ftoa

#CFILES	= $(patsubst %,%.c,$(SRCS))
#OFILES	= $(patsubst %.c,%.o,$(CFILES))

#OPT_FLAGS	+= --stack-auto

DEBUG=
#COMPILE_FLAGS   += --pomit-config-words --pomit-ivt --denable-peeps --optimize-goto --obanksel=2
COMPILE_FLAGS	+= $(DEBUG)

#CFILES	= $(patsubst %,%.c,$(SRCS))
#OFILES	= $(patsubst %.c,%.o,$(CFILES))

#.c.o
#	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

build-library: $(OFILES)

