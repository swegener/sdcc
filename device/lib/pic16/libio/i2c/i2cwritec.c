
#include <pic18fregs.h>

#include <i2c.h>

char i2c_writechar(unsigned char dat)
{
  SSPBUF = dat;
  if( SSPCON1bits.WCOL ) {
    return -1;
  } else {
    while( I2C_DRDY() );
    return 0;
  }
}
