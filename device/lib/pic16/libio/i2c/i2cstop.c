
#include <pic18fregs.h>

#include <i2c.h>

void i2c_stop(void)
{
  SSPCON2bits.PEN = 1;
}
