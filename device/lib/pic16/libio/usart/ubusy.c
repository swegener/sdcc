
/*
 * ubusy - return USART TX state
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

unsigned char usart_busy(void) __naked
{
#if 0
  if(!TXSTAbits.TRMT)return 1;
  else return 0;
#else
  __asm
    movlw       0x00
    btfss       _TXSTAbits, 1
    addlw       0x01
    return
  __endasm;
#endif
}
