
#include <pic18fregs.h>

#include <i2c.h>


char i2c_readstr(_I2CPARAM_SPEC unsigned char *ptr, unsigned char len)
{
  unsigned char count=0;
  
  while( len-- ) {
    *ptr++ = i2c_readchar();
    
    while(SSPCON2bits.RCEN) {
      if(PIR2bits.BCLIF)return (-1);
      count++;
    
      if(len) {
        I2C_ACK();
        while(SSPCON2bits.ACKEN);
      }
    }
  }
  
  return count;
}
