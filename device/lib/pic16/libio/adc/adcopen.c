
#include <pic18fregs.h>

#include <adc.h>


/* parameters are:
 *   channel: one of ADC_CHN_*
 *   fosc:    one of ADC_FOSC_*
 *   pcfg:    one of ADC_CFG_*
 *   config:  ADC_FRM_*  |  ADC_INT_*
 */

void adc_open(unsigned char channel, unsigned char fosc, unsigned char pcfg, unsigned char config)
{
	ADCON0 = 0;
	ADCON1 = 0;

	/* setup channel */
	ADCON0 |= (channel & 0x07) << 3;

	/* setup fosc */
	ADCON0 |= (fosc & 0x03) << 6;
	ADCON1 |= (fosc & 0x04) << 4;
	
	/* setup reference and pins */
	ADCON1 |= pcfg & 0x0f;
	
	ADCON0 |= (config & ADC_FRM_RJUST);
	
	if(config & ADC_INT_ON) {
		PIR1bits.ADIF = 0;
		PIE1bits.ADIE = 1;
		INTCONbits.PEIE = 1;
	}
	
	/* enable the A/D module */
	ADCON0bits.ADON = 1;
}
