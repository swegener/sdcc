#
# Makefile - Makefile to build pic16 C Library
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

include ../Makefile.rules

CSRCS	=

SSRCS	=	cnvfrac \
		cnvint

SRCS	= $(CSRCS)
# $(SSRCS)

CFILES	= $(patsubst %,%.c,$(CSRCS))
SFILES	= $(patsubst %,%.S,$(SSRCS))

COFILES	= $(patsubst %.c,%.o,$(CFILES))
SOFILES	= $(patsubst %.S,%.o,$(SFILES))

OFILES	= $(COFILES)
OFILES	+= $(SOFILES)

DEBUG=
#COMPILE_FLAGS   += --pomit-config-words --pomit-ivt --no-peep
COMPILE_FLAGS	+= $(DEBUG)

%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

%.o: %.S
	$(AS) -c $<


build-library: $(OFILES)

