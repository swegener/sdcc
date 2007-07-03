/*-------------------------------------------------------------------------
  typeof.h - Contains enumerations of values returned by __typeof
 
             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (2001)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#ifndef __SDC51_TYPEOF_H
#define __SDC51_TYPEOF_H 1

#define TYPEOF_INT        1
#define TYPEOF_SHORT      2
#define TYPEOF_CHAR       3
#define TYPEOF_LONG       4
#define TYPEOF_FLOAT      5
#define TYPEOF_FIXED16X16 6
#define TYPEOF_BIT        7
#define TYPEOF_BITFIELD   8
#define TYPEOF_SBIT       9
#define TYPEOF_SFR        10
#define TYPEOF_VOID       11
#define TYPEOF_STRUCT     12
#define TYPEOF_ARRAY      13
#define TYPEOF_FUNCTION   14
#define TYPEOF_POINTER    15
#define TYPEOF_FPOINTER   16
#define TYPEOF_CPOINTER   17
#define TYPEOF_GPOINTER   18
#define TYPEOF_PPOINTER   19
#define TYPEOF_IPOINTER   20
#define TYPEOF_EEPPOINTER 21

#endif
