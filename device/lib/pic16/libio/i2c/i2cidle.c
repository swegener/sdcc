
#include <pic18fregs.h>

#include <i2c.h>

void i2c_idle(void)
{
  while((SSPCON2 & 0x1f) | (SSPSTATbits.R_W));
}
