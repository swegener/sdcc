
#include <pic18fregs.h>

#include <i2c.h>

unsigned char i2c_readchar(void)
{
  SSPCON2bits.RCEN = 1;
  while( !I2C_DRDY() );
  return ( SSPBUF );
}
