#
# Makefile - Makefile to build pic16 USART communications library
#
# This file is part of the GNU PIC Library.
#
# January, 2005
# The GNU PIC Library is maintained by,
# 	Vangelis Rokas <vrokas@otenet.gr>
#
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

include ../Makefile.rules

SRCS	=	uclose \
		ugetc \
		uopen \
		usartd \
		ubusy \
		udrdy \
		ugets \
		uputc \
		uputs

#ubaud 

CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))


DEBUG=
#COMPILE_FLAGS   += --pomit-config-words --pomit-ivt --no-peep
COMPILE_FLAGS	+= $(DEBUG)

CFILES	= $(patsubst %,%.c,$(SRCS))
OFILES	= $(patsubst %.c,%.o,$(CFILES))

%.o: %.c
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -c $<

build-library: $(OFILES)

