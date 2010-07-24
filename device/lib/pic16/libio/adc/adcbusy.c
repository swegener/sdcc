/*-------------------------------------------------------------------------
   adcbusy - check whether the AD module is busy

   Copyright (C) 2004, Vangelis Rokas <vrokas AT otenet.gr>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

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
