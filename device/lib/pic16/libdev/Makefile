#
# Makefile  - Makefile to build device libraries
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

include ../Makefile.common

# The SDCC project directory
PRJDIR	= ../../../..

# headers directory
INCDIR	= $(PRJDIR)/device/include


# Supported devices
DEVS	= 	18f242 \
		18f248 \
		18f252 \
		18f258 \
		18f442 \
		18f448 \
		18f452 \
		18f458 \
		18f1220 \
		18f2220 \
		18f6520 \
		18f6620 \
		18f6680 \
		18f6720 \
		18f8520 \
		18f8620 \
		18f8680 \
		18f8720


# pic16 port specific headers
PIC16_INCDIR	= $(INCDIR)/pic16

CFLAGS	= -I$(PIC16_INCDIR)

#COMPILE_FLAGS	= --pomit-config-words --pomit-ivt --denable-peeps --optimize-goto --obanksel=2
COMPILE_FLAGS	+= $(MODELFLAGS) --nostdinc


CFILES	= $(patsubst %,pic%.c, $(DEVS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))
LFILES	= $(patsubst %.c,%.lib,$(CFILES))

PFILE	= $(patsubst pic%.c,%,$<)
LFILE	= $(patsubst %.c,%.lib,$<)


.c.o:
	$(CC) -p$(PFILE) $(CFLAGS) $(COMPILE_FLAGS) -c $<

%.lib: %.o
	$(AR) -c $@ $<
	
all: make-lib

make-lib: $(LFILES)
	@$(MV) -v $(LFILES) ../bin
	
$(LFILES): $(OFILES)

$(OFILES): $(CFILES)


clean-intermediate:
	$(RM) -f *.lst *.asm *.dump*
	
clean: clean-intermediate
	$(RM) -f *.o

real-clean: clean
	$(RM) -f .depend

dep .depend:
	rm -f .depend
	for temp in $(CFILES); do		\
		$(CPP) $(MM) $(CFLAGS) $$temp > .tmpdepend;	\
		$(SED) s/.rel/.o/g .tmpdepend >> .depend;	\
		$(RM) -f .tmpdepend;	\
	done
	
include .depend
