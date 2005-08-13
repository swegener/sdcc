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

SUBDIRS = 
MKLIB	= device_libs
C_FILES = $(wildcard *.c)
TARGETS = $(OBJS:.o=.lib)

# allow invoking make without arguments (otherwise
# [...]/devicelibs would be the default target)
suball : all

.PHONY : $(topsrcdir)/$(builddir)/device_libs suball

$(topsrcdir)/$(builddir)/device_libs :
	$(Q)+$(foreach dev,$(C_FILES:.c=),$(MAKE) $(topsrcdir)/$(builddir)/$(dev).lib LIB_O=$(topsrcdir)/$(builddir)/$(dev).o;)

include $(topsrcdir)/Makefile.subdir
