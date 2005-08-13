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

topsrcdir = .

include $(topsrcdir)/Makefile.common

ifeq (:,$(findstring :,$(LD) $(AS) $(LIB)))

.PHONY : all

all % : 
	@echo "gputils not found -- PIC16 library not built!"

else

SUBDIRS	   = debug libc libdev libio libm libsdcc startup
builddir   = build
installdir = bin
export installdir

C_SRC = $(wildcard *.c)
S_SRC = $(wildcard *.S)
OBJS = $(C_SRC:.c=.o) $(S_SRC:.S=.o)

.PHONY : all clean clean-intermediate install

all : install

install : recurse

clean : recurse
	$(Q)-$(RMDIR) "$(topsrcdir)/$(builddir)";

clean-intermediate : recurse

include $(topsrcdir)/Makefile.rules

endif
