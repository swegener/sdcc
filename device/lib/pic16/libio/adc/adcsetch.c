
#include <pic18fregs.h>

#include <adc.h>


void adc_setchannel(unsigned char channel)
{
	ADCON0 &= ~(0x7 << 3);
	ADCON0 |= channel << 3;
}
