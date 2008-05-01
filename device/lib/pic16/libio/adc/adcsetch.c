
/*
 * adcsetch - select convertion channel
 *
 * written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
 *
 * Devices implemented:
 *	PIC18F[24][45][28]
 *	PIC18F2455-style
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
 * $Id$
 */

#include <pic18fregs.h>
#include <adc.h>


void adc_setchannel(unsigned char channel) __naked
{
#if 0
#if defined(__SDCC_ADC_STYLE2455)
  ADCON0 &= ~(0xf << 2);
  ADCON0 |= channel << 2;
#else /* all other devices */
  ADCON0 &= ~(0x7 << 3);
  ADCON0 |= channel << 3;
#endif
#else
  (void)channel;

  __asm
#if defined(__SDCC_ADC_STYLE2455)
    movlw       0xc3
#else /* all other devices */
    movlw       0xc7
#endif
    andwf       _ADCON0, f
    
    movlw       0x01
    movf        _PLUSW1, w
#if defined(__SDCC_ADC_STYLE2455)
#else /* all other devices */
    rlcf        _WREG, w
#endif
    rlcf        _WREG, w
    rlcf        _WREG, w

    iorwf       _ADCON0, f

    return
  __endasm;
#endif    
}

