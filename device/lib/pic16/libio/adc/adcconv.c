
#include <pic18fregs.h>

#include <adc.h>


void adc_conv(void)
{
	ADCON0bits.GO = 1;
}
