/*-------------------------------------------------------------------------

   gptrget4.c :- get 4 byte value from generic pointer

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

/* the return value is expected to be in WREG:PRODL, therefore we choose return
 * type void here. Generic pointer is expected to be in WREG:FSR0H:FSR0L,
 * so function arguments are void, too */

extern POSTINC0;
extern INDF0;
extern FSR0L;
extern FSR0H;
extern WREG;
extern TBLPTRL;
extern TBLPTRH;
extern TBLPTRU;
extern TABLAT;
extern PRODL;
extern PRODH;

void _gptrget4(void)
{
  _asm
    /* decode generic pointer MSB (in WREG) bits 6 and 7:
     * 00 -> code
     * 01 -> EEPROM
     * 10 -> data
     * 11 -> unimplemented
     */
    btfss	_WREG, 7
    goto	_lab_01_
    
    /* data pointer  */
    /* data are already in FSR0 */
    movf	_POSTINC0, w
    movff	_POSTINC0, _PRODL
    movff	_POSTINC0, _PRODH
    movff	_POSTINC0, _FSR0L
    
    goto _end_
    

_lab_01_:
    /* code or eeprom */
    btfsc	_WREG, 6
    goto	_lab_02_
    
    ; code pointer
    movwf	_TBLPTRU
    movff	_FSR0L, _TBLPTRL    
    movff	_FSR0H, _TBLPTRH
    
    /* fetch first byte */
    TBLRD*+
    movf	_TABLAT, w

    /* fetch second byte  */
    TBLRD*+
    movff	_TABLAT, _PRODL
    
    /* fetch third byte */
    TBLRD*+
    movff	_TABLAT, _PRODH
    
    /* fetch fourth byte */
    TBLRD*+
    movff	_TABLAT, _FSR0L
    
    goto _end_
 
  
_lab_02_:
    /* EEPROM pointer */

    /* unimplemented yet */

_end_:

  _endasm;
}
