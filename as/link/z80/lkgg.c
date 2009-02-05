/* lkgg.c

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
 * P. Felber
 */

#ifdef GAMEGEAR

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "aslink.h"

#define CARTSIZE ((unsigned long)4*16UL*1024UL)
#define NBSEG 8UL
#define SEGSIZE (CARTSIZE/NBSEG)

unsigned char *cart[NBSEG];

#define ROMSIZE 0x10000UL
#define BANKSIZE 0x4000UL

int current_rom_bank;

VOID gg(int in)
{
  static int first = 1;
  unsigned long pos;
  int i;

  if(first) {
    for(i = 0; i < NBSEG; i++) {
      if((cart[i] = malloc(SEGSIZE)) == NULL) {
        fprintf(stderr, "ERROR: can't allocate %dth segment of memory (%d bytes)\n", i, (int)SEGSIZE);
        exit(EXIT_FAILURE);
      }
      memset(cart[i], 0, SEGSIZE);
    }
    first = 0;
  }
  if(in) {
    if(rtcnt > 2) {
      if(hilo == 0)
        pos = rtval[0] | (rtval[1]<<8);
      else
        pos = rtval[1] | (rtval[0]<<8);

      /* Perform some validity checks */
      if(pos >= ROMSIZE) {
        fprintf(stderr, "ERROR: address overflow (addr %lx >= %lx)\n", pos, ROMSIZE);
        exit(EXIT_FAILURE);
      }
      if(current_rom_bank > 1)
        pos += (current_rom_bank-1)*BANKSIZE;
      for(i = 2; i < rtcnt; i++) {
        if(rtflg[i]) {
          if(pos < CARTSIZE) {
            if(cart[pos/SEGSIZE][pos%SEGSIZE] != 0)
              fprintf(stderr, "WARNING: wrote twice at addr %lx (%02X->%02X)\n", pos, rtval[i], cart[pos/SEGSIZE][pos%SEGSIZE]);
            cart[pos/SEGSIZE][pos%SEGSIZE] = rtval[i];
          } else {
            fprintf(stderr, "ERROR: cartridge size overflow (addr %lx >= %lx)\n", pos, CARTSIZE);
            exit(EXIT_FAILURE);
          }
          pos++;
        }
      }
    }
  } else {
    /* EOF */
    /* Patch before calculating the checksum */
    for(i = 0; i < NBSEG; i++)
      fwrite(cart[i], 1, SEGSIZE, ofp);
  }
}

#endif /* GAMEGEAR */
