
/*
 * adcbusy - check whether the AD module is busy
 *
 * written by Vangelis Rokas, 2004 <vrokas AT otenet.gr>
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
 */

#include <pic18fregs.h>
#include <adc.h>


char adc_busy(void) __naked
{
#if defined(__SDCC_ADC_STYLE65J50)
  WDTCONbits.ADSHR = 0; /* access ADCON0/1 */
#endif
#if 0
  return (ADCON0bits.GO);
#else
#if defined(__SDCC_ADC_STYLE242)
  __asm
    movlw       0x00
    btfsc       _ADCON0bits, 2
    addlw       0x01
    return
  __endasm;
#elif defined(__SDCC_ADC_STYLE1220) \
   || defined(__SDCC_ADC_STYLE13K50) \
   || defined(__SDCC_ADC_STYLE2220) \
   || defined(__SDCC_ADC_STYLE24J50) \
   || defined(__SDCC_ADC_STYLE65J50)
  __asm
    movlw       0x00
    btfsc       _ADCON0bits, 1
    addlw       0x01
    return
  __endasm;
#else /* unsupported ADC style */
#error Unsupported ADC style.
#endif
#endif
}

