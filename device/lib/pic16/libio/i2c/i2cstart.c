
#include <pic18fregs.h>

#include <i2c.h>

void i2c_start(void)
{
  SSPCON2bits.SEN = 1;
}
