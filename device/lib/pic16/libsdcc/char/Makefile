#
# Makefile - Makefile to build pic16 char support library
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


LIB	= sdccchar.lib

SRCS	=	divschar \
		divuchar \
		modschar \
		moduchar

include ../Makefile.rules


all: build-library

build-library: $(LIB)

$(LIB): $(OFILES)
	@echo Creating $(LIB) ...
	@for object in $(OFILES) ; do \
		if [ ! -e $(LIB) ]; then \
			$(AR) -c $(LIB) $$object ; \
		else \
			$(AR) -r $(LIB) $$object ; \
		fi; \
		echo adding $$object ; \
	done ;
