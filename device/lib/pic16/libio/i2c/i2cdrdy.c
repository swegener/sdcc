
#include <pic18fregs.h>

#include <i2c.h>


unsigned char i2c_drdy(void)
{
  if(SSPSTATbits.BF)
    return (+1);
  else
    return (0);
}
