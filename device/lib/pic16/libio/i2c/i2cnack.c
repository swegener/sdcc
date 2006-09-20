
#include <pic18fregs.h>

#include <i2c.h>

void i2c_nack(void)
{
  SSPCON2bits.ACKDT = 1;
  SSPCON2bits.ACKEN = 1;
}
