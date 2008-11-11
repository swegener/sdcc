
/*
 * adcopen - initialize AD module
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


/* parameters are:
 *   channel: one of ADC_CHN_*
 *   fosc:    one of ADC_FOSC_*
 *   pcfg:    one of ADC_CFG_* (a bitmask with set bits denoting digital ports for 242-style)
 *   config:  ADC_FRM_* | ADC_INT_* | ADC_VCFG_*
 */

void adc_open(unsigned char channel, unsigned char fosc, unsigned char pcfg, unsigned char config)
{
  /* disable ADC */
  ADCON0 = 0;

#if defined(__SDCC_ADC_STYLE242)
  ADCON0 = ((channel & 0x07) << 3) | ((fosc & 0x03) << 6);
  ADCON1 = (pcfg & 0x0f) | (config & ADC_FRM_RJUST);
  if (fosc & 0x04) {
    ADCON1bits.ADCS2 = 1;
  }
#elif defined (__SDCC_ADC_STYLE1220)
  ADCON0 = ((channel & 0x07) | (config & ADC_VCFG_AN3_AN2)) << 2;
  ADCON1 = (pcfg & 0x7f);
  ADCON2 = (ADCON2 & 0x38) | (fosc & 0x07) | (config & ADC_FRM_RJUST);
#elif defined(__SDCC_ADC_STYLE2220)
  ADCON0 = (channel & 0x0f) << 2;
  /* XXX: Should be (pcfg & 0x0f) as VCFG comes from config,
   * but we retain compatibility for now ... */
  ADCON1 = (pcfg & 0x3f) | (config & ADC_VCFG_AN3_AN2);
  ADCON2 = (ADCON2 & 0x38) | (fosc & 0x07) | (config & ADC_FRM_RJUST);
#else /* unsupported ADC style */
#error Unsupported ADC style.
#endif

  if (config & ADC_INT_ON) {
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
  }

  /* enable the A/D module */
  ADCON0bits.ADON = 1;
}

