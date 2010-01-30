/*-------------------------------------------------------------------------
  _startup.c - startup routine for sdcc

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

/* External startup code can be written in C
   here .. Special usage if this routine
   returns a non-zero value then global &
   static variable initialisation will be skipped.
   Beware not to use initialized variables as they
   are not initialized yet nor to use pdata/xdata
   variables if external data memory needs to be
   enabled first. */

#if defined(SDCC_ds390) || defined(SDCC_ds400)

/* Disable "ISO C forbids an empty source file" warning message */
#pragma disable_warning 190

#elif defined(SDCC_mcs51) || defined(SDCC_z80)

unsigned char _sdcc_external_startup (void) __nonbanked
{
    return 0;
}

#else

unsigned char _sdcc_external_startup ()
{
    return 0;
}

#endif
