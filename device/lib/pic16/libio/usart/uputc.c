
/*
 * uputc - write a character to USART
 *
 * written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
 *
 * Devices implemented:
 *	PIC18F[24][45][28]
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * $Id$
 */


#include <pic18fregs.h>

#include <usart.h>

extern union USART USART_Status;

void usart_putc(unsigned char dat) wparam _naked
{
#if 0
  if(TXSTAbits.TX9) {
    TXSTAbits.TX9D = 0;
    if(USART_Status.TX_NINE)TXSTAbits.TX9D = 1;
  }

  TXREG = dat;      // Write the data byte to the USART
#else
  dat;
  _asm
    btfss       _TXSTAbits, 6
    bra         _01_
    
    bcf         _TXSTAbits, 0
    banksel     _USART_Status
    btfsc       _USART_Status, 1, b
    bsf         _TXSTAbits, 0

_01_:
    movwf       _TXREG
    return
    _endasm;
#endif
}
