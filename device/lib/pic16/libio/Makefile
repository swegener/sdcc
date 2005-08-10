###########################################################
### Makefile for the GNU PIC Library
###
### Copyright (C) 2005 by Raphael Neider <rneider@web.de>
###
### The GNU PIC Library was originally designed and
### implemented by
###	Vangelis Rokas <vrokas@otenet.gr>
###
### It is currently maintained by
###	Raphael Neider <rneider@web.de>
###
### This file may be distributed under the terms of the the
### GNU General Public License (GPL). See COPYING for details.
###
### $Id$
###

topsrcdir=..

SUBDIRS = adc i2c usart

ifeq ($(origin IO_ARCH),undefined)
include $(topsrcdir)/Makefile.common

all clean clean-intermediate install : builddir
	$(Q)cat $(topsrcdir)/pics.build \
		| $(SED) -e "s,[ \t]*#.*$$,," \
		| $(GREP) -v "^[ \t]*$$" \
		| while read arch; do \
			[ "x$${arch}" = "x" ] \
			|| MKLIB=libio18f$${arch}.lib $(MAKE) builddir=$(builddir)/$${arch} ARCH=$${arch} IO_ARCH=$${arch} $(MAKECMDGOALS) \
			|| exit 1; \
	done

include $(topsrcdir)/Makefile.rules

else

include $(topsrcdir)/Makefile.subdir

# prevent library to be build in $(SUBDIRS) as well
unexport MKLIB

endif
