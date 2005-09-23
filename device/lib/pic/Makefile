###########################################################
### Makefile for the SDCC/PIC14 Library
###
### Copyright (C) 2005 by Raphael Neider <rneider@web.de>
###
### The library is currently maintained by
###	Raphael Neider <rneider@web.de>
###
### This file may be distributed under the terms of the the
### GNU General Public License (GPL). See GPL for details.
###
### $Id$
###

topsrcdir = .

include $(topsrcdir)/Makefile.common

ifeq (:,$(findstring :,$(LD) $(AS) $(LIB)))

.PHONY : all

all % : 
	@echo "gputils not found -- PIC14 library not built!"

else

SUBDIRS	   = libsdcc
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
