
#include <pic18fregs.h>

#include <adc.h>


void adc_close(void)
{
	ADCON0bits.ADON = 0;
	PIE1bits.ADIE = 0;
}
