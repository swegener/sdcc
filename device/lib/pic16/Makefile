#
# Makefile - toplevel makefile that builds everything
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


include ./Makefile.common


DIRS	=	libdev	\
		libsdcc	\
		startup \
		debug


all: build-all-libraries

build-all-libraries:
	@for temp in $(DIRS) ; do \
		$(MAKE) -C $$temp MODELFLAGS=$(MODELFLAGS) ; \
	done


clean: all-clean
	$(RM) -f bin/*.*



all-clean:
	@for temp in $(DIRS) ; do \
		$(MAKE) -C $$temp clean ; \
	done

clean-intermediate:
	@for temp in $(DIRS) ; do \
		$(MAKE) -C $$temp clean-intermediate ; \
	done


dep: all-dep

all-dep:
	for temp in $(DIRS) ; do \
		$(MAKE) -C $$temp dep ; \
	done
