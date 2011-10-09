/* rabbitext.c

   Copyright (C) 1989-1995 Alan R. Baldwin
   721 Berkeley St., Kent, Ohio 44240

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * Extensions: P. Felber
 *
 * Altered by Leland Morrison to support rabbit 2000 
 *   and rabbit 4000 instruction sets (2011)
 */

#include <stdio.h>
#include <setjmp.h>
#include "asxxxx.h"
#include "rabbit.h"

char	*cpu	= "Rabbit 2000/4000";
int	hilo	= 0;
char	*dsft	= "asm";
