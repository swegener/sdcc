/*-------------------------------------------------------------------------

   gptrget1.c :- get 1 byte value from generic pointer

   Adopted for pic16 port by Vangelis Rokas, 2004 (vrokas@otenet.gr)

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
/*
** $Id$
*/

/* the return value is expected to be in WREG, therefore we choose return
 * type void here. Generic pointer is expected to be in WREG:PRODL:FSR0L,
 * so function arguments are void, too */

extern POSTINC0;
extern POSTINC1;
extern PREINC1;
extern INDF0;
extern FSR0L;
extern FSR0H;
extern WREG;
extern TBLPTRL;
extern TBLPTRH;
extern TBLPTRU;
extern TABLAT;
extern PRODL;


void _gptrget1(void) __naked
{
  __asm
    /* decode generic pointer MSB (in WREG) bits 6 and 7:
     * 00 -> code
     * 01 -> EEPROM
     * 10 -> data
     * 11 -> unimplemented
     */
    btfss	_WREG, 7
    bra		_lab_01_
    
    ; data pointer
    ; data are already in FSR0
    movff	_PRODL, _FSR0H    

    movf	_POSTINC0, w

    return
    

_lab_01_:
    ; code or eeprom
    btfsc	_WREG, 6
    bra		_lab_02_
    
    ; code pointer
    movff	_FSR0L, _TBLPTRL
    movff	_PRODL, _TBLPTRH
    movwf	_TBLPTRU
    
    tblrd*+
    
    ; result in WREG
    movf	_TABLAT, w
    
    return 
  
_lab_02_:
    ; EEPROM pointer

    ; unimplemented yet

_end_:

  return
  __endasm;
}
