#
# Makefile - Makefile to build pic16 Math Library
#
# This file is part of the GNU PIC Library.
#
# January, 2004
# The GNU PIC Library is maintained by,
# 	Vangelis Rokas <vrokas@otenet.gr>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
#
# $Id$
#
#

include ../Makefile.common

PRJDIR	= ../../../..

LIB	= libm18f.lib

SRCS	=	acosf  \
		asincosf  \
		asinf  \
		atan2f  \
		atanf  \
		ceilf  \
		cosf  \
		coshf  \
		cotf  \
		errno  \
		expf  \
		fabsf  \
		floorf  \
		frexpf  \
		ldexpf  \
		log10f  \
		logf  \
		modff  \
		powf  \
		sincosf  \
		sincoshf  \
		sinf  \
		sinhf  \
		sqrtf  \
		tancotf  \
		tanf  \
		tanhf

COMPILE_FLAGS	+= $(MODELFLAGS) $(OPT_FLAGS)

CFLAGS	+= $(CSTD)
CFLAGS += -I $(PRJDIR)/device/include/pic16


CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))

%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

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
	@mv -v $(LIB) ../bin


all-clean: clean

clean-intermediate:
	@echo Removing intermediate files ...
	$(RM) -f *.lst *.asm

clean: clean-intermediate
	$(RM) -f $(LIB) *.o

dep .depend:
	-rm .depend
	@for source in $(CFILES); do                            \
		$(CC) $(MM) $(CFLAGS) $$source > .tmpdepend ;   \
		$(SED) s/.rel/.o/g .tmpdepend >> .depend ;      \
		$(RM) -f .tmpdepend;                            \
	done

include .depend

