
#include <pic18fregs.h>

#include <i2c.h>


void i2c_close(void)
{
  SSPCON1 &= 0xdf;
}
