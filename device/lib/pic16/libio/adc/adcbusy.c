
#include <pic18fregs.h>

#include <adc.h>


char adc_busy(void)
{
	return (ADCON0bits.GO == 1);
}
