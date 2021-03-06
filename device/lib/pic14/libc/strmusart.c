/*-------------------------------------------------------------------------
   strmusart.c - usart stream putchar

   Copyright (C) 2004, Vangelis Rokas <vrokas AT otenet.gr>

   Modifications for PIC14 by
   Copyright (C) 2019 Gonzalo Pérez de Olaguer Córdoba <salo@gpoc.es>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <pic16regs.h>

/* note that USART should already been initialized */
static int
__stream_usart_out (char c, FILE *stream)
{
  (void)stream;
  while (!TXIF);
  TXREG = c;
  /* wait until the transmit buffer is empty */
  while (!TRMT);
  return 0;
}

static FILE f = __stream_usart_out;
FILE *usart_out = &f;
