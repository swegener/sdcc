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

SRCS	=	memchr	\
		memcmp	\
		memcpy	\
		memccpy	\
		memmove	\
		memrchr	\
		memset	\
		strcat	\
		strchr	\
		strcmp	\
		strcpy	\
		strcspn	\
		strlen	\
		strlwr	\
		strncat	\
		strncmp	\
		strncpy	\
		strpbrk	\
		strrchr	\
		strspn	\
		strstr	\
		strtok	\
		strupr	\
		memchrpgm \
		memchrram \
		memcpypgm2ram \
		memcpyram2ram \


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

