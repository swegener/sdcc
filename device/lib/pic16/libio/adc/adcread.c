
#include <pic18fregs.h>

#include <adc.h>


int adc_read(void)
{
  union {
  	int ri;
  	char rb[2];
  } result;

	result.rb[0] = ADRESL;
	result.rb[1] = ADRESH;

  return (result.ri);
}
