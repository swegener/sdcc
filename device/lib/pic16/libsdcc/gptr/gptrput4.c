/*-------------------------------------------------------------------------

   gptrput4.c :- put 4 byte value at generic pointer

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

/* write address is expected to be in WREG:FSR0H:FSR0L while
 * write value is in TBLPTRH:TBLPTRL:PRODH:[stack] */
 
extern POSTINC0;
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
extern PRODH;

void _gptrput4(void) _naked
{
  _asm
    /* decode generic pointer MSB (in WREG) bits 6 and 7:
     * 00 -> code (unimplemented)
     * 01 -> EEPROM (unimplemented)
     * 10 -> data
     * 11 -> unimplemented
     */
    btfss	_WREG, 7
    bra		_lab_01_
    
    /* data pointer  */
    /* data are already in FSR0 */
    movff	_PRODL, _FSR0H
    
    movff	_PREINC1, _POSTINC0
    movff	_PRODH, _POSTINC0
    movff	_TBLPTRL, _POSTINC0
    movff	_TBLPTRH, _POSTINC0
    
    return
    

_lab_01_:
    /* code or eeprom */
    btfss	_WREG, 6
    return
    
    /* code pointer, cannot write code pointers */
    
  
_lab_02_:
    /* EEPROM pointer */

    /* unimplemented yet */


  return
  _endasm;
}
