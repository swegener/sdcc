#
# Makefile - Makefile to build pic16 float support library
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


LIB	= sdccfloat.lib

SRCS	=	fs2schar \
		fs2sint \
		fs2slong \
		fs2uchar \
		fs2uint \
		fs2ulong \
		fsadd \
		fsdiv \
		fseq \
		fsgt \
		fslt \
		fsmul \
		fsneq \
		fssub \
		schar2fs \
		sint2fs \
		slong2fs \
		uchar2fs \
		uint2fs \
		ulong2fs

LOCAL_FLAGS	=
COMPILE_FLAGS	+= $(LOCAL_FLAGS)

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
		echo -n "$$object " ; \
	done ;
	@echo
