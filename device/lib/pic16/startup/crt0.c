/*
 * crt0.c - SDCC pic16 port runtime start code
 *
 *
 * Converted for SDCC and pic16 port
 * by Vangelis Rokas (vrokas@otenet.gr)
 *
 * based on Microchip MPLAB-C18 startup files
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 * In other words, you are welcome to use, share and improve this program.
 * You are forbidden to forbid anyone else to use, share and improve
 * what you give them.   Help stamp out software-hoarding!  
 *
 * $Id$
 */

extern stack;
extern stack_end;

extern TBLPTRU;

/* external reference to the user's main routine */
extern void main (void);

/* prototype for the startup function */
void _entry (void) _naked interrupt 0;
void _startup (void) _naked;


/*
 * entry function, placed at interrupt vector 0 (RESET)
 */

void _entry (void) _naked interrupt 0
{
  _asm goto __startup _endasm;
}


void _startup (void) _naked
{
  _asm
    // Initialize the stack pointer
    lfsr 1, _stack_end
    lfsr 2, _stack_end
    clrf _TBLPTRU, 0	// 1st silicon doesn't do this on POR
    
    // initialize the flash memory access configuration. this is harmless
    // for non-flash devices, so we do it on all parts.
    bsf 0xa6, 7, 0
    bcf 0xa6, 6, 0

  _endasm ;
    
  /* Call the user's main routine */
  main();

loop:
  /* return from main will lock up */
  goto loop;
}

