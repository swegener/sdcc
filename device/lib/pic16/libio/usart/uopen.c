
/*
 * uopen - initialize USART module
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

// USART Status Structure
extern union USART USART_Status;

void usart_open(unsigned char config, unsigned int spbrg) __wparam
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;

  if(config&0x01)TXSTAbits.SYNC = 1;

  if(config&0x02) {
    TXSTAbits.TX9 = 1;
    RCSTAbits.RX9 = 1;
  }

  if(config&0x04)TXSTAbits.CSRC = 1;

  if(config&0x08)RCSTAbits.CREN = 1;
  else RCSTAbits.SREN = 1;

  if(config&0x10)TXSTAbits.BRGH = 1;
  else TXSTAbits.BRGH = 0;

  /* TX interrupts */
#if defined(pic18f66j60) || defined(pic18f66j65) || \
	defined(pic18f67j60) || defined(pic18f86j60) || \
	defined(pic18f86j65) || defined(pic18f87j60) || \
	defined(pic18f96j60) || defined(pic18f96j65) || \
	defined(pic18f97j60)

  PIR1bits.TXIF_PIR1 = 0;

#else   /* all other devices */

  PIR1bits.TXIF = 0;

#endif

  if(config&0x40)PIE1bits.RCIE = 1;
  else PIE1bits.RCIE = 0;

  /* RX interrupts */
  PIR1bits.RCIF = 0;

#if defined(pic18f66j60) || defined(pic18f66j65) || \
	defined(pic18f67j60) || defined(pic18f86j60) || \
	defined(pic18f86j65) || defined(pic18f87j60) || \
	defined(pic18f96j60) || defined(pic18f96j65) || \
	defined(pic18f97j60)

  if(config&0x80)PIE1bits.TXIE_PIE1 = 1;
  else PIE1bits.TXIE_PIE1 = 0;

#else   /* all other devices */

  if(config&0x80)PIE1bits.TXIE = 1;
  else PIE1bits.TXIE = 0;

#endif

  SPBRG = (char)spbrg;

  TXSTAbits.TXEN = 1;
  RCSTAbits.SPEN = 1;
}
