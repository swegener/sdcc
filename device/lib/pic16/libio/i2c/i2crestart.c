
#include <pic18fregs.h>

#include <i2c.h>

void i2c_restart(void)
{
  SSPCON2bits.RSEN = 1;
}
